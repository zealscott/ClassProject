/*
 * CS:APP Data Lab
 *
 * <Please put your name and userid here>
 *
 * bits.c - Source file with your solutions to the Lab.
 *          This is the file you will hand in to your instructor.
 *
 * WARNING: Do not include the <stdio.h> header; it confuses the dlc
 * compiler. You can still use printf for debugging without including
 * <stdio.h>, although you might get a compiler warning. In general,
 * it's not good practice to ignore compiler warnings, but in this
 * case it's OK.
 */

#if 0
/*
 * Instructions to Students:
 *
 * STEP 1: Read the following instructions carefully.
 */

You will provide your solution to the Data Lab by
editing the collection of functions in this source file.

INTEGER CODING RULES:

  Replace the "return" statement in each function with one
  or more lines of C code that implements the function. Your code
  must conform to the following style:

  int Funct(arg1, arg2, ...) {
      /* brief description of how your implementation works */
      int var1 = Expr1;
      ...
      int varM = ExprM;

      varJ = ExprJ;
      ...
      varN = ExprN;
      return ExprR;
  }

  Each "Expr" is an expression using ONLY the following:
  1. Integer constants 0 through 255 (0xFF), inclusive. You are
      not allowed to use big constants such as 0xffffffff.
  2. Function arguments and local variables (no global variables).
  3. Unary integer operations ! ~
  4. Binary integer operations & ^ | + << >>

  Some of the problems restrict the set of allowed operators even further.
  Each "Expr" may consist of multiple operators. You are not restricted to
  one operator per line.

  You are expressly forbidden to:
  1. Use any control constructs such as if, do, while, for, switch, etc.
  2. Define or use any macros.
  3. Define any additional functions in this file.
  4. Call any functions.
  5. Use any other operations, such as &&, ||, -, or ?:
  6. Use any form of casting.
  7. Use any data type other than int.  This implies that you
     cannot use arrays, structs, or unions.


  You may assume that your machine:
  1. Uses 2s complement, 32-bit representations of integers.
  2. Performs right shifts arithmetically.
  3. Has unpredictable behavior when shifting an integer by more
     than the word size.

EXAMPLES OF ACCEPTABLE CODING STYLE:
  /*
   * pow2plus1 - returns 2^x + 1, where 0 <= x <= 31
   */
  int pow2plus1(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     return (1 << x) + 1;
  }

  /*
   * pow2plus4 - returns 2^x + 4, where 0 <= x <= 31
   */
  int pow2plus4(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     int result = (1 << x);
     result += 4;
     return result;
  }

FLOATING POINT CODING RULES

For the problems that require you to implent floating-point operations,
the coding rules are less strict.  You are allowed to use looping and
conditional control.  You are allowed to use both ints and unsigneds.
You can use arbitrary integer and unsigned constants.

You are expressly forbidden to:
  1. Define or use any macros.
  2. Define any additional functions in this file.
  3. Call any functions.
  4. Use any form of casting.
  5. Use any data type other than int or unsigned.  This means that you
     cannot use arrays, structs, or unions.
  6. Use any floating point data types, operations, or constants.


NOTES:
  1. Use the dlc (data lab checker) compiler (described in the handout) to
     check the legality of your solutions.
  2. Each function has a maximum number of operators (! ~ & ^ | + << >>)
     that you are allowed to use for your implementation of the function.
     The max operator count is checked by dlc. Note that '=' is not
     counted; you may use as many of these as you want without penalty.
  3. Use the btest test harness to check your functions for correctness.
  4. Use the BDD checker to formally verify your functions
  5. The maximum number of ops for each function is given in the
     header comment for each function. If there are any inconsistencies
     between the maximum ops in the writeup and in this file, consider
     this file the authoritative source.

/*
 * STEP 2: Modify the following functions according the coding rules.
 *
 *   IMPORTANT. TO AVOID GRADING SURPRISES:
 *   1. Use the dlc compiler to check that your solutions conform
 *      to the coding rules.
 *   2. Use the BDD checker to formally verify that your solutions produce
 *      the correct answers.
 */


