/*
*  Lab1.c
*  CSCI 247, Lab exercise 1, Summer, 2017
*
*  David Bover, June, 2017
*
*/
#include <stdio.h>
#include <string.h>
#define MAX_LINE    1024
// function length
// returns the length of a string
// parameter:
//      str, the string
// return:
//      integer value of the number of characters in the string
//      not counting the terminating null character
int length(char *str);
// function find()
// searches for a substring in a string
// parameters:
//      word: the substring to be found
//      line: the string to be searched for the substring
// return:
//      position in line where the substring starts, or
//      -1, if the substring was not found
int find(char *word, char *line);
// function replace
// replace part of a string by another string
// parameters:
//      line: the string to be modified
//      pos: the index in line where the modified string is to start
//      word: the substring to be used to mosidfy line
void replace(char *line, int pos, char *word);
int main (int argc, char *argv[]) {
// check for 3 command-line arguments
if (argc < 3) {
puts("Usage: ./Lab1 word1 word2\n");
return 1;
}
// check that word1 and word2 are of equal length
if (length(argv[1]) != length(argv[2])) {
puts("The two words must have the same length\n");
return 1;
}
// for each line of input, perform string replacement
char line[MAX_LINE];
while (fgets(line, MAX_LINE, stdin)) {
// remove the \n from the end of the line
line[length(line)-1] = 0;
// find word1 in line
int pos = find(argv[1], line);
// while word1 found in line, replace it
while (pos >= 0) {
replace(line, pos, argv[2]);
pos = find(argv[1], line);
}
puts(line);
}
}

int length(char *str) {
int length = 0;
// iterate through string while the pointer points
// to char element in array of chars
while (*str) {
    str++;
    length++;
}
    
return length;
}


int find(char *word, char *line) {
// first, assume that substring was not found; return -1 in such case
int pos = -1;
// iterate through the line until "i < length(line) - length(word)"
// in order to avoid out of bounds error in the inner loop
for (int i = 0; i < length(line) - length(word); i++){
// assume, the equality of substring and word was found; assign flag to 1
 int flag = 1;
// iterate through all chars in the word and check if
// every char in the word equals "i+j" char in the line
    for (int j = 0; word[j]; j++) {
// as soon as char in the line does not equal a char in the word
// assign flag to 0 (indicating that the word was not found) and
// end execution of the loop
        if (line[i+j] != word[j]) {
            flag = 0;
            break;
        }
    }
// if the substr was found, assign the position in the line to variable "pos"
    if (flag) {
        pos = i;
    }
}
return pos;
}

void replace(char *line, int pos, char *word) {
// k is an index of the char in the word
int k = 0;
// iterate through the line, starting from a certain position in the line
// and replace this substring by the word
for (int i = pos; i < pos + length(word); i++) {
    line[i] = word[k];
    k++;
}
}






