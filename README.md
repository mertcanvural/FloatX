# proj02 - Extended Floating Point

## Introduction

The goal of this assignment is to:

- Learn about IEEE floating point numbers
- Learn about manipulating bits ("bit twiddling") in C
- Understand number type conversions

(Note - former students have described this project when interviewing for jobs or internships and the interviewers have been very impressed!)

We have discussed the internal representation of IEEE floating point numbers (float and double) in . We learned how to use the IEEE standard to represent floating point numbers in 32 bits worth of data in the [Data in C: Floating Point](https://www.cs.binghamton.edu/~tbartens/CS220_Spring_2023/Lectures/L06_C_Float.pdf) lecture.

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

## Provided to you

The proj02 repository already contains some infrastructure to get you started, as follows:

### floatx.h and floatx.c

The floatx.h file declares the single floatx function, `doubleToFloatx`, we will be working on for this project. It would also make sense to work on a `floatxToDouble` function if this were a real library, but doing so adds more work to an already difficult project, and doesn't contribute much more to the educational value of the project, so I've left that out. The floatx.h file also contains a typedef statement to define floatx as an unsigned long (64 bit) integer. You should not have to edit floatx.h.

Your main job will be to complete the definition of the `doubleToFloatx` function in floatx.c. This is the function that takes a double precision floating point value, and returns the floatx representation of that value.

### testFloatx.c

This is a program that uses command line arguments to specify 1) the total number of bits, and 2) the number of exponent bits in a floatx format. It then reads standard input, looking for a double precision floating point value. For each floating point value found in standard input:

- the testFloatx program invokes `doubleToFloatx` with the value read from standard input, and the total bits and exponent size specified in the command line arguments.
- prints the original value, the floatx specification, and the floatx value in hexadecimal.

Any non-whitespace characters from standard input that cannot be translated to a double will cause the program to stop.