#endif
/*
 * bitAnd - x&y using only ~ and |
 *   Example: bitAnd(6, 5) = 4
 *   Legal ops: ~ |
 *   Max ops: 8
 *   Rating: 1
 */
int bitAnd(int x, int y) {
/*  using arithmetic operation */
  return ~(~x|~y);
}
/*
 * getByte - Extract byte n from word x
 *   Bytes numbered from 0 (LSB) to 3 (MSB)
 *   Examples: getByte(0x12345678,1) = 0x56
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 6
 *   Rating: 2
 */
int getByte(int x, int n){
/* using n<<3 to express n*8, show the shift amount
 * and using & 0xff to get rid of significant numbers    */
  return x>>(n<<3)&0xff;
}
/*
 * logicalShift - shift x to the right by n, using a logical shift
 *   Can assume that 0 <= n <= 31
 *   Examples: logicalShift(0x87654321,4) = 0x08765432
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 20
 *   Rating: 3
 */
int logicalShift(int x, int n) {
/* 1<<31>>n is to point to the most significant position
    below this position is all 0
    above this position is all 1
    ~ is to reverse             */
  return x>>n&~(1<<31>>n<<1);
}
/*
 * bitCount - returns count of number of 1's in word
 *   Examples: bitCount(5) = 2, bitCount(7) = 3
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 40
 *   Rating: 4
 */
int bitCount(int x) {
/* using temp to calculate how many 1's every 8 bits
   every byte of n stores how many 1's in this byte
   because at most there are 32 1's,so we use 1 byte to store
   using >>16 >>8 to tranfor the significant bytes to lower bytes */
  int tmp=((0x01<<8|0x01)<<8|0x01)<<8|0x01;
  int n=x&tmp;
  n+=tmp&(x>>1);
  n+=tmp&(x>>2);
  n+=tmp&(x>>3);
  n+=tmp&(x>>4);
  n+=tmp&(x>>5);
  n+=tmp&(x>>6);
  n+=tmp&(x>>7);
  n=n+(n>>16);
  n=n+(n>>8);
  return n&0xff;
}
/*
 * bang - Compute !x without using !
 *   Examples: bang(3) = 0, bang(0) = 1
 *   Legal ops: ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 4
 */
int bang(int x) {
/* if x!=0,x+(-x+1)=2^32, and the most significant bit must be 1.
        */
  return (~((x|(~x+1))>>31))&0x1;
}
/*
 * tmin - return minimum two's complement integer
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 4
 *   Rating: 1
 */
int tmin(void) {
/*the minimum integer is all 1111 */
  return 1<<31;
}
/*
 * fitsBits - return 1 if x can be represented as an
 *  n-bit, two's complement integer.
 *   1 <= n <= 32
 *   Examples: fitsBits(5,3) = 0, fitsBits(-4,3) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 15
 *   Rating: 2
 */
int fitsBits(int x, int n) {
/* if x<0, ~x need to be in range 0-2^n-2,so shift n-1, the
   result should be 0x000000   n+~0=n-1*/
  int negative=x>>31;
  return (negative&!((~x)>>(n+~0)))|(!negative&!(x>>(n+~0)));
}
/*
 * divpwr2 - Compute x/(2^n), for 0 <= n <= 30
 *  Round toward zero
 *   Examples: divpwr2(15,1) = 7, divpwr2(-33,4) = -2
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 15
 *   Rating: 2
 */
int divpwr2(int x, int n) {
/*to let negative number round to zero, need to create 0x1111(n)
  to let negative number +1*/
    int sign=x>>31&((1<<n)+(~0x0));
    return (sign+x)>>n;
}
/*
 * negate - return -x
 *   Example: negate(1) = -1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 5
 *   Rating: 2
 */
int negate(int x) {
/* according to the two complement priciple*/
  return ~x+1;
}
/*
 * isPositive - return 1 if x > 0, return 0 otherwise
 *   Example: isPositive(-1) = 0.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 8
 *   Rating: 3
 */
int isPositive(int x) {
/*x>>31 can determine whether it is a negative. and !x can determine
 * whether it is 0x0   */
  return !(x>>31|!x);
}
/*
 * isLessOrEqual - if x <= y  then return 1, else return 0
 *   Example: isLessOrEqual(4,5) = 1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 24
 *   Rating: 3
 */
