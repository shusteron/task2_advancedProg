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


//--------------------------------------------------------------------------------------------------------------


#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>

#define NUM_THREADS 16
#define CHUNK_SIZE 128 // Further reduced chunk size

pthread_mutex_t queueMutex;
pthread_cond_t queueCond;
pthread_mutex_t primeCountMutex;
int primeCount = 0;

typedef struct {
    int numbers[CHUNK_SIZE];
    int size;
} Chunk;

typedef struct Node {
    Chunk chunk;
    struct Node *next;
} Node;

typedef struct {
    Node *head;
    Node *tail;
    int size;
} Queue;

Queue queue = {NULL, NULL, 0};

bool isPrime(int n) {
    if (n <= 1) {
        return false;
    }
    for (int i = 2; i * i <= n; i++) {
        if (n % i == 0) {
            return false;
        }
    }
    return true;
}

void enqueue(Chunk chunk) {
    Node *newNode = (Node *)malloc(sizeof(Node));
    newNode->chunk = chunk;
    newNode->next = NULL;

    pthread_mutex_lock(&queueMutex);
    if (queue.tail) {
        queue.tail->next = newNode;
    } else {
        queue.head = newNode;
    }
    queue.tail = newNode;
    queue.size++;
    pthread_cond_signal(&queueCond);
    pthread_mutex_unlock(&queueMutex);
}

Chunk dequeue() {
    pthread_mutex_lock(&queueMutex);
    while (queue.size == 0) {
        pthread_cond_wait(&queueCond, &queueMutex);
    }

    Node *node = queue.head;
    queue.head = node->next;
    if (!queue.head) {
        queue.tail = NULL;
    }
    queue.size--;

    Chunk chunk = node->chunk;
    free(node);
    pthread_mutex_unlock(&queueMutex);
    return chunk;
}

void *processNumbers(void *arg) {
    while (1) {
        Chunk chunk = dequeue();
        if (chunk.size == 0) break; // End signal

        int localPrimeCount = 0;
        for (int i = 0; i < chunk.size; ++i) {
            if (isPrime(chunk.numbers[i])) {
                ++localPrimeCount;
            }
        }

        pthread_mutex_lock(&primeCountMutex);
        primeCount += localPrimeCount;
        pthread_mutex_unlock(&primeCountMutex);
    }
    return NULL;
}

int main() {
    pthread_mutex_init(&queueMutex, NULL);
    pthread_cond_init(&queueCond, NULL);
    pthread_mutex_init(&primeCountMutex, NULL);

    pthread_t threads[NUM_THREADS];
    for (unsigned int i = 0; i < NUM_THREADS; ++i) {
        pthread_create(&threads[i], NULL, processNumbers, NULL);
    }

    int number;
    Chunk chunk = {{0}, 0};
    while (scanf("%d", &number) != EOF) {
        chunk.numbers[chunk.size++] = number;
        if (chunk.size == CHUNK_SIZE) {
            enqueue(chunk);
            chunk.size = 0;
        }
    }
    if (chunk.size > 0) {
        enqueue(chunk);
    }

    // Enqueue end signals
    for (unsigned int i = 0; i < NUM_THREADS; ++i) {
        Chunk endSignal = {{0}, 0};
        enqueue(endSignal);
    }

    for (unsigned int i = 0; i < NUM_THREADS; ++i) {
        pthread_join(threads[i], NULL);
    }

    pthread_mutex_destroy(&queueMutex);
    pthread_cond_destroy(&queueCond);
    pthread_mutex_destroy(&primeCountMutex);

    printf("%d total primes.\n", primeCount);

    return 0;
}