//-----------------------------
// Brandon Delgado
// bdelgad3
// cse15
// December 4 2019
// Dictionary.c - pa5
// Linked-list solution for a hash table based Dictionary
// Compile:  gcc –std=c-99 –Wall –o Dictionary Dictionary.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "Dictionary.h"

// Private Types and Functions ------------------------------------------------

const int tableSize = 101; // or some prime other than 101

// hash() prototype
int hash(char *key);

// NodeObj
typedef struct NodeObj
{
   char *key;   // unique identifier for the pair
   char *value; // contents which are paired with a unique identifier
   struct NodeObj *next;
} NodeObj;

// Node
typedef NodeObj *Node;

// newNode()
// Constructor for private Node type.
Node newNode(char *k, char *v)
{
   Node N = malloc(sizeof(NodeObj));
   assert(N != NULL);
   N->next = NULL;
   N->key = k;
   N->value = v;
   return N;
}

// freeNode()
// Destructor for private Node type
void freeNode(Node *pN)
{
   if (pN != NULL && *pN != NULL)
   {
      free(*pN);
      *pN = NULL;
   }
}

// DictionaryObj
typedef struct DictionaryObj
{
   Node *table;  // array of Nodes used to store the keys and value
   int numItems; // keeping count of items
} DictionaryObj;

// countChars()
// Returns the number of characters in a text representation of D. Used by
// DictionaryToString() to allocate sufficient heap memory.  Does not count
// the terminating null '\0' character.
int countChars(Dictionary D)
{
   int kcount = 0;
   int count = 0;
   Node N;
   for (int i = 0; i < tableSize; i++)
   {
      N = D->table[i];
      while (N != NULL)
      {
         kcount += strlen(N->key) + 1 + strlen(N->value) + 2; // spaces in between
         N = N->next;
      }
   }
   count = kcount;
   return count;
}

// Constructors-Destructors ---------------------------------------------------

// newDictionary()
// Constructor for the Dictionary type.
Dictionary newDictionary()
{
   Dictionary D = malloc(sizeof(DictionaryObj));
   assert(D != NULL);
   D->numItems = 0;
   D->table = calloc(tableSize, sizeof(Node));
   assert(D->table != NULL);
   return D;
}

// freeDictionary()
// Destructor for the Dictionary type.
void freeDictionary(Dictionary *pD)
{
   if (pD != NULL && *pD != NULL)
   {
      makeEmpty(*pD);
      free((*pD)->table);
      free(*pD);
      *pD = NULL;
   }
}

// ADT operations -------------------------------------------------------------

// findKey()
// Returns a reference to the Node with the same key, else returns NULL
Node findKey(Dictionary D, char *k)
{
   Node N = D->table[hash(k)];
   if (N == NULL)
   {
      return NULL;
   }
   else
   {
      while (N != NULL)
      {
         if (strcmp((N->key), k) == 0)
         {
            return N;
         }
         N = N->next;
      }
   }
   return NULL;
}

// size()
// Return the number of (key, value) pairs in Dictionary D.
int size(Dictionary D)
{
   if (D == NULL)
   {
      fprintf(stderr,
              "Dictionary Error: calling size() on NULL Dictionary reference\n");
      exit(EXIT_FAILURE);
   }
   return D->numItems;
}

// lookup()
// If D contains a pair whose key matches argument k, then return the
// corresponding value, otherwise return NULL.
char *lookup(Dictionary D, char *k)
{
   Node N;
   int index = hash(k);
   if (D == NULL)
   {
      fprintf(stderr,
              "Dictionary Error: calling lookup() on NULL Dictionary reference\n");
      exit(EXIT_FAILURE);
   }
   N = findKey(D, k);
   if (N == NULL)
   {
      return NULL;
   }
   return N->value;
}

// insert()
// Insert the pair (k,v) into D.
// Pre: lookup(D, k)==NULL (D does not contain a pair whose first member is k.)
void insert(Dictionary D, char *k, char *v)
{
   int index = hash(k);
   if (D == NULL)
   {
      fprintf(stderr, "Dictionary Error: calling insert() on NULL Dictionary reference");
      exit(EXIT_FAILURE);
   }
   if (lookup(D, k) != NULL)
   {
      fprintf(stderr, "Dictionary Error: inserting key that already exists\n");
      exit(EXIT_FAILURE);
   }

   Node N = newNode(k, v);

   if (D->table[index] != NULL)
   {
      N->next = D->table[index];
      D->table[index] = N;
   }
   else
      D->table[index] = N;
   D->numItems++;
}

// delete()
// Remove pair whose first member is the argument k from D.
// Pre: lookup(D,k)!=NULL (D contains a pair whose first member is k.)
void delete (Dictionary D, char *k)
{
   Node N, P;
   if (D == NULL)
   {
      fprintf(stderr,
              "Dictionary Error: calling delete() on NULL Dictionary reference\n");
      exit(EXIT_FAILURE);
   }

   N = D->table[hash(k)];
   if (strcmp(N->key, k) == 0)
   {
      P = D->table[hash(k)]; // P points to the head of the linked list
      D->table[hash(k)] = D->table[hash(k)]->next;
      freeNode(&P);
   }
   else
   {
      while (N != NULL && N->next != NULL)
      {
         if (strcmp(N->next->key, k) == 0)
         {
            P = N->next;
            N->next = N->next->next;
            freeNode(&P);
         }
         N = N->next;
      }
   }
   (D->numItems)--;
}

// makeEmpty()
// Reset D to the empty state, the empty set of pairs.
void makeEmpty(Dictionary D)
{
   Node N;
   for (int i = 0; i < tableSize; i++)
   {
      while (D->table[i] != NULL)
      {
         N = D->table[i];
         D->table[i] = N->next;
         freeNode(&N);
         N = NULL;
         (D->numItems)--;
      }
      D->table[i] = NULL;
   }
}

// Other Operations -----------------------------------------------------------

// DictionaryToString()
// Determines a text representation of the current state of Dictionary D. Each
// (key, value) pair is represented as the chars in key, followed by a space,
// followed by the chars in value, followed by a newline '\n' char. The pairs
// occur in alphabetical order by key. The function returns a pointer to a char
// array, allocated from heap memory, containing the text representation
// described above, followed by a terminating null '\0' char. It is the
// responsibility of the calling function to free this memory.
char *DictionaryToString(Dictionary D)
{
   char* str;
   char temp[256];
   int i = 0;
   Node N;
   if(D == NULL){
      fprintf(stderr,
              "Dictionary Error: calling DictionaryToString() on NULL Dictionary reference\n");
      exit(EXIT_FAILURE);
   }
   int count = countChars(D);
   str = calloc((count+1), sizeof(char));

   while(tableSize > i){
      N = D->table[i];
      while(N!=NULL){
         sprintf(temp, "%s %s\n", N->key, N->value);
         N = N->next;
         strcat(str, temp);
      }
      i++;
   }
   return str;
}

// rotate_left()
// rotate the bits in an unsigned int
unsigned int rotate_left(unsigned int value, int shift)
{
   int sizeInBits = 8 * sizeof(unsigned int);
   shift = shift & (sizeInBits - 1);
   if (shift == 0)
      return value;
   return (value << shift) | (value >> (sizeInBits - shift));
}

// pre_hash()
// turn a string into an unsigned int
unsigned int pre_hash(char *input)
{
   unsigned int result = 0xBAE86554;
   while (*input)
   {
      result ^= *input++;
      result = rotate_left(result, 5);
   }
   return result;
}

// hash()
// turns a string into an int in the range 0 to tableSize-1
int hash(char *key)
{
   return pre_hash(key) % tableSize;
}
