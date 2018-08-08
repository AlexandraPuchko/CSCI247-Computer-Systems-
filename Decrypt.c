/*
 *  Assignment2.c
 *  CSCI 247, Fall, 2017
 *
 *  Alexandra Puchko, October, 2017
 *
 */
#include <stdio.h>
#include <string.h>
#define SEPTET_LENGTH 7
#define NIBBLE_LENGTH 4

/*
 In the program below the functions suggested by Dr. Bover are used.
 There is one little change: I changed function "decode_28bits" in such a way
 that I do not put key1 and key 2 into this function. All of the "xor" manipulations,
 shuffling and rotating are made in "main" function, so it is easy to follow the
 algorithm of decryption step by step strating from reading 7 chars from the file.
*/



// function get_keys
// extracts two 28-bit keys from 4bytes
void get_keys(char *twokeys, unsigned *key1, unsigned *key2);

// function shuffle_nibbles
// reverses the shuffle used by the encryption algorithm
void shuffle_nibbles(unsigned *bits);

// function get_n_bits
// returns the integer value of a specified subsequence of 32 bits
unsigned int  get_n_bits(unsigned bits, int width, int index);

// function rotate_left3
// given a sequence of 7 bits, rotates 3 bits to the left
unsigned int rotate_left3(unsigned bits);

// function decode_28bits
// decodes a block of 28 bits
// (where cipher is a sequence of 7 bits, rotates 3 bits to the left (from function rotate_left3)
void decode_28bits(unsigned cipher, char *plain);


unsigned int get_n_bits(unsigned bits, int width, int index){
   // a mask to extract 4 rightmost bits
   unsigned mask = 0xf;
   if(width == SEPTET_LENGTH){
   // a mask to extract 7 rightmost bits
     mask = 0x7f;
   }
  unsigned nbit = (bits >> (width*index) & mask);
  return nbit;
}


void get_keys(char *twokeys, unsigned *key1, unsigned *key2){
int key_length = strlen(twokeys);
// get key1
  for(int i = 0; i < key_length / 2; i++){
    // reduce msb from 1 char element
     char septet = twokeys[i] & 0x7f;
    // combine septets into 28 bit integer to get key1
     *key1 |= septet << 7*((key_length/2 - 1) - i);
   }

  // get key2
  int m = 0;
   for(int i = key_length / 2; i < key_length; i++){
     // reduce msb from 1 char element
     char septet = twokeys[i] & 0x7f;
     // combine septets into 28 bit integer to get key2
      *key2 |= septet << 7*((key_length/2 - 1) - m);
      m++;
   }
}



void shuffle_nibbles(unsigned *bits){
  unsigned nibbles = 0;
  unsigned n0, n1, n2, n3, n4, n5, n6; //nibbles
  // Shuffle nibbles algorithm (reversed)
  // n[0] -> n[6]
  // n[1] -> n[5]
  // n[2] ->n[4]
  // n[3] -> n[0]
  // n[4] -> n[2]
  // n[5] -> n[1]
  // n[6] -> n[3]

  // move nibbles to certain position according to the algorythm
  n0 = get_n_bits(*bits, NIBBLE_LENGTH, 0) << (NIBBLE_LENGTH * 6);
  n1 = get_n_bits(*bits, NIBBLE_LENGTH, 1) << (NIBBLE_LENGTH * 5);
  n2 = get_n_bits(*bits, NIBBLE_LENGTH, 2) << (NIBBLE_LENGTH * 4);
  n3 = get_n_bits(*bits, NIBBLE_LENGTH, 3);
  n4 = get_n_bits(*bits, NIBBLE_LENGTH, 4) << (NIBBLE_LENGTH * 2);
  n5 = get_n_bits(*bits, NIBBLE_LENGTH, 5) << NIBBLE_LENGTH;
  n6 = get_n_bits(*bits, NIBBLE_LENGTH, 6) << (NIBBLE_LENGTH * 3);
  nibbles |= n0 | n1 | n2 | n3 | n4 | n5 | n6;
  *bits = nibbles;
}


unsigned int rotate_left3(unsigned bits){
  unsigned rotated = 0;
  for(int i  = 0; i < 4; i++){
   unsigned septet = get_n_bits(bits, SEPTET_LENGTH,i);
   //get left half of the septet and move to the right
   unsigned left_half = ((septet & 0xf0) >> 4);
   //get right half of the septet
   unsigned right_half = septet & 0x0f;
   // swipe to halves of the septet
   unsigned rotated_septet = (left_half | (right_half << 3));
   // combine septet to the whole sequence of bits
   rotated |= rotated_septet << i*SEPTET_LENGTH;
  }
 return rotated;
}


void decode_28bits(unsigned cipher, char *plain){
  // convert 4 septets to letters
  for (int i = 3; i >= 0; i--){
    unsigned letter = get_n_bits(cipher, SEPTET_LENGTH, i);
    plain[3-i] = letter;
  }
  // add '\0' for string
  plain[5] = '\0';
}



int main(int argc, char *argv[]){
char line[8];
unsigned int bits;
unsigned key1 = 0;
unsigned key2 = 0;
//get key1 and key2
get_keys(argv[1], &key1, &key2);

// //Get the next 7 characters of input (leaving space for the null at the end)
while(fgets(line, 8, stdin)){
  //get rid of null terminating pointer
  line[7] = 0;
  //convert to a number
  sscanf(line, "%x", &bits);
  // xor with key2
  bits = bits ^ key2;
  // shuffle nibbles
  shuffle_nibbles(&bits);
  //xor with key1
  bits = bits ^ key1;
  //rotate septets to the left
  unsigned rotated = rotate_left3(bits);
  // create a piece of plain text to store the result of decode
  char plain[5];
  // decode rotated bits to the original text
  decode_28bits(rotated, plain);
  printf("%s", plain);
 }
}
