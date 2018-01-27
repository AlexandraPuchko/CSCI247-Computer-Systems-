/*
*  Lab2.c
*  CSCI 247, Fall, 2017
*
*  main() written by David Bover, July, 2017
*
*  students get to complete the functions toDecimal() and fromDecimal()
*
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define DECIMAL     10
#define MAX_BASE    36
#define WIDTH       20      // width of the string used to store the 




char digitChar(int value, int base) {
if (value < 0 || value >= base) {
printf("invalid digit value %d for base %d\n", value, base);
exit(1);
}
if (value >= 0 && value <= 9)
return '0' + value;
else
return 'A' + value - 10;
}

int digitValue(char digit, int base) {
int value = base;
if (digit >= '0' && digit <= '9')
value = digit - '0';
else if (digit >= 'a' && digit <= 'z')
value = digit - 'a' + 10;
else if (digit >= 'A' && digit <= 'Z')
value = digit - 'A' + 10;
if (value < base) return value;
printf("invalid digit %c for base %d\n", digit, base);
exit(1);
return value;
}


int toDecimal(int base, char *number) {
int decimal = 0;
int number_length = 0;
// count number of digits in number
while(*number){
    number++;
    number_length++;
}
// move pointer back to the 1st digit of the number
number-=number_length;
// count decimal value of number by iterating through the array, starting from the last digit;
// j - value of power
int j = 0;
for (int i = number_length - 1; i >= 0; i--) {
//  count a factor, first assuming that base in a power of 0 is 1
    int factor = 1;
    for (int k = 0; k < j; k++) {
      factor *= base; 
    }
// get decimal value
    decimal = decimal + digitValue(number[i], base)*factor;
    j++;
}

return decimal;
}


char *fromDecimal(int base, int decimal, char *number) {
// count number of all reminders, which will be calculated by dividing decimal by base;
// copy value of "decimal" to a new variable "dec", since we will need original value of variable "decimal"
// further
int dec = decimal;
int number_length = 0;
while(dec) {
  dec = dec/base;
  number_length++;
}
// add null character to a string
number[number_length] = '\0';
// convert digits to chars in reverse order
// push digits to the string
int i = number_length - 1;
while(decimal) {
// convert reminder (decimal%base) to a char
 number[i] = digitChar(decimal%base, base);
// update value of decimal
 decimal = decimal/base;
 i--;
}
return number;
}


int main(int count, char *word[]) {
if (count < 4) {
printf("Usage: ./Lab2 base num1 num2\n");
return 0;
}
// get the number base and check for range 2 to 36
int base = toDecimal(DECIMAL, word[1]);
if (base < 2 || base > MAX_BASE) {
printf("Usage: ./Lab2 base num1 num2\n");
printf("base must be in the range 2 to 36\n");
return 0;
}
// get the decimal value of the two numbers to be added
int num1 = toDecimal(base, word[2]);
int num2 = toDecimal(base, word[3]);
// calculate the sum of the two numbers
int result = num1 + num2;
// convert the decimal result back to the base of the numbers
char text[WIDTH];
char *answer = fromDecimal(base, result, text);
// display the result
printf("In base %d, %s + %s = %s\n", base, word[2], word[3], answer);
}







