#include "floatx.h"
#include <assert.h>
#include <limits.h> // for CHAR_BIT - number of bits per byte
#include <math.h>   // for isinf and isnan
#include "bitFields.h"

#define doubleExponentBits 11
#define doubleMantissaBits 52

// lng is used to reach to the bitfields of the double value
union hexDouble
{
  double dbl;
  long lng;
} unionVal;

floatx doubleToFloatx(double val, int totBits, int expBits)
{
  assert(totBits > 3 && totBits <= 64 && "total number of bits should be in between 4 and 64");
  assert(expBits > 1 && expBits <= (totBits - 2) && "number of exponent bits should be in between 2 and total number of bits - 2");

  unionVal.dbl = val;
  unsigned long doubleExp = getBitFldU(62, doubleExponentBits, unionVal.lng);
  unsigned long dblFraction = getBitFldU(doubleMantissaBits - 1, doubleMantissaBits, unionVal.lng);

  floatx result = 0;
  // 1. Extracting the sign bit from the double value, and inserting it into the floatx value at the correct position.
  int signBit = (unionVal.lng >> 63);
  setBit(totBits - 1, signBit, &result);
  // 2. Handling the special cases, such as 0.0, infinity, or not-a-number.
  if (doubleExp == 0x0 && dblFraction == 0x0)
  {
    // Zero
    return result;
  }
  if (~doubleExp == 0x0)
  {
    // Infinity
    setBitFld(totBits - 2, expBits, (1L << expBits) - 1, &result);
    // if fraction is not all zero, then its NaN
    if (dblFraction)
    {
      // NaN
      setBit(totBits - 2 - expBits, 1, &result);
    }
    return result;
  }

  /*
  3. Extract the biased exponent from the double value. Check to see if the double value is sub-normal. If so, handle it.
  Check to make sure the floatx exponent won't overflow (or underflow).
  If so, handle as a special case. If not, re-bias the exponent using the floatx bias (which depends on the number of exponent bits)
  and write the result to the correct location in the floatx result.
  */
  int floatxMantissaBits = totBits - expBits - 1;
  int doubleExponentBias = (1 << (doubleExponentBits - 1)) - 1; // 1023
  int floatxExponentBias = (1 << (expBits - 1)) - 1;            // 2**(expBits-1) - 1

  int bias = doubleExp - doubleExponentBias + floatxExponentBias;

  if (bias <= 0x0)
  {
    // Subnormal number
    int shiftCount = 0;
    while ((dblFraction & (1L << 52)) == 0)
    {
      shiftCount = shiftCount + 1;
      dblFraction = dblFraction << 1;
    }
    bias -= shiftCount;
    
    if (bias == 0)
    {
      dblFraction = dblFraction << 1;
    }
    else
    {
      dblFraction |= (1L << 52);
    }
    while (bias < 0)
    {
      dblFraction = dblFraction >> 1;
      bias++;
    }
    setBitFld(totBits - 2, expBits, bias, &result);
    setBitFld(floatxMantissaBits - 1, floatxMantissaBits, dblFraction >> (doubleMantissaBits - floatxMantissaBits), &result);
  }
  else if (bias < (1L << expBits))
  {
    // Normal number
    setBitFld(totBits - 2, expBits, bias, &result);
    setBitFld(floatxMantissaBits - 1, floatxMantissaBits, dblFraction >> (doubleMantissaBits - floatxMantissaBits), &result);
  }
  else
  {
    // The exponent is too big to represent, setting the exponent bits of the result to all 1s.
    // Infinity
    setBitFld(totBits - 2, expBits, (1L << expBits) - 1, &result);
  }

  return result;
}
