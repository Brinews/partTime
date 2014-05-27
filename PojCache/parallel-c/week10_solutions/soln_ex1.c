#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

// How many philosophers are dining?
#define NPHIL 5

pthread_t threads[NPHIL];
pthread_mutex_t forks[NPHIL];

void fail() {
  printf("Could not lock a mutex.\n");
  exit(1);
}

// Each thread is a philosopher at the table.
void* worker (void * arg) {
  // ID of the philosopher (from 0 to NPHIL-1)
  int id = *(int *) arg;

  // IDs of their left and right fork.
  int left = id;
  int right = (id + 1) % NPHIL;
  
  // All we need to do is impose a global order of locking:
  // lets simply lock lower numbered mutexes first.
  if (left > right) {
    int tmp = left;
    left = right;
    right = tmp;
  }

  for (;;) {
    // Pick up left, pick up right.
    if (pthread_mutex_lock(&forks[left]) != 0) {
      fail();
    }
    if (pthread_mutex_lock(&forks[right]) != 0) {
      fail();
    }

    // Om nom nom.
    printf("Philosopher %d is eating...\n", id);

    // Release left, release right.
    pthread_mutex_unlock(&forks[left]);
    pthread_mutex_unlock(&forks[right]);

    // Thinking.
    printf("Philosopher %d is thinking...\n", id);
  }
}

int main(void) {
  int args[NPHIL];

  // Create mutexes
  for (int i = 0; i < NPHIL; i++) {
    pthread_mutex_init(&forks[i], NULL);
  }
  
  // Start each philosopher dining. They never stop.
  for (int i = 0; i != NPHIL; i++) {
    args[i] = i;
    pthread_create(&threads[i], NULL, worker, &args[i]);
  }

  for (;;)
    sleep(1);
  
  return 0;
}
