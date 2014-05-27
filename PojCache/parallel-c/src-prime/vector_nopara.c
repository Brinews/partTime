#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <stdbool.h>
#include <pthread.h>
#include <inttypes.h>

#include "vector.h"

/****
  added code
 **/

#include <string.h>

#define	MAXLENGTH	10000
#define MAXTHREAD	256

static int64_t g_seed = 0;
static int64_t g_length = 0;
static int64_t g_nthreads = 0;

typedef void (*sort_func)(int64_t s, int64_t e, int64_t *array);

typedef struct ThreadNode
{
	int64_t nThreadNum;
	int64_t nStartIndex;
	int64_t nRetValue;
	int64_t *vector;
	sort_func mysort;
} ThreadNode;

pthread_t threads[MAXTHREAD];
ThreadNode threadArgs[MAXTHREAD];

void* getMin(void *);
void* getMax(void *);

void* quicksort(void *); 
void ascSort(int64_t s, int64_t e, int64_t *array);
void dscSort(int64_t s, int64_t e, int64_t *array);
void ascMerge(int64_t *v, int64_t threadNum);
void dscMerge(int64_t *v, int64_t threadNum);

////////////////////////////////
///     UTILITY FUNCTIONS    ///
////////////////////////////////

/**
 * Sets the number of elements that each vector will contain
 */
void set_length(int64_t length) {

    g_length = length;
}

void set_threadnum(int64_t nthreads)
{
	g_nthreads = nthreads;
}

/**
 * Sets the seed used when generating pseudorandom numbers
 */
void set_seed(int64_t seed) {

    g_seed = seed;
}

/**
 * Returns pseudorandom number determined by the seed
 */
int64_t fast_rand(void) {

    g_seed = (214013 * g_seed + 2531011);
    return (g_seed >> 16) & 0x7FFF;
}

////////////////////////////////
///   VECTOR INITALISATIONS  ///
////////////////////////////////

/**
 * Returns new vector, with all elements set to zero
 */
int64_t* new_vector(void) {

    return (int64_t *) calloc(g_length, sizeof(int64_t));
}

/**
 * Returns new vector, with all elements set to given value
 */
int64_t* uniform_vector(int64_t value) {

	int64_t i;
    int64_t* vector = new_vector();

    for (i = 0; i < g_length; i++) {
        vector[i] = value;
    }

    return vector;
}

/**
 * Returns new vector, with elements generated at random using given seed
 */
int64_t* random_vector(int64_t seed) {

	int64_t i;
    int64_t* vector = new_vector();

    set_seed(seed);

    for (i = 0; i < g_length; i++) {
        vector[i] = fast_rand();
    }

    return vector;
}

/**
 * Returns whether given number is prime
 */
bool is_prime(int64_t number) {
	int64_t i;

    if (number == 0) {
        return false;
    } else if (number == 1) {
        return false;
    }

    for (i = 2; i < number; i++) {
        if (number % i == 0) {
            return false;
        }
    }

    return true;
}

/**
 * Returns new vector, containing primes numbers in sequence from given start
 */
int64_t* prime_vector_a(int64_t start) {

    int64_t* vector = new_vector();

    int64_t number = start;
    int64_t i = 0;

    while (i < g_length) {

        if (is_prime(number)) {
            vector[i] = number;
            i += 1;
        }

        number += 1;
    }

    return vector;
}

/* added */
int64_t* prime_vector(int64_t start) {

    int64_t* vector = new_vector();

    int64_t number;
    int64_t i, t;

	/* get primes */
	int64_t size = INT64_MAX;
	int64_t mSize = 1000000000;
	char *isprime = calloc(mSize, sizeof(char));

	memset(isprime, 1, sizeof(char)*mSize);
	isprime[0] = 0;
	isprime[1] = 0;

	for (i = 0; i*i < size; i++)
	{
		if (isprime[i])
		{
			for (number = i*i; number < mSize; number += i)
			{
				isprime[number] = 0;
			}
		}

		if (isprime[i] && i >= start)
		{
			vector[t] = i;
			t++;
		}

		if (t >= g_length) break;
	}

	free(isprime);

	return vector;
}

/**
 * Returns new vector, with elements in sequence from given start and step
 */
int64_t* sequence_vector(int64_t start, int64_t step) {

	int64_t i;
	int64_t* vector = new_vector();

	int64_t current = start;

	for (i = 0; i < g_length; i++) {
		vector[i] = current;
		current += step;
	}

	return vector;
}

////////////////////////////////
///     VECTOR OPERATIONS    ///
////////////////////////////////

/**
 * Returns new vector, cloning elements from given vector
 */
