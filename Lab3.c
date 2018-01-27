/*
*  bits.c
*  CSCI 247, Lab exercise 3, Fall, 2017
*
*  Alexandra Puchko, October, 2017
*
*/

#include<stdio.h>


//compute the bit-wise AND of two integers, using only the ~ and |
int func1(int x, int y);
//compute the bit-wise exclusive-or of two integers, using only the ~ and &
int func2(int x, int y);

//function to return 1 if any odd-numbered bit in an integer is set to 1,
//        and return 0 (The least significant bit is bit number 0).
int func3(int x);

//function to return 1 if all the odd-numbered bits in an integer are set to 1, and return 0
int func4(int x);

// function to return a bit-mask with all 1's between lowbit and highbit, inclusive.
// Assume 0 <= lowbit <= 31, and 0 <= highbit <= 31.
// If lowbit > highbit, then mask should be all 0's.
int func5(int highbit, int lowbit);

// a function to swap the mthbyte and the nth byte of an integer.
// You may assume that 0 <= n <= 3, 0 <= m <= 3.
int func6(int x, int m, int n);


int main(int argc, char *argv[]){





}
