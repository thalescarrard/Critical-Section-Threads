/*
 *     - Anthony L Carvalho 6365160
 *     - Thales Moraes 6332398
    Description: 
    This program solves the critical section problem for two threads using mutex locks. 
    Both threads increment a shared counter up to 2,000,000 times each, but Thread 1 
    increments the counter by 100 every time counter % 100 == 0 (bonus condition).
    The program ensures mutual exclusion, progress, and bounded wait.
*/

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

// Global shared counter and mutex
int counter = 0;
int bonus_count = 0;  // To track how many times thread 1 got the bonus
pthread_mutex_t mutex;

// Thread 1 Function (Bonus Thread)
void* thread1_func(void* arg) {
    int local_updates = 0;

    while (local_updates < 2000000) {
        // Entry section
        pthread_mutex_lock(&mutex);

        // Critical section
        if (counter % 100 == 0) {
            // check if we can apply the bonus without exceeding the limit
            if (local_updates <= 1999900) { // leave room for the next updates
                counter += 100;  // bonus applied
                bonus_count++;
                local_updates += 100; // increment local_updates by 100 for the bonus
            } else if (local_updates < 2000000) {
                counter++; // increment by 1 if bonus cannot be applied
                local_updates++;
            }
        } else if (local_updates < 2000000) {
            counter++; // regular increment
            local_updates++; // increment the local update count
        }

        // Exit section
        pthread_mutex_unlock(&mutex);
    }

    // Remainder section
    printf("I'm thread1, I did %d updates and got the bonus %d times, counter = %d\n", 
            local_updates, bonus_count, counter);
    pthread_exit(0);
}

// Thread 2 Function (Regular Increment Thread)
void* thread2_func(void* arg) {
    int local_updates = 0;

    while (local_updates < 2000000) {
        // Entry section
        pthread_mutex_lock(&mutex);

        // Critical section
        if (local_updates < 2000000) {
            counter++; // Increment by 1
            local_updates++; // Increment the local update count
        }

        // Exit
        pthread_mutex_unlock(&mutex);
    }

    // Remainder section
    printf("I'm thread2, I did %d updates, counter = %d\n", local_updates, counter);
    pthread_exit(0);
}

int main() {
    pthread_t thread1, thread2;

    // Initialize mutex
    if (pthread_mutex_init(&mutex, NULL) != 0) {
        printf("Error: Mutex init failed\n");
        return 1;
    }

    // Create the threads
    if (pthread_create(&thread1, NULL, thread1_func, NULL) != 0) {
        printf("Error: Thread 1 creation failed\n");
        return 1;
    }

    if (pthread_create(&thread2, NULL, thread2_func, NULL) != 0) {
        printf("Error: Thread 2 creation failed\n");
        return 1;
    }

    // Wait threads to finish
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    // thread prints the final value of the counter
    printf("From parent: Final counter value = %d\n", counter);

    // Destroy the mutex
    pthread_mutex_destroy(&mutex);

    return 0;
}