int isLessOrEqual(int x, int y) {
/*xSign and ySign express whether it is positive, if so,return 1,else
 *  return 0, xSign^ySign judge whether they have the same sign,
 *  if so, compare x+(~y),else judge the sign of x */
  int xSign=!(x>>31);
  int ySign=!(y>>31);
  return ((!(xSign^ySign))&((x+(~y))>>31))|((xSign^ySign)&~xSign);
}
/*
 * ilog2 - return floor(log base 2 of x), where x > 0
 *   Example: ilog2(16) = 4
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 90
 *   Rating: 4
 */
int ilog2(int x) {
/*      */
  int byte4=x>>24;
  int byte3=x>>16&0xff;
  int byte2=x>>8&0xff;
  int exit4=!!(byte4);/*if exit in this byte,return 1*/
  int exit3=(!!(byte3))|exit4;
  int exit2=(!!(byte2))|exit3;
  int significantByte=(exit4+exit3+exit2)<<3;/*multiple 8,get the highest byte*/
  int byte=x>>significantByte&0xff; /*get the highest byte */
  int bit8=!!(byte>>7);  /*get the highest bit*/
  int bit7=!!(byte>>6);
  int bit6=!!(byte>>5);
  int bit5=!!(byte>>4);
  int bit4=!!(byte>>3);
  int bit3=!!(byte>>2);
  int bit2=!!(byte>>1);
  int significantBit=bit8+bit7+bit6+bit5+bit4+bit3+bit2; /*get the highest bit*/
  return significantBit+significantByte;
}
/*
 * float_neg - Return bit-level equivalent of expression -f for
 *   floating point argument f.
 *   Both the argument and result are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representations of
 *   single-precision floating point values.
 *   When argument is NaN, return argument.
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 10
 *   Rating: 2
 */
unsigned float_neg(unsigned uf) {
/* compare the frac and exp */
 int temp=1<<23;
 if  (((uf>>23&0xff)==0xff)&&(uf&(temp-1)))
     return uf;
 return uf^(1<<31);
}
/*
 * float_i2f - Return bit-level equivalent of expression (float) x
 *   Result is returned as unsigned int, but
 *   it is to be interpreted as the bit-level representation of a
 *   single-precision floating point values.
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 30
 *   Rating: 4
 */
unsigned float_i2f(int x) {
/* using lift shift to store exp,frac, remember to rounding */
    unsigned sign=(x>>31&1);
    unsigned frac=x;
    unsigned exp,c=0,flag=0;
    if (x==0) return 0;
    if (sign)
    {
        frac=~frac+1;
    }
    while (!(frac&(1<<31)))
    {
        frac<<=1;
        c++;
    }
    exp=31-c+0x7f;
    frac<<=1;
    if (frac>>8 & 0x1)  
    {
	    if((frac & 0x1ff) !=0x100)
		    flag=1;
	    else
	    {
		    if (frac>>9 &0x1)
			    flag=1;
	    }
    }
    frac>>=9;
    return (sign<<31)+(exp<<23)+(frac&0x7fffff)+flag;
}
/*
 * float_twice - Return bit-level equivalent of expression 2*f for
 *   floating point argument f.
 *   Both the argument and result are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representation of
 *   single-precision floating point values.
 *   When argument is NaN, return argument
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 30
 *   Rating: 4
 */
unsigned float_twice(unsigned uf) {
/* if its infinity or NaN,returen argument
   else */
    unsigned sign=uf>>31;
    unsigned exp=(uf>>23)&0xff;
    unsigned frac=uf&0x7fffff;
    if(exp==0xff && frac!=0)
        return uf;   /* NaN  */
    if(exp==0xff && frac==0)
        return uf;   /* infinite */
    if(exp==0)
    {
	
        if (frac>>22)   //if frac>=0.5 
        {
            exp=1;     //return to normalized 
        }       
	frac=(frac<<1);
    }
    else
    {
        exp+=1;
        if(exp==0xff)
            frac=0;
    }
    return (sign<<31)|(exp<<23)|frac;
}
