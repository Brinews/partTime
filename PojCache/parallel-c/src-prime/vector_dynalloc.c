#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
//#include <unistd.h>
//#include <stdbool.h>
#include <pthread.h>
//#include <inttypes.h>

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

typedef void (*sort_func)(int s, int e, int64_t *array);

typedef struct ThreadNode
{
	int nThreadNum;
	int nStartIndex;
	int64_t nRetValue;
	int64_t *vector;
	sort_func mysort;
} ThreadNode;

pthread_t threads[MAXTHREAD];
ThreadNode threadArgs[MAXTHREAD];

void* getMin(void *);
void* getMax(void *);

void* quicksort(void *); 
void ascSort(int s, int e, int64_t *array);
void dscSort(int s, int e, int64_t *array);
void ascMerge(int64_t *v, int threadNum);
void dscMerge(int64_t *v, int threadNum);

/* added for primes */
#define chunkSize	20000
#define baseScope	9000

int64_t totalPrimes = 0;
int64_t primeSize = baseScope;
int64_t unsearchedScope_low_bound = baseScope+1;
int task_id = 0, result_id = 0;
int inAlloc = 0;

int64_t *primes = NULL;
int64_t cntPrimes = 0;
int64_t startPrimes = 2;

pthread_mutex_t mutex1, mutex2, mutex3;
pthread_cond_t cond, cond1;

