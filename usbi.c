/*
 * usbi.c
 *
 * package for unsigned big integer package
 *
 * used for CSCI 247, Assignment 3, Fall 2017
 *
 * Alexandra Puchko , WWU, November 2017
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include "usbi.h"

#define BASE    100                 // Each byte of the USBI contains 0-99
#define DIGITS  10                  // Each byte represented by two decimal digits
#define SMALL(N)  (N % DIGITS)      // The ones digit of a byte
#define LARGE(N)  (N / DIGITS)      // The tens digit of a byte
#define SIZE(V) (V[0])              // Number of bytes used by a USBI for its value
#define MSB(N)  (N / BASE)          // The most significant byte from a partial calculation
#define LSB(N)  (N % BASE)          // The least significant byte from a partial calculation


/*
 * function dumpUsbi
 * writes to stdout to display the numeric value and bytes values of a usbi
 */
void dumpUsbi(usbi x) {
    // display numeric value of the usbi
    printf("usbi dump of %s: ", usbiDecimalString(x));

    // display the value of each byte of the usbi
    for (int i = 0; i <= SIZE(x); i++)
        printf("%02d%c ", x[i], (i < SIZE(x) ? ',' : ' '));
    printf("\n");
}


/*
 * function newUsbi
 * create a usbi data item from an unsigned long int
 */
usbi newUsbi (unsigned long int value) {

    // determine how many bytes needed
    unsigned char count = 1;
    if (value > 0) {
        unsigned long int copy = value;
        count = 0;
        while (copy > 0) {
            count++;
            copy /= BASE;
        }
    }

    // create the usbi object of correct length
    usbi result = (usbi)malloc(count+1);
    if (!result) {
        printf("malloc() failed on call for %d bytes\n", count+1);
        exit(1);
    }

    // set the size byte of the usbi
    result[0] = count;

    // load the number into the other bytes of the usbi
    for (int i = 1; i <= count; i++) {
        result[i] = value % BASE;
        value /= BASE;
    }

    return result;
}

/*
 * function zeroUsbi
 * create a usbi data object with zero value, containing the specified number of bytes,
 * plus the first byte for the count
 */
usbi zeroUsbi (unsigned int bytes){

    // if bytes is 0, make it 1
    if (bytes == 0)
        bytes = 1;

    // the count byte cannot contain a value greater than 255
    // this is the limit on the size of any usbi
    if (bytes > 255) {
        printf("cannot create a usbi with more than 255 bytes\n");
        exit(1);
    }

    // allocate memory for the usbi
    usbi result = (usbi)malloc(bytes+1);
    if (!result) {
        printf("malloc() failed on call for %d bytes\n", bytes+1);
        exit(1);
    }

    // set the count byte
    result[0] = bytes;

    // set all the value bytes to 0
    for (int i = 1; i <= bytes; i++)
        result[i] = 0;

    return result;
}

/*
 * function usbiDecimalString
 * returns a character string of the decimal digits for display of the numeric value of
 * a usbi
 */
char *usbiDecimalString(usbi val) {

    // determine the length of the string that will be created
    // get 2 digits from each value byte,
    // maybe only 1 digit for the most significant byte
    // add one for the null character at the end
    int size = SIZE(val) * 2;

    // check most significant byte, if > 9 need an extra character
    if (val[SIZE(val)] > 9) size ++;

    // allocate the memory for the string
    char *result = (char*)malloc(size);

    // pos is the position in the result string
    int pos = 0;

    // a possible first digit from the most significant byte
    if (val[SIZE(val)] > 9)
        result[pos++] = LARGE(val[SIZE(val)]) + '0';

    // the second (rightmost) digit from the most significant byte
    result[pos++] = SMALL(val[SIZE(val)]) + '0';

    // now, all the digits from the other bytes
    for (int i = SIZE(val) - 1; i > 0; i--) {
        result[pos++] = LARGE(val[i]) + '0';
        result[pos++] = SMALL(val[i]) + '0';
    }

    // null character at the end of the string
    result[size-1] = 0;
    return result;

}

