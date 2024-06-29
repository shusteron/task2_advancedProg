// #include <stdio.h>
// #include <stdbool.h>
// #include <time.h> 


// // Function to check if a number is prime
// bool isPrime(int n) {
//     if (n <= 1) {
//         return false;
//     }
//     for (int i = 2; i * i <= n; i++) {
//         if (n % i == 0) {
//             return false;
//         }
//     }
//     return true;
// }

// int main() {
//     int num;
//     int total_counter = 0;

//     // Read numbers from stdin until end of file
//     while (scanf("%d", &num) != EOF) {   
//         if (isPrime(num)) {
//             total_counter++;
//         }
//     }
    
//     printf("%d total primes.\n", total_counter);

//     return 0;
// }






//----------------------------------------------SOLUTION 1-----------------------------------------------------------------

// #include <stdio.h>
// #include <stdlib.h>
// #include <pthread.h>
// #include <math.h>

// // Define the number of threads
// #define NUM_THREADS 2

// // Mutex to protect the shared count of primes
// pthread_mutex_t primeCountMutex;
// int primeCount = 0;

// typedef struct {
//     int *numbers;
//     int start;
//     int end;
// } ThreadData;

// // Function to check if a number is prime
// int isPrime(int n) {
//     if (n <= 1) return 0;
//     if (n == 2) return 1;
//     if (n % 2 == 0) return 0;
//     for (int i = 3; i <= sqrt(n); i += 2) {
//         if (n % i == 0) return 0;
//     }
//     return 1;
// }

// // Function to process a range of numbers and count primes
// void *processNumbers(void *arg) {
//     ThreadData *data = (ThreadData *)arg;
//     int localPrimeCount = 0;
//     for (int i = data->start; i < data->end; ++i) {
//         if (isPrime(data->numbers[i])) {
//             ++localPrimeCount;
//         }
//     }

//     pthread_mutex_lock(&primeCountMutex);
//     primeCount += localPrimeCount;
//     pthread_mutex_unlock(&primeCountMutex);

//     return NULL;
// }

// int main() {
//     // Read input numbers from stdin
//     int *numbers = NULL;
//     int number;
//     size_t numbersCount = 0;
//     size_t capacity = 10;
//     numbers = malloc(capacity * sizeof(int));
//     if (numbers == NULL) {
//         perror("Unable to allocate memory");
//         return EXIT_FAILURE;
//     }

//     while (scanf("%d", &number) != EOF) {
//         if (numbersCount >= capacity) {
//             capacity *= 2;
//             numbers = realloc(numbers, capacity * sizeof(int));
//             if (numbers == NULL) {
//                 perror("Unable to reallocate memory");
//                 return EXIT_FAILURE;
//             }
//         }
//         numbers[numbersCount++] = number;
//     }

//     // Calculate the chunk size
//     size_t chunkSize = (numbersCount + NUM_THREADS - 1) / NUM_THREADS; // Ensure we cover all numbers

//     // Initialize the mutex
//     pthread_mutex_init(&primeCountMutex, NULL);

//     // Create and launch threads
//     pthread_t threads[NUM_THREADS];
//     ThreadData threadData[NUM_THREADS];

//     for (unsigned int i = 0; i < NUM_THREADS; ++i) {
//         threadData[i].numbers = numbers;
//         threadData[i].start = i * chunkSize;
//         threadData[i].end = (i + 1) * chunkSize;
//         if (threadData[i].end > numbersCount) {
//             threadData[i].end = numbersCount;
//         }
//         if (threadData[i].start < threadData[i].end) {
//             pthread_create(&threads[i], NULL, processNumbers, &threadData[i]);
//         }
//     }

//     // Wait for all threads to finish
//     for (unsigned int i = 0; i < NUM_THREADS; ++i) {
//         pthread_join(threads[i], NULL);
//     }

//     // Destroy the mutex
//     pthread_mutex_destroy(&primeCountMutex);

//     // Output the result
//     printf("%d total primes.\n", primeCount);

//     // Free allocated memory
//     free(numbers);

//     return 0;
// }


//----------------------------------------------SOLUTION 2 -----------------------------------------------------------------


#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>

#define NUM_THREADS 4

pthread_mutex_t primeCountMutex;
int primeCount = 0;

typedef struct {
    int *numbers;
    int start;
    int end;
} ThreadData;

int isPrime(int n) {
    if (n <= 1) return 0;
    if (n <= 3) return 1;
    if (n % 2 == 0 || n % 3 == 0) return 0;

    for (int i = 5; i * i <= n; i += 6) {
        if (n % i == 0 || n % (i + 2) == 0) return 0;
    }
    return 1;
}

void *processNumbers(void *arg) {
    ThreadData *data = (ThreadData *)arg;
    int localPrimeCount = 0;
    for (int i = data->start; i < data->end; ++i) {
        if (isPrime(data->numbers[i])) {
            ++localPrimeCount;
        }
    }

    pthread_mutex_lock(&primeCountMutex);
    primeCount += localPrimeCount;
    pthread_mutex_unlock(&primeCountMutex);

    return NULL;
}

int main() {
    int *numbers = NULL;
    int number;
    size_t numbersCount = 0;
    size_t capacity = 1000000;  // Initial capacity for numbers array

    numbers = malloc(capacity * sizeof(int));
    if (numbers == NULL) {
        perror("Unable to allocate memory");
        return EXIT_FAILURE;
    }

    while (scanf("%d", &number) != EOF) {
        if (numbersCount >= capacity) {
            // Increase capacity by a smaller factor
            capacity += 1000000;
            numbers = realloc(numbers, capacity * sizeof(int));
            if (numbers == NULL) {
                perror("Unable to reallocate memory");
                return EXIT_FAILURE;
            }
        }
        numbers[numbersCount++] = number;
    }

    size_t chunkSize = (numbersCount + NUM_THREADS - 1) / NUM_THREADS;

    pthread_mutex_init(&primeCountMutex, NULL);

    pthread_t threads[NUM_THREADS];
    ThreadData threadData[NUM_THREADS];

    for (unsigned int i = 0; i < NUM_THREADS; ++i) {
        threadData[i].numbers = numbers;
        threadData[i].start = i * chunkSize;
        threadData[i].end = (i + 1) * chunkSize;
        if (threadData[i].end > numbersCount) {
            threadData[i].end = numbersCount;
        }
        if (threadData[i].start < threadData[i].end) {
            pthread_create(&threads[i], NULL, processNumbers, &threadData[i]);
        }
    }

    for (unsigned int i = 0; i < NUM_THREADS; ++i) {
        pthread_join(threads[i], NULL);
    }

    pthread_mutex_destroy(&primeCountMutex);

    printf("%d total primes.\n", primeCount);

    free(numbers);

    return 0;
}