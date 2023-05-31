# FloatX - Extended Floating Point

## Introduction

Some applications need the flexibility of floating point numbers, but may not need the full precision offered by the standard float (32 bit) or double (64 bit) implementations of floating point numbers. Or, applications may need higher precision, and are willing to sacrifice the number of exponent bits to get a higher precision. This project asks you implement a generalized floating point number capability; one which uses an arbitrary number of bits, `expBits`, to represent the exponent of a floating point number, and an arbitrary number of bits to represent the fractional part of the number.  We will call this generalized floating point number a "floatx" number.

Other than changing the number of bits for the sub-fields, all the rest of the IEEE conventions for floating point numbers must be followed:

- The left-most (most significant) bit is a sign bit; 0 for positive, 1 for negative
- The next *exBits* bits represent an exponent field, biased by $2^{(expBits-1)} - 1$
- Exponent bits of all ones represents special values like infinity (if fraction bits are all zero), or "Not a Number" (Nan) (if fraction bits are not all zero.)
- The remaining bits represent a fraction field, `F`.  For most numbers, there is an implied "1." in front of the fraction so we interpret it as $1.F$.  
- If the exponent field and the fraction field are all zeroes, that represents the number 0.0.
- If the exponent field is all zeroes, but the fraction field is not zero, then we treat the number as a "de-normalized" number, and assume it has the value of $0.F \times 2^{-bias+1}$

Obviously, creating a floatx number will require a lot of bit manipulation. The purpose of HW04 was to provide bit manipulation tools that you can use in this project. I have found that using bitFields.c and bitFields.h from HW04 significantly reduces the time required to complete this project.

We will use the standard double-precision floating point specification to provide values to convert to the floatx format. The Wikipedia [Double-precision floating-point](https://en.wikipedia.org/wiki/Double-precision_floating-point_format) article does a good job of describing this format.

For the project, you will need to implement a single function, the `doubleToFloatx` function that takes a double precision floating point input argument, along with the total number of bits and the number of exponent bits for the floatx version of the value. The function needs to return the floatx version of the number. We define a floatx number as a 64 bit unsigned integer (an unsigned long). However, we will only be using the rightmost bits (bit positions totBits-1 to zero) for our floatx value.

For example, a floatx number with 12 total bits and 5 exponent bits would use the following bits in a long unsigned value...

<table>
<tr><td>63</td><td>...</td><td>12</td><td>11</td><td>10</td><td>9</td><td>8</td><td>7</td><td>6</td><td>5</td><td>4</td><td>3</td><td>2</td><td>1</td><td>0</td></tr>
<tr><th colspan=3>Padding</th><th>S</th><th colspan=5>Exponent</th><th colspan=6>Fraction</th><tr>
</table>

At a high level, doing the conversion requires several manipulations:

1. Extracting the sign bit from the double value, and inserting it into the floatx value at the correct position.
2. Handle special cases, such as 0.0, infinity, or not-a-number.
3. Extract the biased exponent from the double value. Check to see if the double value is sub-normal. If so, handle it. Check to make sure the floatx exponent won't overflow (or underflow). If so, handle as a special case. If not, re-bias the exponent using the floatx bias (which depends on the number of exponent bits) and write the result to the correct location in the floatx result.
4. Extract the fraction bits from the double value. Determine how many bits are available for the fraction in the floatx value, and truncate or extend the original value, and write the resulting bits to the floatx result.
5. Return the floatx result.

### floatx.h and floatx.c

The floatx.h file declares the single floatx function, `doubleToFloatx`, we will be working on for this project. It would also make sense to work on a `floatxToDouble` function if this were a real library, but doing so adds more work to an already difficult project, and doesn't contribute much more to the educational value of the project, so I've left that out. The floatx.h file also contains a typedef statement to define floatx as an unsigned long (64 bit) integer. You should not have to edit floatx.h.

Your main job will be to complete the definition of the `doubleToFloatx` function in floatx.c. This is the function that takes a double precision floating point value, and returns the floatx representation of that value.

### testFloatx.c

This is a program that uses command line arguments to specify 1) the total number of bits, and 2) the number of exponent bits in a floatx format. It then reads standard input, looking for a double precision floating point value. For each floating point value found in standard input:

- the testFloatx program invokes `doubleToFloatx` with the value read from standard input, and the total bits and exponent size specified in the command line arguments.
- prints the original value, the floatx specification, and the floatx value in hexadecimal.

Any non-whitespace characters from standard input that cannot be translated to a double will cause the program to stop.

We will use UNIX redirection to read and write from files (see the Wikipedia [Redirection(computing)](https://en.wikipedia.org/wiki/Redirection_(computing)) article for more details.) Please do not edit the code in testFloatx.c. There is no need to use file I/O functions to read or write files for this project.

Here is an example of the output produced when your program is coded correctly:

```txt
>./testFloatx 32 8 <test1.txt
             1 = floatx(32,8) : 000000003f800000
           100 = floatx(32,8) : 0000000042c80000
     1.3333333 = floatx(32,8) : 000000003faaaaaa
   1.27563e+45 = floatx(32,8) : 000000007f800000
      6.23e+22 = floatx(32,8) : 00000000655314a2
       7.9e-39 = floatx(32,8) : 00000000005605fc
 3.4028235e+38 = floatx(32,8) : 000000007f7fffff
 3.4028236e+38 = floatx(32,8) : 000000007f7fffff
Input : q not recognized as a double...         quitting

```
