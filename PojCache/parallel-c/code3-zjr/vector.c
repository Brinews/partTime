#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <stdbool.h>
#include <pthread.h>
#include <inttypes.h>

#include "vector.h"

//#define _MYDEBUG
static int64_t g_seed = 0;
static int64_t g_length = 0;
static int64_t g_ncores        = 0; /* 1 <= n <= 128 */
static int64_t g_nthreads      = 0; /* 1 <= n <= 256 */
static int64_t maxDepth        = 0; /* 0 <= n <= 7   */
static int64_t mynthreads	   = 0;
static bool primetable	= false;
static bool *primemap   = NULL; 
#define MTHRD	256

#ifdef _MYDEBUG
#define PMAPMAX 10
#define PRENUM 4
#define MINISLICE 2
#define PBUFLEN 25
#define PSLICELEN PBUFLEN * 4
#else
#define PMAPMAX 200000
#define PRENUM 17984	/* There are @PRENUM primes in [1, 200k] */
#define MINISLICE 600
#define PBUFLEN 25
#define PSLICELEN PBUFLEN * 4
#endif

static pthread_t threads[MTHRD];
static int64_t smallprime[PRENUM];
#define INF  -1

////////////////////////////////
///     UTILITY FUNCTIONS    ///
////////////////////////////////

/**
 * Sets the number of elements that each vector will contain
 */
void set_length(int64_t length) {

    g_length = length;
}

/**
 * Sets the number of cores
 */
void set_ncores(int64_t ncores) {

    g_ncores = ncores;
}

/**
 * Sets the number of threads
 */
void set_nthreads(int64_t nthreads) {

    g_nthreads = nthreads;
	mynthreads = nthreads;
	int64_t temp = mynthreads;
	maxDepth = 0;
	temp = temp >> 1;
	while (temp) {
		maxDepth ++;
		temp = temp >> 1;
	}
}

void set_primetable(void) {

	primetable = false;
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

    int64_t* vector = new_vector();

    for (int64_t i = 0; i < g_length; i++) {
        vector[i] = value;
    }

    return vector;
}

/**
 * Returns new vector, with elements generated at random using given seed
 */
int64_t* random_vector(int64_t seed) {

    int64_t* vector = new_vector();

    set_seed(seed);

    for (int64_t i = 0; i < g_length; i++) {
        vector[i] = fast_rand();
    }

    return vector;
}

bool witness(int64_t a, int64_t n) {
	int64_t t, u, x0, x1, b, i;
	u = n - 1;
	t = 0;
	while (u % 2 == 0) {
		u /= 2;
		t ++;
	}
	b = a;
	x0 = 1;
	while (u) {
		if (u % 2)
			x0 = x0 * b % n;
		u /= 2;
		b = b * b % n;
	}
	for (i = 0; i < t; i ++)
	{
		x1 = x0 * x0 % n;
		if (x1 == 1 && x0 != 1 && x0 != n - 1)
			return true;
		x0 = x1;
	}
	if (x0 != 1)
		return true;
	return false;
}


/**
 * Returns whether given number is prime
 */
bool is_prime(int64_t *vector, int64_t startindex, int64_t index, int64_t number) {

	int64_t randomnum[4] = { 19, 71, 97, 137 };
    if (number == 0) {
        return false;
    } else if (number == 1) {
        return false;
    } else if (number == 2) {
		return true;
	}
	/* number % 2 === 1 */

	int64_t i;
	for (i = 0; i < 4; i ++) {
		if (number == randomnum[i])
			return true;
		if (witness(randomnum[i], number)) {
			return false;
		}
	}
	
	/* test using smallprimes */
    for (i = 0; smallprime[i] * smallprime[i] <= number && i < PRENUM; i++) {
        if (number % smallprime[i] == 0) {
            return false;
        }
    }
	if (i < PRENUM)
		return true;

	i = smallprime[PRENUM - 1] + 2;
	/* when first generate prime after smallprimes */
	if (index == startindex) {
		for ( ; i * i <= number; i += 2) {
			if (number % i == 0) {
				return false;
			}
		}
		return true;
	}

	/* test using numbers between the max 'smallprime' and the vector[0] if exsit */
	if (startindex == 0) {
		for ( ; i < vector[0] && i * i <= number; i += 2) {
			if (number % i == 0) {
				return false;
			}
		}
	}

	/* test using former primes in vector */
	int64_t idx;
	for (idx = startindex; idx < index && vector[idx] * vector[idx] <= number; idx ++) {
		if (number % vector[idx] == 0) {
			return false;
		}
	}

	/* exception for parallel program */
	if (idx == startindex) {
		return true;
	}
	i = vector[idx - 1] + 2;
	for ( ; i * i <= number; i += 2) {
		if (number % i == 0) {
			return false;
		}
	}
		
    return true;
}