int64_t* cloned(int64_t* vector) {

	int64_t i;
    int64_t* clone = new_vector();

    for (i = 0; i < g_length; i++) {
        clone[i] = vector[i];
    }

    return clone;
}

/**
 * Returns new vector, with elements ordered in reverse
 */
int64_t* reversed(int64_t* vector) {

	int64_t i;
    int64_t* result = new_vector();

    for (i = 0; i < g_length; i++) {
        result[i] = vector[g_length - 1 - i];
    }

    return result;
}

void ascSort(int64_t s, int64_t e, int64_t *array)
{
	if (s<e)
	{
		int64_t x=array[s];
		int64_t i=s,j=e+1;
		int64_t temp;

		while (i<j)
		{
			while(--j && array[j]>x);
			while(++i && array[i]<x);

			if(i>=j)break;

			temp = array[i];
			array[i]=array[j];
			array[j]=temp;
		}

		temp = array[s];
		array[s] = array[j];
		array[j] = temp;

		ascSort(s,j-1, array);
		ascSort(j+1,e, array);
	}

	return;
}

void ascMerge(int64_t *array, int64_t threadNum)
{
	/* merge */

	int64_t *arr = cloned(array);
	int64_t min;

	int64_t i,j,N;
	int64_t index[MAXTHREAD];

	int64_t slice = (g_length + threadNum - 1) / threadNum;

	for(i=0; i<threadNum; i++)
		index[i] = i * slice;

	i=0;

	while(i<g_length)
	{
		min = INT64_MAX;
		N   = 0;

		for(j=0 ; j<threadNum; j++)
			if(index[j] < (j+1)*slice 
					&& index[j] < g_length
					&& min > array[index[j]])
			{
				min = array[index[j]];
				N   = j;
			}

		arr[i++] = min;
		index[N]++;
	}

	memcpy(array, arr, sizeof(int64_t)*g_length);

	free(arr);
}

void* quicksort(void *args)
{
	ThreadNode *node = (ThreadNode*) args;

	int64_t tn = node->nThreadNum;
	int64_t s = ((g_length+tn-1)/tn) * node->nStartIndex;
	int64_t e = s + ((g_length+tn-1)/tn);
	
	/* last left part */
	if (e > g_length) e = g_length;

	/* sort [s, e-1] */
	node->mysort(s, e-1, node->vector);

	return ((void*)NULL);
}

int ascCmp(const void *a, const void *b)
{
	int64_t p = *(int64_t *)a;
	int64_t q = *(int64_t *)b;

	if (p < q) return -1;
	else if (p == q) return 0;
	else return 1;
}

int64_t* ascending(int64_t* vector) 
{
	int64_t* result = cloned(vector);

	qsort(result, g_length, sizeof(int64_t),  ascCmp);

    return result;
}
/**
 * Returns new vector, with elements ordered from smallest to largest
 */
int64_t* ascending_para(int64_t* vector) {

	int64_t* result = cloned(vector);

	/*
        to do

        look at the week 8 tutorial
        and then run this in parallel
    */

	int64_t i;
	
	/* calculate threads nums */

	int64_t threadNum = g_nthreads;
	while (threadNum > 1 && g_length/threadNum < MAXLENGTH)
	{
		threadNum --;
	}

	memset(threadArgs, 0, sizeof(ThreadNode));

	/* create thread */
	for (i = 0; i < threadNum; i++)
	{
		threadArgs[i].nThreadNum = threadNum;
		threadArgs[i].vector = result;
		threadArgs[i].mysort = ascSort;
		threads[i] = i;
		
		pthread_create(&threads[i], NULL, quicksort, &threadArgs[i]);
	}

	for (i = 0; i < threadNum; i++)
		pthread_join(threads[i], NULL);

	/* do merge */
	ascMerge(result, threadNum);

    return result;
}

/* quick sort by desc */
void dscSort(int64_t s, int64_t e, int64_t *array)
{
	if (s<e)
	{
		int64_t x=array[s];
		int64_t i=s,j=e+1;
		int64_t temp;

		while (i<j)
		{
			while(--j && array[j]<x);
			while(++i && array[i]>x);

			if(i>=j)break;

			temp = array[i];
			array[i]=array[j];
			array[j]=temp;
		}

		temp = array[s];
		array[s] = array[j];
		array[j] = temp;

		dscSort(s,j-1, array);
		dscSort(j+1,e, array);
	}

	return;
}

