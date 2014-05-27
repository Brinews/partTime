#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
  char *key;
  void *data;
  struct Node *next;
} Node;

typedef struct {
  unsigned int sentinel;
  // whatever else you think is important to store in the HashTable struct
  Node **list;
  unsigned int size;
  unsigned int numKeys;
} HashTableObject;

typedef HashTableObject *HashTablePTR;

int CreateHashTable(HashTablePTR *hashTableHandle, unsigned int initialSize);

int DestroyHashTable(HashTablePTR *hashTableHandle);

int InsertEntry(HashTablePTR hashTable, char *key, void *data, void **previousDataHandle);

int DeleteEntry(HashTablePTR hashTable, char *key, void **dataHandle);

int FindEntry(HashTablePTR hashTable, char *key, void **dataHandle);

int GetKeys(HashTablePTR hashTable, char * (*keysArrayHandle)[], unsigned int *keyCount);

int GetLoadFactor(HashTablePTR hashTable, float *loadFactor);

#endif
