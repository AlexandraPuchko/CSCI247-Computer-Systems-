/*
*  bits.c
*  CSCI 247, Lab exercise 3, Fall, 2017
*
*  Alexandra Puchko, October, 2017
*
*/
#include<stdio.h>

/*
* func1 - returns x & y
*/
int func1(int x, int y){
  /* exploit ability of ~ and | to compute x & y */
  int expr = ~(~x | ~y);
  return expr;
}

/*
* func2 returns x ^ y
*/
int func2(int x, int y){
    /* exploit ability of ~ and & to compute x & y */
    int expr1 = ~(x & ~y); //proof: De Morgan's law
    int expr2 = ~(~x & y);
    int expr3 = ~(expr1 & expr2);
    return expr3;
}

/*
*  func3 - returns 1 if any odd-numbered bit in an integer is set to 1
*  otherwise it returns 0 (The least significant bit is bit number 0)
*/
int func3(int x){
  int expr1 = 0xaa;//copy expr to get 0xaaaaaaaa
  int expr2 = (expr1 << 8) | expr1;//0xaaaa
  int mask = (expr2 << 16)| expr2;//0xaaaaaaaa
// extract all bits in odd-numbered positions
  int expr3 = x & mask;
  return !!(expr3);
}

/*
* func4 - returns 1 if all the odd-numbered bit in an integer are set to 1
* otherwise it returns 0
*/
int func4  (int x){
  int expr1 = 0xaa;//copy expr to get 0xaaaaaaaa
  int expr2 = (expr1 << 8) | expr1; //0xaaaa
  int expr3 = (expr2 << 8) | expr2; //0xaaaaaa
  int mask = (expr3 << 8) | expr3;  //0xaaaaaaaa
  // extract all bits in odd-numbered positions
  int expr4 = x & mask;
  // compare number (expr4) with mask. If it "equals" (we can not say it directly, since we are not allowed to use "=="") mask - return 1,
  // otherwise return 0
  int expr5 = expr4 ^ mask;
  return !expr5;
}



int main(){
  /*
  * params to test func1
  */
  int x1 = 5;
  int y1 = 6; // returns 4
  printf("x1 = %d, y1 = %d; func1(x1,y1) = %d\n", x1, y1, func1(x1,y1));

  /*
  * params to test func2
  */
  int x2 = 4; // returns 1
  int y2 = 5;
  printf("x2 = %d, y2 = %d; func2(x2,y2) = %d\n", x2, y2, func2(x2, y2));

  /*
  * params to test func3
  */
   int x3 = 0x5; // returns 0
  //int x3 = 0x7; // returns 1
  printf("x3 = %d; func3(x3) = %d\n", x3, func3(x3));

  /*
  * params to test func4
  */
  int x4 = 0xFFFFFFFD; //returns 0
 //int x4 = 0xAAAAAAAA; //returns 1
  printf("x4 = %d; func4(x4) = %d\n", x4, func4(x4));
}