/**
 * binary find the 1st prime in array smallprime 
*/
int64_t find_index_in_smallprime(int64_t value) {

	int64_t left = 0;
	int64_t right = PRENUM - 1;
	int64_t m;
	if (value < smallprime[left])
		return 0;
	if (value > smallprime[right])
		return PRENUM;
	while (left < right) {
		m = (left + right) / 2;
		if (smallprime[m] == value)
			return m;
		if (smallprime[m] > value)
			right = m;
		else
			left = m + 1;
	}
	return left;
}

struct prime_arg {
	int64_t *vector;
	int64_t startindex;
	int64_t index;
	int64_t start;
	int64_t end;
	int64_t buf[25];
	int64_t count;
} primeargs[MTHRD];

void *PMfind_prime(void *ptr) {

	struct prime_arg * Arg = (struct prime_arg *)ptr;
	pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
	pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
	Arg->count = 0;
	int64_t i;
	for (i = Arg->start; i < Arg->end; i = i + 2) {
		if (is_prime(Arg->vector, Arg->startindex, Arg->index, i)) {
		//	printf("thread %" PRId64 " generate prime %" PRId64 "\n", (int64_t)pthread_self(), i);
			Arg->buf[Arg->count] = i;
			Arg->count ++;
		}
	}
	return NULL;
}

/**
 * Returns new vector, containing primes numbers in sequence from given start
 */
int64_t* prime_vector(int64_t start) {

    int64_t* vector = new_vector();

    int64_t number = start;
    int64_t i = 0;
	int64_t j;

	if (primetable == false) {
		primemap = (bool *)calloc(PMAPMAX, sizeof(bool));
		if (primemap != NULL) {
			primemap[0] = false;
			primemap[1] = false;
			for (i = 2; i < PMAPMAX; i ++)
				primemap[i] = true;
			for (i = 2; i < PMAPMAX / 2; i ++) {
				if (primemap[i] == false)
					continue;
				for (j = i + i; j < PMAPMAX; j += i) {
					primemap[j] = false;
				}
			}
			j = 0;
			for (i = 2; i < PMAPMAX; i ++) {
				if (primemap[i]) {
					smallprime[j] = i;
					j ++;
				}
			}
			primetable = true;
			free(primemap);
		}
	}

	int64_t index = 0;
	int64_t startindex = 0;
	if (primetable) {
		i = find_index_in_smallprime(start);
		for ( ; i < PRENUM && index < g_length; i ++) {
			vector[index] = smallprime[i];
			index ++;
		}
	}
	if (index == 0 && number == 2) {
		vector[index] = number;
		index ++;
		number ++;
	}
	if (index == g_length)
		return vector;
	startindex = index;
	if (index > 0)
		number = vector[index - 1] + 2;
	
	if (number % 2 == 0) {
		number ++;
	}

	for (i = 0; i < mynthreads; i ++) {
		primeargs[i].vector = vector;
		primeargs[i].startindex = startindex;
		primeargs[i].index = index;
		primeargs[i].start = number + i * PSLICELEN;
		primeargs[i].end = number + (i + 1) * PSLICELEN;
	}
	for (i = 0; i < mynthreads; i ++) {
		pthread_create(&threads[i], NULL, PMfind_prime, (void *)&primeargs[i]);
	}
	i = 0;
	while (index < g_length) {
		pthread_join(threads[i], NULL);
		int64_t j;
		j = 0;
		while (index < g_length && j < primeargs[i].count) {
			vector[index] = primeargs[i].buf[j];
		//	printf("vector[%" PRId64 "] = %" PRId64 "\n", index, vector[index]);
			index ++;
			j ++;
		}
		if (index == g_length) {
			for (j = 0; j < mynthreads; j ++) {
				if (j != i)
					pthread_cancel(threads[j]);
			}
			for (j = 0; j < mynthreads; j ++) {
				if (j != i)
					pthread_join(threads[j], NULL);
			}
			return vector;
		} else {
			primeargs[i].index = index;
			primeargs[i].start += mynthreads * PSLICELEN;
			primeargs[i].end += mynthreads * PSLICELEN;
			pthread_create(&threads[i], NULL, PMfind_prime, (void *)&primeargs[i]);
		}
		i = (i + 1) % mynthreads;
	}

/*
    while (index < g_length) {

        if (is_prime(vector, startindex, index, number)) {
            vector[index] = number;
            index += 1;
        }

        number += 2;
    }
*/
    return vector;
}