/* merge the sort-array */
void dscMerge(int64_t *array, int64_t threadNum)
{
	/* merge */

	int64_t *arr = cloned(array);
	int64_t max;

	int64_t i,j,N;
	int64_t index[MAXTHREAD];

	int64_t slice = (g_length + threadNum - 1) / threadNum;

	for(i=0; i<threadNum; i++)
		index[i] = i * slice;

	i=0;

	while(i<g_length)
	{
		max = INT64_MIN;
		N   = 0;

		for(j=0 ; j<threadNum; j++)
			if(index[j] < (j+1)*slice 
					&& index[j] < g_length
					&& max < array[index[j]])
			{
				max = array[index[j]];
				N   = j;
			}

		arr[i++] = max;
		index[N]++;
	}

	memcpy(array, arr, sizeof(int64_t)*g_length);

	free(arr);
}

int dscCmp(const void *a, const void *b)
{
	int64_t q = *(int64_t *)a;
	int64_t p = *(int64_t *)b;

	if (p < q) return -1;
	else if (p == q) return 0;
	else return 1;
}

int64_t* descending(int64_t* vector) {

    int64_t* result = cloned(vector);

	qsort(result, g_length, sizeof(int64_t), dscCmp);

	return result;
}
/**
 * Returns new vector, with elements ordered from largest to smallest
 */
int64_t* descending_para(int64_t* vector) {

    int64_t* result = cloned(vector);

    /*
        to do

        look at the week 8 tutorial
        and then run this in parallel
    */
	int64_t i;
	
	/* calculate threads nums */

	int64_t threadNum = g_nthreads;
	while (threadNum > 1 && g_length/threadNum < MAXLENGTH)
	{
		threadNum --;
	}

	memset(threadArgs, 0, sizeof(ThreadNode));

	/* create thread */
	for (i = 0; i < threadNum; i++)
	{
		threadArgs[i].nThreadNum = threadNum;
		threadArgs[i].vector = result;
		threadArgs[i].mysort = dscSort;
		threads[i] = i;
		
		pthread_create(&threads[i], NULL, quicksort, &threadArgs[i]);
	}

	for (i = 0; i < threadNum; i++)
		pthread_join(threads[i], NULL);

	/* do merge */
	dscMerge(result, threadNum);

    return result;
}

/**
 * Returns new vector, adding scalar to each element
 */
int64_t* scalar_add(int64_t* vector, int64_t scalar) {

	int64_t i;
    int64_t* result = new_vector();

    for (i = 0; i < g_length; i++) {
        result[i] = vector[i] + scalar;
    }

    return result;
}

/**
 * Returns new vector, multiplying scalar to each element
 */
int64_t* scalar_mul(int64_t* vector, int64_t scalar) {

	int64_t i;
    int64_t* result = new_vector();

    for (i = 0; i < g_length; i++) {
        result[i] = vector[i] * scalar;
    }

    return result;
}

/**
 * Returns new vector, adding elements with the same index
 */
int64_t* vector_add(int64_t* vector_a, int64_t* vector_b) {

	int64_t i;
    int64_t* result = new_vector();

    for (i = 0; i < g_length; i++) {
        result[i] = vector_a[i] + vector_b[i];
    }

    return result;
}

/**
 * Returns new vector, multiplying elements with the same index
 */
int64_t* vector_mul(int64_t* vector_a, int64_t* vector_b) {

	int64_t i;
    int64_t* result = new_vector();

    for (i = 0; i < g_length; i++) {
        result[i] = vector_a[i] * vector_b[i];
    }

    return result;
}

////////////////////////////////
///       COMPUTATIONS       ///
////////////////////////////////

/**
 * Returns the sum of all elements
 */
int64_t get_sum(int64_t* vector) {

	int64_t i;
    int64_t sum = 0;

    for (i = 0; i < g_length; i++) {
        sum += vector[i];
    }

    return sum;
}

/**
 * Returns the most frequently occuring element
 * or -1 if there is no such unique element
 */
int64_t get_mode(int64_t* vector) {

    /*
        to do

        [1 2 2] => 2
        [1 2 3] => -1
		[1 2 2 3 3] =?? -1
    */

	int64_t i;
	int64_t ret = -1, cur;
	int64_t curTimes, maxTimes = INT64_MIN;

	int64_t* ascVec = ascending(vector);

	i = 0;

	while (i < g_length)
	{
		cur = ascVec[i];
		curTimes = 1;

		while (++i < g_length && ascVec[i] == cur)
		{
			curTimes++;
		}

		if (curTimes > maxTimes)
		{
			maxTimes = curTimes;
			ret = cur;
		}
		else if (curTimes == maxTimes)
		{
			ret = -1;
		}
	}

	free(ascVec);

    return ret;
}

/**
 * Returns the lower median
 */
