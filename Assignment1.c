/*
*  Assignment1.c
*  CSCI 247, Fall, 2017
*
*  Alexandra Puchko, October, 2017
*
*/

#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX_WORD_LEN 40
#define MAX_WORDS 110000
#define MAX_WORD_TO_SUGGEST 5
#define FILE_NAME "wordsEn.txt"
#define ALPHA_GAP 32
#define SPACE_VAL 32

// function loadWordlist
// loads wordlist from the file to the array
// parameter:
//      input, the file to be read from
//      wordlist, an array of chars to be filled in
//      MAX_WORDS, a max amount of words the txt may have
//      MAX_WORD_LEN, a max length of each word in the txt file
void loadWordlist(FILE *input, char wordlist[MAX_WORDS][MAX_WORD_LEN]){
  int i = 0;
    while (fgets(wordlist[i], MAX_WORD_LEN, input)){
      for(int j = 0; j < strlen(wordlist[i]); j++){
     // Remove any special characters at the end of the word that are less than ’ ’
        if(wordlist[i][j] < SPACE_VAL)
           wordlist[i][j] = 0;
       }
      i++;
    }
}

// function toLowerCase
// converts chars in a string to lowercase
// parameter:
//      str, a string; each element of a string (represented by char) should be converted to lowercase
void toLowerCase(char *str) {
  while(*str){
    // If char is in a given range, convert it to lower case by adding ALPHA_GAP
    if(*str >= 'A' && *str<='Z'){
      *str+=ALPHA_GAP;
    }
    str++;
   }
}

// function countHammingDistance
// counts Hamming distance between two words
// parameter:
//      word1, a string
//      word2, a string
int countHammingDistance(char *word1, char *word2){
  int hammingDistance = 0;
  // Iterate through the each char of the word1 and compare it with the char of the word2 (with the same index).
  // If the chars differ from each other, add 1 to hamming distance counter
  for(int i = 0; i < strlen(word1); i++){
    if(word1[i] != word2[i]){
        hammingDistance+=1;;
    }
  }
  return hammingDistance;
}



int main(int argc, char *argv[])
{
  FILE *input = fopen(argv[1], "r");
  char wordlist[MAX_WORDS][MAX_WORD_LEN];
  // Check whether the name of the file containing the word list is provided on the command line.
  // If not - terminate the program
  if(argc > 1 && !strcmp(argv[1], FILE_NAME)){
      loadWordlist(input, wordlist);
      puts("Words are successfully loaded to the dictionary");
  } else{
      puts("The name of the file containing the word list is not provided on the command line\n");
      return 1;
  }

  char word[MAX_WORD_LEN];
  // while user prompts words
  while(fgets(word, MAX_WORD_LEN, stdin)){
    // Delete null terminating pointer from the word from the keyboard
    word[strlen(word)-1] = 0;
    // Check the word length; if it equals 0, exit the program
    if(strlen(word) > 0){
      // Declare an array to contain the final result of the user's prompt
      toLowerCase(word);
      // First assume that minHammingDistance is MAX_WORD_LEN
      int minHammingDistance = MAX_WORD_LEN;
      // Flag if the word was found
      int wasFound = 0;
      int i = 0;
      // Iterate through the list of all words
      while(wordlist[i][0]){
        // If length is the same, count Hamming Distance
        if(strlen(word) == strlen(wordlist[i])){
          int hammingDistance = countHammingDistance(word, wordlist[i]);
          // If (hammingDistance == 0) => the word is in the list
            if(!hammingDistance){
              printf("The word %s was found!\n", word);
              //set wasFound to 1
              wasFound = 1;
              break;
            }
            // Otherwise check if hammingDistance is less than minHammingDistance.
            // If yes, update value of minHammingDistance
            else{
              if(hammingDistance < minHammingDistance){
                 minHammingDistance = hammingDistance;
               }
            }
        }
       i++;
      }

      // Run this piece of code if the word was not found
      if(!wasFound){
        // minWordsSize will contain size of all words with equal minHammingDistance
        int minWordsSize = 0;
        int j = 0;
        int i = 0;
        char wordsToSort[MAX_WORD_TO_SUGGEST][MAX_WORD_LEN];
        // Iterate through the list of all words
        // and get the first 5 ( when minWordsSize <=5) to sort and display
        while(wordlist[i][0] && minWordsSize <=5){
          if(strlen(word) == strlen(wordlist[i])){
            if(countHammingDistance(word, wordlist[i]) == minHammingDistance){
              // Copies the word with equal minHammingDistance to the "wordsToSort" array to sort
              strcpy(wordsToSort[j],wordlist[i]);
              minWordsSize++;
              j++;
            }
          }
          i++;
        }

        // alphabetical sort
        char *temp;
        for(int i = 0; i < minWordsSize - 1; i++){
          for(int j = i + 1 ; j < minWordsSize; j++){
             if(strcmp(wordsToSort[i],wordsToSort[j])>0){
                char *ptr1 = wordsToSort[i];
                char *ptr2 = wordsToSort[j];
                temp = ptr1;
                ptr1 = ptr2;
                ptr2 = temp;
             }
          }
        }
          //  alphabetical sort ------- end

          printf("The word %s was not found. Try this: ", word);
          // Check if the number of all words founded is less than MAX_WORD_TO_SUGGEST)
          if(minWordsSize > MAX_WORD_TO_SUGGEST){
            minWordsSize = MAX_WORD_TO_SUGGEST;
          }

          // Display the result
          for(int i = 0; i < minWordsSize; i++){
            printf("%s ", wordsToSort[i]);
          }
          printf("\n");
       }
      } else{
            return 1;
          }
    }


    return 0;
}