void *searchPrimes(void *);

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
int is_prime(int64_t number)
{
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
int64_t* prime_vector(int64_t start) {

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

	//printf("number=%lld\n", number);

    return vector;
}


void free_primes()
{
	if (NULL != primes)
		free(primes);
}

void* searchPrimes(void *n)
{
	int my_id;
	int64_t i, k, m, *newPrimes;
	int64_t loc_low_bound, loc_up_bound;
	int64_t *locPrimes = (int64_t*) calloc(chunkSize, sizeof(int64_t));
	int locTotal = 0;

	while (1)
	{
		if (cntPrimes > g_length) break;

		pthread_mutex_lock(&mutex1);
		loc_low_bound = unsearchedScope_low_bound;
		unsearchedScope_low_bound += chunkSize;
		my_id = task_id;
		task_id++;
		pthread_mutex_unlock(&mutex1);

		loc_up_bound = loc_low_bound + chunkSize;
		locTotal = 0;

		pthread_mutex_lock(&mutex3);
		while (inAlloc == 1)
			pthread_cond_wait(&cond1, &mutex3);

		pthread_mutex_unlock(&mutex3);

		for (i = loc_low_bound; i < loc_up_bound; i+=2)
		{
			for (k = 0; primes[k]*primes[k] < i; k++)
			{
				if (primes[k] == 0)
				{
					printf("prime:%lld, %lld, %lld\n\n", k, primes[k], i);
					break;
				}

				if (i % primes[k] == 0) break;
			}

			//if (k > totalPrimes) break;

			if (primes[k]*primes[k] > i)
			{
				locPrimes[locTotal] = i;
				locTotal++;
			}
		}


		pthread_mutex_lock(&mutex2);
		while (my_id != result_id)
			pthread_cond_wait(&cond, &mutex2);

		/* when enough exit */
		if (cntPrimes < g_length)
		{
			for (i = 0; i < locTotal; i++)
			{
				if (cntPrimes > g_length)
					break;

				if (totalPrimes >= primeSize)
				{
					/*
					newPrimes = (int64_t*)malloc((primeSize+chunkSize)*sizeof(int64_t));
					memcpy(newPrimes, primes, sizeof(int64_t)*totalPrimes);
					*/
					inAlloc = 1;
					primes = realloc(primes, (primeSize<<1)*sizeof(int64_t));
					primeSize <<= 1;

					inAlloc = 0;
					pthread_cond_broadcast(&cond1);

					/*
					primes = newPrimes;
					primeSize = primeSize + chunkSize;
					*/
				}

				primes[totalPrimes] = locPrimes[i];

				totalPrimes++;

				if (locPrimes[i] > startPrimes)
					cntPrimes++;
			}
		}

		result_id++;
		pthread_cond_broadcast(&cond);
		pthread_mutex_unlock(&mutex2);
	}

	free(locPrimes);

	return((void*)NULL);   
}

int64_t* prime_vector_a(int64_t start)
{
	int64_t i, k, pos = 0;
	int64_t nowCnt = 0;

	int64_t *vector = new_vector();

	/* thread no init */
	task_id = result_id = 0;
	inAlloc = 0;

	cntPrimes = 0;
	startPrimes = start;

	if (NULL == primes)
	{
		primes = (int64_t*) calloc(primeSize, sizeof(int64_t));
		primes[0] = 2;
		totalPrimes = 1;

		for (i = 3; i < baseScope+1; i+=2)
		{
			for (k = 0; primes[k]*primes[k] < i; k++)
				if (i % primes[k] == 0) break;

			if (primes[k] * primes[k] > i)
			{
				primes[totalPrimes] = i;
				totalPrimes++;
			}
		}
	}

	for (i = 0; i < totalPrimes; i++)
	{
		if (primes[i] >= start && cntPrimes < g_length)
		{
			vector[cntPrimes++] = primes[i];
			pos = i+1;
		}
	}

	nowCnt = cntPrimes;

	if (cntPrimes >= g_length)
		return vector;

	//printf("Primes: %lld, %lld, %lld\n\n", cntPrimes, totalPrimes, pos);
	//return vector;

	pthread_mutex_init(&mutex1, NULL);
	pthread_mutex_init(&mutex2, NULL);
	pthread_mutex_init(&mutex3, NULL);
	pthread_cond_init(&cond, NULL);
	pthread_cond_init(&cond1, NULL);

	/* reuse */
	unsearchedScope_low_bound = baseScope + 1;
	if (primes[totalPrimes-1] > unsearchedScope_low_bound)
		unsearchedScope_low_bound = primes[totalPrimes-1]+2;

	for (i = 0; i < g_nthreads; i++)
	{
		pthread_create(&threads[i], NULL, searchPrimes, NULL);
	}

	for (i = 0; i < g_nthreads; i++)
		pthread_join(threads[i], NULL);

	//printf("Primes: %lld, %lld, %lld\n\n", cntPrimes, totalPrimes, pos);
	//return vector;

	while (pos < totalPrimes && primes[pos] < start)
		pos++;

	for (i = nowCnt; i < g_length; i++)
	{
		vector[i] = primes[pos++];
	}

	pthread_mutex_destroy(&mutex1);
	pthread_mutex_destroy(&mutex2);
	pthread_mutex_destroy(&mutex3);
	pthread_cond_destroy(&cond);
	pthread_cond_destroy(&cond1);

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

void ascSort(int s, int e, int64_t *array)
{
	if (s<e)
	{
		int x=array[s];
		int i=s,j=e+1;
		int temp;

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

void ascMerge(int64_t *array, int threadNum)
{
	/* merge */

	int64_t *arr = cloned(array);
	int64_t min;

	int i,j,N;
	int index[MAXTHREAD];

	int slice = (g_length + threadNum - 1) / threadNum;

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

	int tn = node->nThreadNum;
	int s = ((g_length+tn-1)/tn) * node->nStartIndex;
	int e = s + ((g_length+tn-1)/tn);
	
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

void* aQsort(void *args)
{
	ThreadNode *node = (ThreadNode*) args;

	int tn = node->nThreadNum;
	int s = ((g_length+tn-1)/tn) * node->nStartIndex;
	int e = s + ((g_length+tn-1)/tn);
	
	/* last left part */
	if (e > g_length) e = g_length;

	/* sort [s, e-1] */
	qsort(node->vector+s, e-s, sizeof(int64_t), ascCmp);

	return ((void*)NULL);
}

int dscCmp(const void *a, const void *b)
{
	int64_t q = *(int64_t *)a;
	int64_t p = *(int64_t *)b;

	if (p < q) return -1;
	else if (p == q) return 0;
	else return 1;
}

void* dQsort(void *args)
{
	ThreadNode *node = (ThreadNode*) args;

	int tn = node->nThreadNum;
	int s = ((g_length+tn-1)/tn) * node->nStartIndex;
	int e = s + ((g_length+tn-1)/tn);
	
	/* last left part */
	if (e > g_length) e = g_length;

	/* sort [s, e-1] */
	qsort(node->vector+s, e-s, sizeof(int64_t), dscCmp);

	return ((void*)NULL);
}

/**
 * Returns new vector, with elements ordered from smallest to largest
 */
int64_t* ascending(int64_t* vector) {

	int64_t* result = cloned(vector);

	/*
        to do

        look at the week 8 tutorial
        and then run this in parallel
    */

	int i;
	
	/* calculate threads nums */

	int threadNum = g_nthreads;
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
		
		//pthread_create(&threads[i], NULL, quicksort, &threadArgs[i]);
		pthread_create(&threads[i], NULL, aQsort, &threadArgs[i]);
	}

	for (i = 0; i < threadNum; i++)
		pthread_join(threads[i], NULL);

	/* do merge */
	ascMerge(result, threadNum);

    return result;
}

/* quick sort by desc */
void dscSort(int s, int e, int64_t *array)
{
	if (s<e)
	{
		int x=array[s];
		int i=s,j=e+1;
		int temp;

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
void dscMerge(int64_t *array, int threadNum)
{
	/* merge */

	int64_t *arr = cloned(array);
	int64_t max;

	int i,j,N;
	int index[MAXTHREAD];

	int slice = (g_length + threadNum - 1) / threadNum;

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

/**
 * Returns new vector, with elements ordered from largest to smallest
 */
int64_t* descending(int64_t* vector) {

    int64_t* result = cloned(vector);

    /*
        to do

        look at the week 8 tutorial
        and then run this in parallel
    */
	int i;
	
	/* calculate threads nums */

	int threadNum = g_nthreads;
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

	int i;
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

	int64_t *ascVec = ascending(vector);
	int64_t ret = -1;

	if (g_length > 0) 
	{
		if (g_length % 2) 
			ret = ascVec[g_length/2];
		else
			ret = ascVec[g_length/2 - 1];
	}

	free(ascVec);

    return ret;
}

void* getMin(void *args)
{
	int i;
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
	int i;
	int64_t min;
	
	/* calculate threads nums */

	int threadNum = g_nthreads;
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

		pthread_create(&threads[i], NULL, getMin, &threadArgs[i]);
	}

	for (i = 0; i < threadNum; i++)
		pthread_join(threads[i], NULL);

	/* get the Min from min_array */
	min = INT64_MAX;

	for (i = 0; i < threadNum; i++)
	{
		if (min > threadArgs[i].nRetValue)
			min = threadArgs[i].nRetValue;
	}

    return min;
}

void* getMax(void *args)
{
	int i;
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
	int i;
	int64_t max;

	/* calculate threads nums */

	int threadNum = g_nthreads;
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

		pthread_create(&threads[i], NULL, getMax, &threadArgs[i]);
	}

	for (i = 0; i < threadNum; i++)
		pthread_join(threads[i], NULL);

	/* get the Max from max_array */
	max = INT64_MIN;

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
