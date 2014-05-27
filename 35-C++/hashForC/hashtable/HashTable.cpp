#include "hashtable.h"
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#define GET_COLLIDE 1
#define GET_EXISTS 2
#define GET_NONE 0

#define NOT_A_HASH_TABLE -1

#define true 1
#define false 0

typedef struct Node {
	char *key;
	void *data;
	struct Node *left, *right;
	unsigned int nodeCnt;
} Node;

struct HashTableObjectTag {
	unsigned int sentinel;
	Node **list;
	unsigned int size;
	unsigned int numKeys;
	unsigned int usedBuckets;
	unsigned int maxNodeCnt;

	int dynamicBehaviour;
	float expandUseFactor;
	float contractUseFactor;
};

int GetHashTableInfo(HashTablePTR hashTable, HashTableInfo *pHashTableInfo)
{
	if (NULL == hashTable || hashTable->sentinel != 0xDEADBEEF)
	{
		return NOT_A_HASH_TABLE;
	}

	pHashTableInfo->loadFactor=(float)hashTable->numKeys/hashTable->size;
	pHashTableInfo->useFactor =(float)hashTable->usedBuckets/hashTable->size;
	pHashTableInfo->largestBucketSize = hashTable->maxNodeCnt;
	pHashTableInfo->bucketCount = hashTable->size;

	pHashTableInfo->dynamicBehaviour = hashTable->dynamicBehaviour;
	pHashTableInfo->expandUseFactor = hashTable->expandUseFactor;
	pHashTableInfo->contractUseFactor = hashTable->contractUseFactor;

	return 0;
}

int SetResizeBehaviour(HashTablePTR hashTable, int dynamicBehaviour, float expandUseFactor, float contractUseFactor)
{
	if (NULL == hashTable || hashTable->sentinel != 0xDEADBEEF)
	{
		return NOT_A_HASH_TABLE;
	}

	if (contractUseFactor >= expandUseFactor)
		return 1;

	hashTable->dynamicBehaviour = 1;
	if (dynamicBehaviour == 0)
		hashTable->dynamicBehaviour = 0;

	hashTable->expandUseFactor = expandUseFactor;
	hashTable->contractUseFactor = contractUseFactor;

	return 0;
}

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
  HashTablePTR table =(HashTablePTR) malloc(sizeof(HashTableObject));
  /* return	-1 if there was insufficient memory to allocate the hash table */
  if (table == NULL)
    return -1;
  table->list =(Node**) malloc(sizeof(Node *) * initialSize);
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

  table->usedBuckets = 0;
  table->maxNodeCnt = 0;

  /* sets the value of sentinel to 0xDEADBEEF */
  table->sentinel = 0xDEADBEEF;

  float a = 0.7, b = 0.2;
  SetResizeBehaviour(table, 1, (float)a, (float)b);
  /* sets the value of hashTableHandle to the address of the newly created hash table object */
  *hashTableHandle = table;

  /* returns 0 if the allocation and initialization was successful */
  return 0;
}

static void destroyTreeNode(Node *root)
{
	if (NULL != root)
	{
		destroyTreeNode(root->left);
		destroyTreeNode(root->right);

		free(root->key);
		free(root);
	}
}

int DestroyHashTable(HashTablePTR *hashTableHandle) {
  unsigned int i;

  /* returns -1 if there was insufficient memory to allocate the hash table */
  if (*hashTableHandle == NULL || (*hashTableHandle)->sentinel != 0xDEADBEEF) {
    return -1;
  }
  /* frees all of the memory allocated by the hash table API  */
  for (i = 0; i < (*hashTableHandle)->size; i++) {
    if (NULL != (*hashTableHandle)->list[i]) {
		destroyTreeNode((*hashTableHandle)->list[i]);
    }
  }

  free((*hashTableHandle)->list);
  free(*hashTableHandle);

  /* sets the value of the hashTableHandle to NULL */
  *hashTableHandle = NULL;

  /* returns 0 if the destruction was successful */
  return 0;
}

/***
 * ret=true, retNode=found node; 
 * ret=false, retNode=parent Node
 **/
int findTreeNode(Node *root, char *key, Node** ret, Node** paraNode)
{
	int r = false;

	if (NULL != root)
	{
		int r = strcmp(root->key, key);

		if (r == 0) {
			*ret = root;
			return true;
		}
		else if (r > 0) {
			*paraNode = root;
			*ret = root;
			r = findTreeNode(root->left, key, ret, paraNode);
			if (NULL == root->left) {
				//*ret = root;
				//*paraNode = root;
			}
			return r;
		}
		else {
			*paraNode = root;
			*ret = root;
			r = findTreeNode(root->right, key, ret, paraNode);
			if (NULL == root->right) {
				//*ret = root;
				//*paraNode = root;
			}
			return r;
		}
	}
	else 
		return false;
}