/**
 * Returns new vector, with elements in sequence from given start and step
 */
int64_t* sequence_vector(int64_t start, int64_t step) {

    int64_t* vector = new_vector();

    int64_t current = start;

    for (int64_t i = 0; i < g_length; i++) {
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

    int64_t* clone = new_vector();

    for (int64_t i = 0; i < g_length; i++) {
        clone[i] = vector[i];
    }

    return clone;
}

/**
 * Returns new vector, with elements ordered in reverse
 */
int64_t* reversed(int64_t* vector) {

    int64_t* result = new_vector();

    for (int64_t i = 0; i < g_length; i++) {
        result[i] = vector[g_length - 1 - i];
    }

    return result;
}

struct merge_arg{
	int64_t * array;
	int64_t p;
	int64_t r;
	int64_t depth;
	int64_t max_depth;
	bool ascending;
};

void merge(int64_t *array, int64_t p, int64_t q, int64_t r, bool ascending) {

	int64_t nL = q - p + 1;
	int64_t nR = r - q;
	int64_t i, j, k;
    int64_t *L = (int64_t *) calloc(nL, sizeof(int64_t));
    int64_t *R = &array[q+1];

	if (L == NULL) {
		if (ascending) {
			for (j = q + 1; j <= r; j ++) {
				if (array[j] >= array[j - 1])
					break;
				int64_t temp = array[j];
				for (i = j - 1; i >= p; i --) {
					if (temp < array[i]) {
						array[i + 1] = array[i];
					} else {
						break;
					}
				}
				array[i + 1] = temp;
			}
		} else {
			for (j = q + 1; j <= r; j ++) {
				if (array[j] <= array[j - 1])
					break;
				int64_t temp = array[j];
				for (i = j - 1; i >= p; i --) {
					if (temp > array[i]) {
						array[i + 1] = array[i];
					} else {
						break;
					}
				}
				array[i + 1] = temp;
			}
		}
		return;
	}
	for (i = 0; i < nL; i ++) {
		L[i] = array[p + i];
	}
	i = 0;
	j = 0;
	if (ascending) {
		for (k = p; k <= r; k ++) {
			if (L[i] <= R[j]) {
				array[k] = L[i];
				i ++;
				if (i == nL) {
					break;
				}
			} else {
				array[k] = R[j];
				j ++;
				if (j == nR) {
					for (k ++; k <= r; k ++) {
						array[k] = L[i];
						i ++;
					}
				}
			}
		}
	} else {
		for (k = p; k <= r; k ++) {
			if (L[i] >= R[j]) {
				array[k] = L[i];
				i ++;
				if (i == nL) {
					break;
				}
			} else {
				array[k] = R[j];
				j ++;
				if (j == nR) {
					for (k ++; k <= r; k ++) {
						array[k] = L[i];
						i ++;
					}
				}
			}
		}
	} 
	free(L);
}

void merge_sort(int64_t *array, int64_t p, int64_t r, bool ascending) {

	int64_t q;
	if (p < r) {
		q = (p + r) / 2;
		merge_sort(array, p, q, ascending);
		merge_sort(array, q+1, r, ascending);
		merge(array, p, q, r, ascending);
	}
}

void *PMSort (void *ptr);

void ParallelMergeSort(int64_t *array, int64_t p, int64_t r, int64_t depth, int64_t max_depth, bool ascending) {

	if (depth == max_depth || r - p < MINISLICE) {
		merge_sort(array, p, r, ascending);
	} else {
		int64_t q;
		if (p < r) {
			struct merge_arg LeftArg, RightArg;
			pthread_t LThread, RThread;

			q = (p + r) / 2;
			LeftArg.array = array;
			LeftArg.p = p;
			LeftArg.r = q;
			LeftArg.depth = depth + 1;
			LeftArg.max_depth = max_depth;
			LeftArg.ascending = ascending;
			RightArg.array = array;
			RightArg.p = q + 1;
			RightArg.r = r;
			RightArg.depth = depth + 1;
			RightArg.max_depth = max_depth;
			RightArg.ascending = ascending;
			pthread_create(&LThread, NULL, PMSort, (void *)&LeftArg);
			pthread_create(&RThread, NULL, PMSort, (void *)&RightArg);
			pthread_join(LThread, NULL);
			pthread_join(RThread, NULL);
			merge(array, p, q, r, ascending);
		}
	}
}

void *PMSort(void *ptr) {

	struct merge_arg * MyArg = (struct merge_arg *) ptr;
	ParallelMergeSort(MyArg->array, MyArg->p, MyArg->r, MyArg->depth, MyArg->max_depth, MyArg->ascending);
	return NULL;
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

	ParallelMergeSort(result, 0, g_length - 1, 0, maxDepth, true);

    return result;
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

	ParallelMergeSort(result, 0, g_length - 1, 0, maxDepth, false);

    return result;
}

/**
 * Returns new vector, adding scalar to each element
 */
int64_t* scalar_add(int64_t* vector, int64_t scalar) {

    int64_t* result = new_vector();

    for (int64_t i = 0; i < g_length; i++) {
        result[i] = vector[i] + scalar;
    }

    return result;
}

/**
 * Returns new vector, multiplying scalar to each element
 */
int64_t* scalar_mul(int64_t* vector, int64_t scalar) {

    int64_t* result = new_vector();

    for (int64_t i = 0; i < g_length; i++) {
        result[i] = vector[i] * scalar;
    }

    return result;
}

/**
 * Returns new vector, adding elements with the same index
 */
int64_t* vector_add(int64_t* vector_a, int64_t* vector_b) {

    int64_t* result = new_vector();

    for (int64_t i = 0; i < g_length; i++) {
        result[i] = vector_a[i] + vector_b[i];
    }

    return result;
}

/**
 * Returns new vector, multiplying elements with the same index
 */
int64_t* vector_mul(int64_t* vector_a, int64_t* vector_b) {

    int64_t* result = new_vector();

    for (int64_t i = 0; i < g_length; i++) {
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

    int64_t sum = 0;

    for (int64_t i = 0; i < g_length; i++) {
        sum += vector[i];
    }

    return sum;
}

struct pair {
	int64_t value;
	int64_t count;
};

int64_t hash(int64_t value, int64_t length) {

	return (value ^ (value >> 2)) % length;
}

/**
 * Serial function to solve the operation 'get_mode'
 */
struct pair get_modepair(int64_t *vector, int64_t length) {

	int64_t i, j, maxj, maxcount;
	maxcount = 0;
	struct pair result;
	struct pair * hashmap;

	if (length == 0) {
		result.value = -1;
		result.count = 0;
		return result;
	}
	hashmap = (struct pair *) calloc(length, sizeof(struct pair));
	if (hashmap == NULL) {
		/* drop hash method */
		result.value = -1;
		result.count = 0;
		for (i = 0; i < length; i ++) {
			int64_t count = 1;
			for (j = i + 1; j < length; j ++) {
				if (vector[j] == vector[i])
					count ++;
			}
			if (count > result.count) {
				result.count = count;
				result.value = vector[i];
			} else if (count == result.count) {
				result.value = -1;
			}
		}

		return result;
	}
	for (j = 0; j < length; j ++) {
		hashmap[j].value = INF;
		hashmap[j].count = 0;
	}
	for (i = 0; i < length; i ++) {
		j = hash(vector[i], length);
		while (hashmap[j].count && vector[i] != hashmap[j].value)
			j = (j + 1) % length;
		hashmap[j].value = vector[i];
		hashmap[j].count ++;
		if (hashmap[j].count > maxcount) {
			maxcount = hashmap[j].count;
			maxj = j;
		}
	}

	result.count = maxcount;
	for (j = 0; j < length; j ++) {
		if (j == maxj)
			continue;
		if (hashmap[j].count == maxcount) {
			result.value = -1;
			if (hashmap != NULL)
				free(hashmap);
			return result;
		}
	}
	result.value = hashmap[maxj].value;
	if (hashmap != NULL)
		free(hashmap);
	return result;
}

/**
 * The argument for parallel function PM_get_modepair
 */
struct mode_arg {
	int64_t *vector;
	int64_t length;
	struct pair result;
};

/**
 * Parallel function to solve the operation 'get_mode'
 */
void *PM_get_modepair(void * ptr) {
	
	struct mode_arg * modeArg = (struct mode_arg*) ptr;
	modeArg->result = get_modepair(modeArg->vector, modeArg->length);
	return NULL;
}

int64_t get_mode_serial(int64_t *vector) {

	struct pair result;
	result = get_modepair(vector, g_length);
	return result.value;
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
    */

	int64_t i, j;
	struct pair result;
	if (g_length <= MINISLICE) {
		result = get_modepair(vector, g_length);
		return result.value;
	}
	/* divide to @mynthreads threads */
	struct mode_arg *mArgs = (struct mode_arg *)calloc(mynthreads, sizeof(struct mode_arg));
	if (mArgs == NULL)
		return get_mode_serial(vector);
	for (j = 0; j < mynthreads; j ++) {
		mArgs[j].vector = new_vector();
		if (mArgs[j].vector == NULL) {
			for (j --; j >= 0; j --) {
				free(mArgs[j].vector);
			}
			return get_mode_serial(vector);
		}
		mArgs[j].length = 0;
	}

	for (i = 0; i < g_length; i ++) {
		j = hash(vector[i], mynthreads);
		mArgs[j].vector[mArgs[j].length] = vector[i];
		mArgs[j].length ++;
	}

	for (j = 0; j < mynthreads; j ++) {
		pthread_create(&threads[j], NULL, PM_get_modepair, (void *)&mArgs[j]);
	}

	for (j = 0; j < mynthreads; j ++) {
		pthread_join(threads[j], NULL);
	}

	result.value = -1;
	result.count = 0;
	for (j = 0; j < mynthreads; j ++) {
		if (mArgs[j].vector != NULL) {
			free(mArgs[j].vector);
		}
		if (mArgs[j].result.count > result.count) {
			result = mArgs[j].result;
		} else if (mArgs[j].result.count == result.count) {
			result.value = -1;
		}
	}


	free(mArgs);
    return result.value;
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

	int64_t start = 0;
	int64_t end = g_length - 1;
	int64_t mpivot = end / 2;
	int64_t value;
	int64_t l, r;
	int64_t* tempv = cloned(vector);

	while (start < end) {
		value = tempv[mpivot];
		l = start;
		r = end;
		tempv[mpivot] = tempv[end];
		while (l != r) {
			while (tempv[l] < value && l < r)
				l ++;
			if (l < r) {
				tempv[r] = tempv[l];
				r --;
			}
			while (tempv[r] > value && l < r)
				r --;
			if (l < r) {
				tempv[l] = tempv[r];
				l ++;
			}
		}
		tempv[l] = value;
		if (r < mpivot)
			start = r + 1;
		else if (l > mpivot)
			end = l - 1;
		else
			break;
	}
	value = tempv[mpivot];
	free(tempv);
    return value;
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
	int64_t min = INT64_MAX;
	for (int64_t i = 0; i < g_length; i ++) {
		if (min > vector[i])
			min = vector[i];
	}

    return min;
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

	int64_t max = INT64_MIN;
	for (int64_t i = 0; i < g_length; i ++) {
		if (max < vector[i])
			max = vector[i];
	}

    return max;
}

/**
 * Returns the frequency of the value in the vector
 */
int64_t get_frequency(int64_t* vector, int64_t value) {

    int64_t count = 0;

    for (int64_t i = 0; i < g_length; i++) {
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

    printf("vector~%" PRId64 " ::", label);

    for (int64_t i = 0; i < g_length; i++) {
        printf(" %" PRId64, vector[i]);
    }

    puts("");
}

