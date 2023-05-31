#include "bitFields.h"
#include <assert.h>

int getBit(int bn, unsigned long val)
{
	assert(bn >= 0 && bn < 64);
	if (1L << bn & val)
		return 1;
	return 0;
}

void setBit(int bn, int new, unsigned long *val)
{
	assert(bn >= 0 && bn < 64);
	if (new)
		(*val) |= 1L << bn;
	else
		(*val) &= ~(1L << bn);
}

long getBitFld(int bs, int len, unsigned long val)
{
	assert(bs >= 0 && bs < 64);
	assert(0 < len && len <= bs + 1);
	unsigned long result = getBitFldU(bs, len, val);
	if (getBit(len, result))
	{
		unsigned long sbits = ((1L << (64 - len)) - 1) << len;
		result |= sbits;
	}
	return result;
}
unsigned long getBitFldU(int bs, int len, unsigned long val)
{
	assert(bs >= 0 && bs < 64);
	assert(0 < len && len <= bs + 1);
	int be = bs - len + 1;
	unsigned long mask = ((1L << len) - 1) << be;
	return (val & mask) >> be;
}

void setBitFld(int bs, int len, unsigned long new, unsigned long *val)
{
	assert(bs >= 0 && bs < 64);
	assert(0 < len && len <= bs + 1);
	int be = bs - len + 1;
	unsigned long mask = ((1L << len) - 1);
	new &= mask;	 // Turn off bits to the left of the field
	new <<= be;		 // Shift new to the proper position
	mask <<= be;	 // Shift mask to the proper position
	(*val) &= ~mask; // Turn off all bits in val in the subfield
	(*val) |= new;	 // Turn on bits in val in the subfield as needed
}