int InsertEntry(HashTablePTR hashTable, char *key, void *data, void **previousDataHandle) {
  int isFind = false;
  unsigned int index;
  Node *node;
  Node *retNode, *paraNode;
  /* returns -1 if hashTable does not point to a HashTableObject created by CreateHashTable() */
  if (hashTable == NULL || hashTable->sentinel != 0xDEADBEEF) {
    return -1;
  }
  /* get the hash value of the key */
  index = HashFunction(key, hashTable->size);

  /* search for the key in the list */
  node = hashTable->list[index];
  isFind = findTreeNode(node, key, &retNode, &paraNode);

  if (isFind == true)
  {
	  *previousDataHandle = retNode->data;
	  retNode->data = data;

	  return GET_EXISTS;
  }

  /* create a new node */
  hashTable->numKeys++;
  node =(Node*) malloc(sizeof(Node));
  
  node->left = node->right = NULL;
  /* copy the data address */
  node->data = data;
  /* copy the key content */
  node->key = (char*)malloc(sizeof(char) * (strlen(key) + 1));
  strcpy(node->key, key);

  if (hashTable->list[index] == NULL) {
	node->nodeCnt = 1;
	hashTable->usedBuckets++;

	if (node->nodeCnt > hashTable->maxNodeCnt)
		hashTable->maxNodeCnt = node->nodeCnt;

    /* insert to an empty list */
    hashTable->list[index] = node;

    /* returns 0 if the insertion was successful */
    return 0;

  } else {
	int nCnt = ++(hashTable->list[index]->nodeCnt);
	if (nCnt > hashTable->maxNodeCnt)
		hashTable->maxNodeCnt = nCnt;

    /* insert to an non-empty list */
	if (strcmp(retNode->key, key) > 0) {
		retNode->left = node;
	}
	else {
		retNode->right = node;
	}

    /* returns 1 if there was a hash collision and the insertion was successful */
    return 1;
  }
}

static int deleteTreeNode(Node **broot, char *key, void **data)
{
	Node *root = *broot;
	Node *retNode, *pNode;

	if (findTreeNode(root, key, &retNode, &pNode) == false)
	{
		return false;
	}

	Node *p = retNode;
	//1. p is leaf node
	if (p->left == NULL && p->right == NULL) {
		if (p == *broot) {
			*broot = NULL;
		}
		else if ((pNode)->left == p) {
			(pNode)->left = NULL;
		}
		else {
			(pNode)->right = NULL;
		}

		*data = p->data;
		free(p->key);
		free(p);
	}
	else if (p->left == NULL || p->right == NULL) {
	//2. p has single child
		if (p == *broot) {
			if (p->left == NULL)
				*broot = p->right;
			else
				*broot = p->left;
		}
		else {
			if ((pNode)->left == p && p->left) {
				(pNode)->left = p->left;
			} else if ((pNode)->left == p && p->right) {
				(pNode)->left = p->right;
			} else if ((pNode)->right == p && p->left) {
				(pNode)->right = p->left;
			} else {
				(pNode)->right = p->right;
			}
		}

		*data = p->data;
		free(p->key);
		free(p);
	}
	else {
	//3. p has two child
		Node *t = p;
		Node *s = p->left;
		while (s->right) {
			t = s;
			s = s->right;
		}

		*data = p->data;

		free(p->key);
		p->key = (char*) malloc(sizeof(char) * (strlen(s->key) + 1));
		strcpy(p->key, s->key);
		//p->key = s->key;
		p->data = s->data;

		if (t == p) {
			p->left = s->left;
		}
		else {
			t->right = s->left;
		}

		free(s->key);
		free(s);
	}

	return true;
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
  if (hashTable->list[index] == NULL 
	|| hashTable->numKeys <= 1) {
    /* returns -2 if the key was not found */
    return -2;
  }

  if (deleteTreeNode(&(hashTable->list[index]), key, dataHandle) == false) {
	  return -2;
  }
  else {
	  node = hashTable->list[index];
	  hashTable->numKeys--;

	  if (node == NULL) {
		  hashTable->usedBuckets--;
	  }
	  else {
		  node->nodeCnt--;
	  }
	  
	  //update maxNodeCnt;
	  for (int i = 0; i < hashTable->size; i++) {
		  temp = hashTable->list[i];
		  if (NULL != temp && temp->nodeCnt > hashTable->maxNodeCnt)
			  hashTable->maxNodeCnt = temp->nodeCnt;
	  }

	  return 0;
  }
}

int FindEntry(HashTablePTR hashTable, char *key, void **dataHandle) {
  unsigned int index;
  Node *node, *rnode, *pnode;
  /* returns -1 if hashTable does not point to a HashTableObject created by CreateHashTable() */
  if (hashTable == NULL || hashTable->sentinel != 0xDEADBEEF) {
    return -1;
  }
  /* get the hash value of the key */
  index = HashFunction(key, hashTable->size);

  /* search for the key in the list */
  node = hashTable->list[index];
  if (findTreeNode(node, key, &rnode, &pnode) == true)
  {
	  *dataHandle = rnode->data;
	  return 0;
  }

  /* returns -2 if the key was not found */
  return -2;
}

int idx = 0;
static void precopyTreeNode(Node *root, char ***keysArray)
{
	if (NULL != root) {
		precopyTreeNode(root->left, keysArray);
		(*keysArray)[idx] =(char*) malloc(sizeof(char) * (strlen(root->key) + 1));
		strcpy((*keysArray)[idx], root->key);
		idx++;
		precopyTreeNode(root->right, keysArray);
	}
}

int GetKeys(HashTablePTR hashTable, char ***keysArrayHandle, unsigned int *keyCount) {
  unsigned int i;
  Node *node;
  char ***keysArray = (char ***) keysArrayHandle;
  /* returns -1 if hashTable does not point to a HashTableObject created by CreateHashTable() */
  if (hashTable == NULL || hashTable->sentinel != 0xDEADBEEF) {
    return -1;
  }

  *keyCount = hashTable->numKeys;
  /* allocate the keys array */
  *keysArray = (char**)malloc(sizeof(char *) * hashTable->numKeys);

  /* copy the keys */
  idx = 0;
  for (i = 0; i < hashTable->size; i++) {
	node = hashTable->list[i];

	precopyTreeNode(node, keysArrayHandle);
  }

  //assert(idx == hashTable->numKeys);

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
