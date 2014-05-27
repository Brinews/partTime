#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  int maxnum; // Sieve up to, but not including, this number.
  if (argc != 2 || sscanf(argv[1], "%d", &maxnum) != 1) {
    printf("Usage: %s <maxnum>\n", argv[0]);
    return 1;
  }

  // Initialise the sieve - 0 and 1 are never prime.
  char *isprime = calloc(maxnum, sizeof(char));
  for (int i = 2; i < maxnum; i++) {
    isprime[i] = 1;
  }

  // Do the sieving step.
  for (int i = 0; i*i < maxnum; i++) {
    if (isprime[i]) {
      for (int j = i*i; j < maxnum; j += i) {
        isprime[j] = 0;
      }
    }
  }

  // Collect results.
  for (int i = 0; i < maxnum; i++) {
    if (isprime[i]) {
      printf("%d\n", i);
    }
  }

  return 0;
}