int64_t get_median(int64_t* vector) {

    /*
        to do

        [1 2 3] ===> 2
        [1 2 3 4] => 2
    */
	int64_t* tempv = cloned(vector);

	int64_t start = 0;
	int64_t end = g_length - 1;
	int64_t mpivot = end / 2;
	int64_t value;
	int64_t left, right;

	while (start < end) {
		value = tempv[mpivot];

		left = start;
		right = end;

		tempv[mpivot] = tempv[end];

		while (left != right) {
			while (tempv[left] < value && left < right)
				left ++;

			if (left < right) {
				tempv[right] = tempv[left];
				right --;
			}

			while (tempv[right] > value && left < right)
				right --;

			if (left < right) {
				tempv[left] = tempv[right];
				left ++;
			}
		}
		tempv[left] = value;

		if (right < mpivot)
			start = right + 1;
		else if (left > mpivot)
			end = left - 1;
		else
			break;
	}

	value = tempv[mpivot];
	free(tempv);

    return value;
}

void* getMin(void *args)
{
	int64_t i;
	int64_t min = INT64_MAX;
	
	/* get parameters */

	ThreadNode *node = (ThreadNode *)args;
	
	for (i = node->nStartIndex; i < g_length; i+= node->nThreadNum)
	{
		if (node->vector[i] < min)
			min = node->vector[i];
	}

	node->nRetValue = min;

	return ((void*)NULL);
}

/**
 * Returns the smallest value in the vector
 */
int64_t get_minimum(int64_t* vector) {

    /*
        to do

        [1 2 3] => 1
        [2 3 1] => 1
    */
	int64_t i;
	
	/* calculate threads nums */

	int64_t threadNum = g_nthreads;
	while (threadNum > 1 && g_length/threadNum < MAXLENGTH)
	{
		threadNum --;
	}
	
	memset(threadArgs, 0, sizeof(ThreadNode));

	/* create thread */
	
	for (i = 0; i < threadNum; i++)
	{
		threadArgs[i].nThreadNum = threadNum;
		threadArgs[i].vector = vector;
		threadArgs[i].nStartIndex = i;
		threads[i] = i;
		pthread_create(&threads[i], NULL, getMin, &threadArgs[i]);
	}

	for (i = 0; i < threadNum; i++)
		pthread_join(threads[i], NULL);

	/* get the Min from min_array */
	int64_t min = INT64_MAX;

	for (i = 0; i < threadNum; i++)
	{
		if (min > threadArgs[i].nRetValue)
			min = threadArgs[i].nRetValue;
	}

    return min;
}

void* getMax(void *args)
{
	int64_t i;
	int64_t max = INT64_MIN;

	/* get parameters */
	ThreadNode *node = (ThreadNode*) args;

	for (i = node->nStartIndex; i < g_length; i+=node->nThreadNum)
	{
		if (node->vector[i] > max)
			max = node->vector[i];
	}

	node->nRetValue = max;

	return ((void*)NULL);
}

/**
 * Returns the largest value in the vector
 */
int64_t get_maximum(int64_t* vector) {

    /*
        to do

        [1 2 3] => 3
        [2 3 1] => 3
    */
	int64_t i;

	/* calculate threads nums */

	int64_t threadNum = g_nthreads;
	while (threadNum > 1 && g_length / threadNum < MAXLENGTH)
	{
		threadNum--;
	}

	memset(threadArgs, 0, sizeof(ThreadNode));

	//printf("TN=%d\n", threadNum);

	/* create thread */

	for (i = 0; i < threadNum; i++)
	{
		threadArgs[i].nThreadNum = threadNum;
		threadArgs[i].vector = vector;
		threadArgs[i].nStartIndex = i;

		threads[i] = i;
		pthread_create(&threads[i], NULL, getMax, &threadArgs[i]);
	}

	for (i = 0; i < threadNum; i++)
		pthread_join(threads[i], NULL);

	/* get the Max from max_array */
	int64_t max = INT64_MIN;

	for (i = 0; i < threadNum; i++)
	{
		if (max < threadArgs[i].nRetValue)
		{
			max = threadArgs[i].nRetValue;
		}
	}

	return max;
}

/**
 * Returns the frequency of the value in the vector
 */
int64_t get_frequency(int64_t* vector, int64_t value) {

	int64_t i;
    int64_t count = 0;

    for (i = 0; i < g_length; i++) {
        if (vector[i] == value) {
            count += 1;
        }
    }

    return count;
}

/**
 * Returns the value stored at the given element index
 */
int64_t get_element(int64_t* vector, int64_t index) {

    return vector[index];
}

/**
 * Output given vector to standard output
 */
void display(int64_t* vector, int64_t label) {

	int64_t i;
    printf("vector~%" PRId64 " ::", label);

    for (i = 0; i < g_length; i++) {
        printf(" %" PRId64, vector[i]);
    }

    puts("");
}
