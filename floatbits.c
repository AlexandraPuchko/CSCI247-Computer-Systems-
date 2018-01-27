/*
*  floatbits.c
*  CSCI 247, Lab exercise 4, Fall, 2017
*
*  Alexandra Puchko, October, 2017
*
*/
#include<stdio.h>
/* access bit-level representation of a floating-point number */
typedef unsigned float_bits;

/*
float_class identifies the class of floating-point number represented by a float_bits parameter
  returns:
   - negative integer, if the floating-point number is a “special” (±∞, or NaN)
   - 0, if the floating-point number is denormalized
   - positive integer, if the floating-point number is normalized
*/
int float_class(float_bits f){
  //set default value to a number as a normalized
  int fb_class = 1;
  //calculate mask
  int expr0 = (0xff << 8) | 0xff; //0xffff
  int expr2 = (0x7f << 8) | 0xff; //0x7fff
  //mask to set MSB as a 0
  int mask1 = (expr2 << 16)| expr0; //0x7fffffff
  //set MSB to 0
  int expr = f & mask1;
  int f_exp = expr >> 23;
  if(!f_exp){
    fb_class = 0;
  } else if(!(f_exp ^ 0xff)){
    fb_class = -1;
  }

  return fb_class;
}

/*
 function which negates a floating-point number
  returns:
   - -f, if floating point number is not NaN
   - f, if floating point number is NaN
*/
float_bits float_negate(float_bits f){
  unsigned negate_fb = f;
  // check if NaN
  int expr0 = (0xff << 8) | 0xff; //0xffff
  int expr2 = (0x7f << 8) | 0xff; //0x7fff
  // set MSB to 0
  int mask1 = (expr2 << 16)| expr0; //0x7fffffff
  int expr = f & mask1;
  // get exp_part
  int f_exp = expr >> 23;
  // get frac_part
  int mask2 = (0x7f << 8) | 0xff;
  mask2 = (mask2 << 8) | 0xff; //0x7fffff
  int f_frac = f & mask2;
  // check if f is NaN (first check exponent and then fractional part)
  if(!(f_exp ^ 0xff) && !!(f_frac)){
    // f is NaN, return f
    negate_fb = f;
  } else{
     // change sign of MSB and remove back on its position
     int sign = ((f >> 31) ^ 0x1) << 31;
     // reassamble msb and exp and frac part
     negate_fb = sign | expr;
  }
  return negate_fb;
}


/*
  function which computer the absolute value of a floating-point number
  returns:
    - |f|, if f is not NaN
   -  f, if f is NaN
*/
float_bits float_absval(float_bits f){
  unsigned absval;
  // check for NaN
  int expr0 = (0xff << 8) | 0xff; //0xffff
  int expr2 = (0x7f << 8) | 0xff; //0x7fff
  //set MSB to 0
  int mask1 = (expr2 << 16)| expr0; //0x7fffffff
  int expr = f & mask1;
  //get exp_part
  int f_exp = expr >> 23;
  int mask2 = (0x7f << 8) | 0xff;
  mask2 = (mask2 << 8) | 0xff; //0x7fffff
  int f_frac = f & mask2;
  // check if f is NaN (first check exponent and then fractional part)
  if(!(f_exp ^ 0xff) && !!(f_frac)){
    // f is NaN
    absval = f;
  } else {
    //set MSB to 0 and return back to its position
    int sign = ((f >> 31) & 0x0) << 31;
    absval = sign | expr;
   }

    return absval;
}

  // /*
  // * function which doubles a floating-point number;
  // * Given a parameter f, this function will compute 2 ´ f.
  // * If f is Nan, the function should simply return f.
  // */
  float_bits float_twice(float_bits f){
    unsigned float_twice;
    // check for NaN
    int expr0 = (0xff << 8) | 0xff; //0xffff
    int expr2 = (0x7f << 8) | 0xff; //0x7fff
    //set MSB to 0
    int mask1 = (expr2 << 16)| expr0; //0x7fffffff
    int expr = f & mask1;
    //get exp_part
    int f_exp = expr >> 23;
    int mask2 = (0x7f << 8) | 0xff;
    mask2 = (mask2 << 8) | 0xff; //0x7fffff
    int f_frac = f & mask2;
    // check if f is NaN (first check exponent and then fractional part)
    if(!(f_exp ^ 0xff) && !!(f_frac)){
      // f is NaN
      float_twice = f;
    } else {
      // when we double floating point number, fractional part stays the same, by the exponent changes
      f_exp = f_exp + 1;
      int sign = f & (0x80 << 24);
     // reassamble sign, exponent and fractional parts of f
      float_twice = sign | (f_exp << 23) | f_frac;
    }

    return float_twice;
  }


  //
  // /*
  // * function which halves a floating-point number;
  // * Given a parameter f, this function will compute 0.5 ´ f.
  // * If f is Nan, the function should simply return f.
  // */
  float_bits float_half(float_bits f){
    unsigned float_half;
    // check for NaN
    int expr0 = (0xff << 8) | 0xff; //0xffff
    int expr2 = (0x7f << 8) | 0xff; //0x7fff
    //set MSB to 0
    int mask1 = (expr2 << 16)| expr0; //0x7fffffff
    int expr = f & mask1;
    //get exp_part
    int f_exp = expr >> 23;
    int mask2 = (0x7f << 8) | 0xff;
    mask2 = (mask2 << 8) | 0xff; //0x7fffff
    int f_frac = f & mask2;
    // check if f is NaN (first check exponent and then fractional part)
    if(!(f_exp ^ 0xff) && !!(f_frac)){
      // f is NaN
      float_half = f;
    } else {
    //  when we half floating point number,fractional part stays the same, but the exponent changes
      f_exp = f_exp - 1;
      int sign = f & (0x80 << 24);
      // reassamble sign,exp and frac parts of f
      float_half = sign | (f_exp << 23) | f_frac;
    }

    return float_half;
  }



  int main(){

  /*
  * params to test float_class(float_bits f)
  */
  unsigned int x1 = 0x5555ffff;
  //unsigned int x1 = 0x0000ffff;
  //unsigned int x1 = 0xffffffff;
  printf("float_class(x1) = %d\n",float_class(x1));

  /*
  * params to test float_negate(float_bits f)
  */
  unsigned int x2 = 0x40000000;
  printf("float_negate(x2) = %u\n",float_negate(x2));

  /*
  * params to test float_absval(float_bits f)
  */
  unsigned int x3 = 0xc5001040;
  printf("float_absval(x3) = %u\n",float_absval(x3));

  /*
  * params to test float_twice(float_bits f)
  */
  unsigned int x4 = 0x44000000;
  printf("float_twice(x4) = %u\n",float_twice(x4));

  /*
  * params to test float_half(float_bits f)
  */
  unsigned int x5 = 0x43560000;
  printf("float_half(x5) = %u\n",float_half(x5));



}
