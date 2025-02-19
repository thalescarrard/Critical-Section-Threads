 This program solves the critical section problem for two threads using mutex locks.
 Both threads increment a shared counter up to 2,000,000 times each, but Thread 1
 increments the counter by 100 every time counter % 100 == 0 (bonus condition).
 The program ensures mutual exclusion, progress, and bounded wait.
