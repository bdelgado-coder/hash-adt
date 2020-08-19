//-----------------------------
// Brandon Delgado
// bdelgad3
// cse15
// December 4 2019
// DictionaryTest.c - pa5
// Test client for exercising functions from source file
// Compile:  gcc –std=c-99 –Wall –o DictionaryTest DictionaryTest.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Dictionary.h"
#define BREAKER_TEST_RANGE 20000

int main(){
   Dictionary D= newDictionary();
   if(size(D) == 0){
      printf("Dictionary has zero items\n");
   }

   insert(D, "chill", "icecream");

   if(size(D) == 0){
      printf("Dictionary has zero items\n");
   }
   else{
      printf("Dictionary has %d items\n", size(D));
   }

   insert(D, "poop", "donkey");
   if(size(D)!=0){
      printf("Dictionary has %d items\n", size(D));
   }

   printf("Number of chars in dictionary: %d\n", countChars(D));
   printf("%s\n", DictionaryToString(D));
   if( strcmp((lookup(D,"chill")) , "icecream") == 0 ){
      printf("This key exists\n");
   }

   delete(D, "poop");

   if(size(D)!=0){
      printf("Dictionary has %d items\n", size(D));
   }

   makeEmpty(D);
   printf("Make Empty\n");
   if(size(D) == 0){
      printf("Dictionary has zero items\n");
   }
   else{
      printf("Dictionary has %d items\n", size(D));
   }

   freeDictionary(&D);
   return EXIT_SUCCESS;
}