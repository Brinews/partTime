#include "hashtable.h"
#include <string.h>
#include <assert.h>

/* A hash function */
unsigned int HashFunction(char *key, unsigned int size) {
  unsigned int result = 0;
  unsigned int seed = 131;
  if (key == NULL) return result;
  while (*key != '\0') {
    result = (result * seed + (unsigned int) *key) % size;
    key++;
  }
  return result;
}

int CreateHashTable(HashTablePTR *hashTableHandle, unsigned int initialSize) {
  unsigned int i;
  /* creates (that is, allocates and initializes) a hash table object */
  HashTablePTR table = malloc(sizeof(HashTableObject));
  /* return	-1 if there was insufficient memory to allocate the hash table */
  if (table == NULL)
    return -1;
  table->list = malloc(sizeof(Node) * initialSize);
  /* return	-1 if there was insufficient memory to allocate the hash table */
  if (table->list == NULL)
    return -1;
  /* initialize each list */
  for (i = 0; i < initialSize; i++)
    table->list[i] = NULL;
  /* sets the size */
  table->size = initialSize;
  /* sets the number of keys */
  table->numKeys = 0;
  /* sets the value of sentinel to 0xDEADBEEF */
  table->sentinel = 0xDEADBEEF;
  /* sets the value of hashTableHandle to the address of the newly created hash table object */
  *hashTableHandle = table;
  /* returns 0 if the allocation and initialization was successful */
  return 0;
}

int DestroyHashTable(HashTablePTR *hashTableHandle) {
  unsigned int i;
  Node *node;
  /* returns -1 if there was insufficient memory to allocate the hash table */
  if (*hashTableHandle == NULL || (*hashTableHandle)->sentinel != 0xDEADBEEF) {
    return -1;
  }
  /* frees all of the memory allocated by the hash table API  */
  for (i = 0; i < (*hashTableHandle)->size; i++) {
    while ((*hashTableHandle)->list[i]) {
      node = (*hashTableHandle)->list[i];
      (*hashTableHandle)->list[i] = node->next;
      free(node->key);
      free(node);
    }
  }
  free((*hashTableHandle)->list);
  free(*hashTableHandle);
  /* sets the value of the hashTableHandle to NULL */
  *hashTableHandle = NULL;
  /* returns 0 if the destruction was successful */
  return 0;
}


int InsertEntry(HashTablePTR hashTable, char *key, void *data, void **previousDataHandle) {
  unsigned int index;
  Node *node;
  /* returns -1 if hashTable does not point to a HashTableObject created by CreateHashTable() */
  if (hashTable == NULL || hashTable->sentinel != 0xDEADBEEF) {
    return -1;
  }
  /* get the hash value of the key */
  index = HashFunction(key, hashTable->size);
  /* search for the key in the list */
  for (node = hashTable->list[index]; node != NULL; node = node->next) {
    if (strcmp(node->key, key) == 0) {
      /* returns 2 if there was an existing entry with the same key (and by implication that */
      /* previousDataHandle now points to the address of the previous data) */
      *previousDataHandle = node->data;
      node->data = data;
      return 2;
    }
  }
  /* create a new node */
  hashTable->numKeys++;
  node = malloc(sizeof(Node));
  node->next = NULL;
  /* copy the data address */
  node->data = data;
  /* copy the key content */
  node->key = malloc(sizeof(char) * (strlen(key) + 1));
  strcpy(node->key, key);
  if (hashTable->list[index] == NULL) {
    /* insert to an empty list */
    hashTable->list[index] = node;
    /* returns 0 if the insertion was successful */
    return 0;

  } else {
    /* insert to an non-empty list */
    node->next = hashTable->list[index];
    /* returns 1 if there was a hash collision and the insertion was successful */
    hashTable->list[index] = node;
    return 1;
  }
}

int DeleteEntry(HashTablePTR hashTable, char *key, void **dataHandle) {
  unsigned int index;
  Node *node, *temp;
  /* returns -1 if hashTable does not point to a HashTableObject created by CreateHashTable() */
  if (hashTable == NULL || hashTable->sentinel != 0xDEADBEEF) {
    return -1;
  }
  /* get the hash value of the key */
  index = HashFunction(key, hashTable->size);
  if (hashTable->list[index] == NULL) {
    /* returns -2 if the key was not found */
    return -2;

  } else if (strcmp(key, hashTable->list[index]->key) == 0) {
    /* remove the first node from the list */
    temp = hashTable->list[index];
    *dataHandle = temp->data;
    hashTable->list[index] = temp->next;
    free(temp->key);
    free(temp);
    hashTable->numKeys--;
    /* returns 0 if the entry was deleted successfully */
    return 0;

  } else {
    /* search for the key in the list  */
    for (node = hashTable->list[index]; node->next != NULL; node = node->next) {
      if (strcmp(key, node->next->key) == 0) {
        temp = node->next;
        *dataHandle = temp->data;
        node->next = temp->next;
        free(temp->key);
        free(temp);
        hashTable->numKeys--;
        /* returns 0 if the entry was deleted successfully */
        return 0;
      }
    }
    /* returns -2 if the key was not found */
    return -2;
  }
}

int FindEntry(HashTablePTR hashTable, char *key, void **dataHandle) {
  unsigned int index;
  Node *node;
  /* returns -1 if hashTable does not point to a HashTableObject created by CreateHashTable() */
  if (hashTable == NULL || hashTable->sentinel != 0xDEADBEEF) {
    return -1;
  }
  /* get the hash value of the key */
  index = HashFunction(key, hashTable->size);
  /* search for the key in the list */
  for (node = hashTable->list[index]; node != NULL; node = node->next) {
    if (strcmp(key, node->key) == 0) {
      *dataHandle = node->data;
      /* returns 0 if the entry was found */
      return 0;
    }
  }
  /* returns -2 if the key was not found */
  return -2;
}

int GetKeys(HashTablePTR hashTable, char *(*keysArrayHandle)[], unsigned int *keyCount) {
  unsigned int i, j = 0;
  Node *node;
  char ***keysArray = (char ***) keysArrayHandle;
  /* returns -1 if hashTable does not point to a HashTableObject created by CreateHashTable() */
  if (hashTable == NULL || hashTable->sentinel != 0xDEADBEEF) {
    return -1;
  }
  *keyCount = hashTable->numKeys;
  /* allocate the keys array */
  *keysArray = malloc(sizeof(char *) * hashTable->numKeys);
  /* copy the keys */
  for (i = 0; i < hashTable->size; i++) {
    for (node = hashTable->list[i]; node != NULL; node = node->next) {
      (*keysArray)[j] = malloc(sizeof(char) * (strlen(node->key) + 1));
      strcpy((*keysArray)[j], node->key);
      j++;
    }
  }
  assert(j == hashTable->numKeys);
  /* returns if the keys were returned successfully */
  return 0;
}

int GetLoadFactor(HashTablePTR hashTable, float *loadFactor) {
  /* returns -1 if hashTable does not point to a HashTableObject created by CreateHashTable() */
  if (hashTable == NULL || hashTable->sentinel != 0xDEADBEEF) {
    return -1;
  }
  /* calculate the load factor */
  *loadFactor = (float) hashTable->numKeys / hashTable->size;
  /* returns 0 if the load factor was determined and returned successfully */
  return 0;
}
