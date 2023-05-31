/* Utility to get/set bits and bit fields
	Conventions:
		vals are 64 bit integers (may be signed or unsigned)
		A field has a "starting" position, the leftmost, most significant bit, bs
		Bit positions are numbered RIGHT to LEFT, 0<=pos<64
		A field also has a length, len ... from the bs position to the right
		lengths must be at least 1, and fit in value size: 1<=len<64-bs
		setBit uses the C truth value of "new" to set the bit
		setBitFld will use bits (len-1) to 0 in the "new" parameter
		getBitFld will return a value with the sign bit of the sub-field propagated for the entire return value
		getBitFldU will return a value padded to the left with zeros
*/

int getBit(int bn,unsigned long val);
void setBit(int bn,int new,unsigned long *val);
long getBitFld(int bs,int len,unsigned long val);
unsigned long getBitFldU(int bs,int len,unsigned long val);
void setBitFld(int bs,int len,unsigned long new,unsigned long *val);