We will use UNIX redirection to read and write from files (see the Wikipedia [Redirection(computing)](https://en.wikipedia.org/wiki/Redirection_(computing)) article for more details.) Please do not edit the code in testFloatx.c. There is no need to use file I/O functions to read or write files for this project.

### Makefile

I have provided a Makefile with several targets:

- `clean` Removes all the files Makefile can create
- `testFloatx` - creates the testFloatx executable from testFloatx.c, floatx.c.
- `gdb` - A target to run gdb on the testFloatx executable. This will automatically set a breakpoint at the first instruction in `doubleToFloatx`, and run with command line arguments 16 and 4. *You will need to type in values to convert since stdin is connected to the keyboard in this case*.
- `test` - Runs testFloatx using some easy to debug formats such as 32 8 (for standard `float` format), 64 11 (for standard double format) and 16 4 which does not map to an existing floating point format. The conversion is run on all the numbers in test1.txt which contains several test cases that are useful for debugging.

## Using the HW04 bitField Code

If you want to use the HW04 bitField code in this project, you will need to do the following steps:

1. Copy bitFields.c and bitFields.h from your hw04 repository to your proj02 repository.
2. Do a `git add bitFields.c` and `git add bitFields.h` in the proj02 repository.
3. Edit the Makefile in the proj02 repository by adding bitFields.c and bitFields.h as dependencies for the testFloatx target, and adding bitFields.c in the list of code used to make the testFloatx executable.
4. Add `#include "bitFields.h"` in floatx.c.

If you totally bombed HW04 but would still like to use the bitFields library for proj02, contact the professor and he will share a correct version of the bitFields code with you.

## Coding the project

After reading through this readme, decide whether you want to use the bitFields library. If so, follow the instructions above. Then, start working on floatx.c, implementing the `doubleToFloatx` function. Use `make test` to check your results. You can use on-line floating point calculators to figure out what the correct values should be for a float (32 8) or double (64 23) specification, so get those working first.

You will have to manually verify the results for other specifications, such as a 16 4 floatx specification.

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

Once you have the basics working, think about some other specifications and values that might cause problems, and try those as well.

## Hints and Suggestions

1. It's hard to get the bit values that make up a double. If you try to assign a double to a long, C will convert the double from floating point value to an integer, and put that integer in the long result instead of just copying the bits. The easiest way to get the bit values is to use a `union` in C. I used the following union...

   ```C
   union hexDouble {
     double dbl;
     long lng;
   } unionVal;   
   ```

   Then, you can take the double input parameter, copy it to `unionVal.dbl`, and use `unionVal.lng` to inspect the bits.

2. This project is almost impossible without the use of the shift left `<<` and shift right `>>` C operators. Remember that in C, the right shift operator, `>>`, behaves differently on signed and unsigned data types.

3. When bit twiddling, remember that a literal value is treaded as a 32 bit integer UNLESS it has an "L" suffix! I have spent hours and hours debugging code such as `(1<<pos & value)`, where `value` is a 64 bit (long) value, and `pos` is a position between 0 and 63. The bug is that `1<<pos` says shift a **32** bit value by `pos` bits. If `pos` is greater than 31, the result is undefined! The correct code is `(1L<<pos & value)`.

4. To get a bit field that contains `n` bits, right justified, you can use the bit-twiddle: `(1L<<n)-1`.

5. When the C `scanf` function reads standard input and converts to a double, it accepts many different formats. Several different formats are represented in the example above. Scan also recognizes "inf" for infinity, and on many machines "nan" for "not a number". However, not all versions of the C library recognize "nan" as not a number, including the C library on the LDAP machines. Therefore, we will not use "nan" as a test case when grading.

6. Use the online floating point calculators referenced in the Brightspace/Content/Reference section to double check your work, at least for common specifications such as "32 8" for single-precision floating point numbers.

7. One of the hardest special cases to deal with are "sub-normal" numbers... numbers in which the biased exponent is 0, which indicates that the fraction should be preceded with "0." instead of "1."

   This gets even more complicated because it is possible for numbers which are normal when represented as a double can become sub-normal when represented as a floatx. There are even cases where numbers which *are* subnormal when represented as a double may *not be* subnormal when represented in floatx (for instance, when floatx has more than the 11 bits reserved for exponents in the double format.)
  
   Since sub-normal numbers are often complicated, no deduction will occur if you get sub-normal numbers incorrect. However, if you get them correct, you can earn some extra credit.

## Submitting the Project

When you have finished coding and testing your testFloatx program, make sure you do a final git commit and push to update your main git repository. Then, retrieve the 16 digit git hash code either from browsing your updated repository on the web, or using the command `git rev-parse HEAD` on the command line. Paste the git hash code in the Brightspace PROJ02 assignment. You may resubmit as many times as you would like to. Only the last submission will be graded.

## Grading Criteria

Your repository will be downloaded onto an LDAP machine at the level corresponding to the hash code retrieved from Brightspace, and compiled and run using the Makefile. Your code will be tested with the same inputs as the example above, and with three other sets of input values, and the results will be compared to correct results. The project is worth 100 points, and the following deductions will occur:

- -100 if your code matches or is significantly similar to any other student's code.
- -80 If your code does not attempt to address the problem.
- -80 if testFloatx does not compile
- Up to -30 if there are mismatches on the published test case
- Up to -15 points per test if there are incorrect results on each unpublished test case (for a maximum of -45)
- -10 if testFloatx produces warning  messages when compiled
- -10 for each 24 hours (or part of 24 hours) the submission is late After 5 days, there will be a -100 deduction
- -5 If your repository contains an executable file.

If the correct results are not produced for the published test case, the TA will try to fix your code. For each fix, there will be a deduction of -15. If the TA cannot fix your code, or if you code requires more than 4 fixes, you will get a -60 deduction.

There will be no deductions for mismatches where either the input is "subnormal" (has a biased exponent of 0), or the resulting floatx would be "subnormal" (have a biased exponent of 0). However, if you get all the subnormal test cases correct, you will get an extra 10 points added to your grade (up to but not exceeding 100).