/*
 * function trimUsb
 * trims surplus bytes from the most significant end of a usbi value
 * Keeps usbi representation as compact as possible
 */
usbi trimUsbi (usbi x) {

    // if the most significant byte is not 0, no trimming is needed
    if (x[SIZE(x)] > 0) return x;

    // count the number of leading zero bytes at the most significant end
    int size = SIZE(x);
    while (x[size] == 0) size--;

    // create a new usbi of the right size
    usbi result = zeroUsbi(size);

    // copy byte values across to the new usbi
    result[0] = size;
    for (int i = 1; i <= size; i++)
        result[i] = x[i];

    // release the memory used by the old usbi
    free(x);

    return result;
}

/*
 * function addUsbi
 * adds two usbi values and returns the usbi value of the sum of the arguments
 */
usbi addUsbi (usbi a, usbi b) {

    // 1) get first byte of a number and compare that. Therefore, the size of usbi will be the size
    // of the number which is bigger
    int usbi_size = (SIZE(a) - SIZE(b) < 0) ? SIZE(b) : SIZE(a);

    // 2) create result usbi
    usbi result = zeroUsbi(usbi_size);
    int carry = 0;
    // 3) calculate byte of new usbi
    for (int i = 1; i <= usbi_size; i++){
      // 4) add bytes from arrays, add carry
      int new_byte = a[i] + b[i] + carry;
      // 5) uodate carry and new_byte
      if(new_byte >= BASE){
        carry = 1;
        new_byte %= BASE;
      }
     else{
        carry = 0;
      }
      //6) put byte to new usbi
      result[i] = new_byte;
   }

   return result;
}

/*
 * function multiplyUsbi
 * multiplies two usbi values and returns the usbi value of the product of the two arguments
 */
usbi multiplyUsbi (usbi a, usbi b) {

    // 1) get the result of the result of multiplication
    int usbi_size = SIZE(a) + SIZE(b);
    usbi result = zeroUsbi(usbi_size);
    int carry = 0;

    // 2) calculate each byte in the final result
    for(int i = 1; i < usbi_size; i++){
      int byte = 0;

      // 3) count boundaries of the loop (obtained from the Math formula)
      int max = 1;
      int min = i;
      int max1 =  i  -  SIZE(b) - 1;
      int min1 = SIZE(a) + 1;
      if(max1 > max)
        max = max1;
      if(min1 < min)
        min = min1;
      // count boundaries of the loop (are obtained from the Math formula) ----- end--->

      // 4) calculated sum according to the boundaries of sum iterator
      for(int j = max; j <= min; j++){
        // 1st factor
        int factor_1 = a[j];
        // 2nd factor
        int factor_2 = b[i - j + 1];
        // check if factors exist in arrays, otherwise assign 0
        if(j > SIZE(a))
          factor_1 = 0;
        if(i - j + 1 > SIZE(b))
          factor_2 = 0;

        byte += factor_1 * factor_2;
      }
      // 5) add carry from previous operation to sum
      byte += carry;
      // 6) calculate new carry for next iteration
      carry = byte / BASE;
      // 7) calculate value of byte and put to new usbi
      result[i] = byte % BASE;
    }

    // 8) if there is a carry after all iterations and it does not equal 0, add as MSB
    if(carry != 0)
      result[usbi_size] = carry;

    // 9) get rid of unnecessary 0s and return usbi
    return trimUsbi(result);
}

/*
 * function factorial
 * calculates n!, for argument n, and returns the result as a usbi
 */
usbi factorial(int n) {

    // create a usbi for the result, initially 1
    usbi result = newUsbi(1);
    usbi newresult;

    while (n > 1) {

        // create a usbi for n
        usbi factor = newUsbi(n);

        // get a new result as the old result multiplied by n
        newresult = multiplyUsbi(result, factor);
        // don't need the usbi factor, result, so release their memory
        free(factor);
        free(result);

        // update result with newresult
        result = newresult;

        // decrement n
        n--;
    }

    return result;
}
