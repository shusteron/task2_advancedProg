#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>

#define NUM_THREADS 16
#define CHUNK_SIZE 128 

pthread_mutex_t queueMutex;  // Mutex for protecting access to the queue
pthread_cond_t queueCond;    // Condition variable for signaling the queue
pthread_mutex_t primeCountMutex;  // Mutex for protecting access to the prime count
int primeCount = 0;  // Global counter for prime numbers

// Structure representing a chunk of numbers
typedef struct {
    int numbers[CHUNK_SIZE];
    int size;
} Chunk;

// Node structure for the linked list queue
typedef struct Node {
    Chunk chunk;
    struct Node *next;
} Node;

// Queue structure for holding chunks
typedef struct {
    Node *head;
    Node *tail;
    int size;
} Queue;

Queue queue = {NULL, NULL, 0};  // Initialize an empty queue

// Optimized isPrime function
bool isPrime(int n) {
    if (n <= 1) return false;  // 0 and 1 are not prime numbers
    if (n <= 3) return true;   // 2 and 3 are prime numbers
    if (n % 2 == 0 || n % 3 == 0) return false;  // Eliminate multiples of 2 and 3

    // Check for factors from 5 onwards, skipping even numbers
    for (int i = 5; i * i <= n; i += 6) {
        if (n % i == 0 || n % (i + 2) == 0) return false;
    }
    return true;
}

// Function to enqueue a chunk into the queue
void enqueue(Chunk chunk) {
    Node *newNode = (Node *)malloc(sizeof(Node));  // Allocate memory for a new node
    newNode->chunk = chunk;
    newNode->next = NULL;

    pthread_mutex_lock(&queueMutex);  // Lock the queue mutex
    if (queue.tail) {
        queue.tail->next = newNode;
    } else {
        queue.head = newNode;
    }
    queue.tail = newNode;
    queue.size++;
    pthread_cond_signal(&queueCond);  // Signal that a new item is added to the queue
    pthread_mutex_unlock(&queueMutex);  // Unlock the queue mutex
}

// Function to dequeue a chunk from the queue
Chunk dequeue() {
    pthread_mutex_lock(&queueMutex);  // Lock the queue mutex
    while (queue.size == 0) {
        pthread_cond_wait(&queueCond, &queueMutex);  // Wait for an item to be enqueued
    }

    Node *node = queue.head;
    queue.head = node->next;
    if (!queue.head) {
        queue.tail = NULL;
    }
    queue.size--;

    Chunk chunk = node->chunk;
    free(node);  // Free the memory of the dequeued node
    pthread_mutex_unlock(&queueMutex);  // Unlock the queue mutex
    return chunk;
}

// Function executed by each thread to process numbers
void *processNumbers(void *arg) {
    while (1) {
        Chunk chunk = dequeue();  // Dequeue a chunk of numbers
        if (chunk.size == 0) break;  // End signal

        int localPrimeCount = 0;
        for (int i = 0; i < chunk.size; ++i) {
            if (isPrime(chunk.numbers[i])) {
                ++localPrimeCount;
            }
        }

        pthread_mutex_lock(&primeCountMutex);  // Lock the prime count mutex
        primeCount += localPrimeCount;
        pthread_mutex_unlock(&primeCountMutex);  // Unlock the prime count mutex
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

    // Enqueue end signals to stop all threads
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
