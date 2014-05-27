#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <stdbool.h>
#include <pthread.h>
#include <inttypes.h>

#include "vector.h"

static int64_t g_seed = 0;
static int64_t g_length = 0;

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

/**
 * Returns whether given number is prime
 */
bool is_prime(int64_t number) {

    if (number == 0) {
        return false;
    } else if (number == 1) {
        return false;
    }

    for (int64_t i = 2; i < number; i++) {
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

    return -1;
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

    return -1;
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

    return INT64_MIN;
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

    return INT64_MAX;
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

