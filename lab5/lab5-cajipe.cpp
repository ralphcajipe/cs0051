/*
 * Lab 5 Semaphores
 * Ralph Cajipe
 * This program demonstrates the producer-consumer problem using semaphores.
 *
 * Here are some bullet points about the program:
 * - The program simulates concurrent access to a shared buffer using a stack, two producer threads,
 *   and two consumer threads.
 *   
 * - The program uses POSIX (Portable Operating System Interface) semaphores to synchronize access to the shared buffer
 *   and block threads when the buffer is full or empty.
 *   
 * - The program generates random integers, pushes them onto the buffer, and calculates their sum when consumed.
 * 
 * - The program prints messages to indicate when threads are pushing or popping integers and when they are waiting.
 * 
 * - The program checks whether the expected sum of integers produced is equal to the sum
 *   of integers consumed and prints a message indicating whether the sum is correct.
 */

#include <iostream>
#include <stack>
#include <thread>
#include <chrono>
#include <random>
#include <semaphore.h>

// The buffer is a stack that holds integers.
std::stack<int> buffer;

// The maximum size of the buffer.
const int max_size = 10000;

/*
The semaphores used for synchronization.
In this code, three semaphores are used to synchronize access to the buffer: mutex, empty, and full.
The mutex semaphore is used to protect the critical section where the buffer is accessed.
The empty semaphore is used to block the producer when the buffer is full,
and the full semaphore is used to block the consumer when the buffer is empty.
*/
sem_t mutex, empty, full;

// The sum of all the elements popped from the buffer.
int sum = 0;

// The sum of all the elements expected to be in the buffer.
int expected_sum = 0;

// A boolean flag that indicates when the producers are done.
bool producer_done = false;


// The producer function that generates random integers and pushes them onto the buffer.
void producer(int id)
{
    // Create a random number generator using the current time.
    std::mt19937 rng(std::chrono::system_clock::now().time_since_epoch().count());

    // Create a uniform distribution that generates integers between 1 and 10.
    std::uniform_int_distribution<int> dist(1, 10);

    // Loop for half the maximum buffer size.
    for (int i = 0; i < max_size / 2; i++)
    {
        // Generate a random integer between 1 and 10.
        int num = dist(rng);

        // Update the expected sum.
        expected_sum += num;

        // Wait on the empty semaphore, which blocks the producer if the buffer is full.
        int ret = sem_wait(&empty);
        if (ret == -1)
        {
            std::cout << "Stack Full Producer Waiting [" << id << "]" << std::endl;
            sem_wait(&empty);
        }

        // Wait on the mutex semaphore to protect the critical section where the buffer is accessed.
        ret = sem_wait(&mutex);
        if (ret == -1)
        {
            std::cout << "Mutex Wait Producer [" << id << "]" << std::endl;
            sem_wait(&mutex);
        }

        // Push the generated integer onto the buffer.
        buffer.push(num);

        // Release the mutex semaphore.
        sem_post(&mutex);

        // Signal the full semaphore to wake up the consumers.
        sem_post(&full);

        // Print a message indicating the generated integer was pushed onto the buffer.
        std::cout << "Producer " << id << " pushed " << num << " to the stack." << std::endl;
    }

    // Set the producer_done flag to true.
    producer_done = true;

    // Signal the full semaphore to wake up the consumers.
    sem_post(&full);
}

// The consumer function that pops integers from the buffer and adds them to the sum.
void consumer(int id)
{
    // Loop until the producers are done and the buffer is empty.
    while (true)
    {
        // Wait on the full semaphore, which blocks the consumer if the buffer is empty.
        // The sem_wait function is used to wait for a semaphore to become available
        int ret = sem_wait(&full);
        if (ret == -1)
        {
            std::cout << "Stack Empty Consumer Waiting [" << id << "]" << std::endl;
            sem_wait(&full);
        }

        // Wait on the mutex semaphore to protect the critical section where the buffer is accessed.
        ret = sem_wait(&mutex);
        if (ret == -1)
        {
            std::cout << "Mutex Wait Consumer [" << id << "]" << std::endl;
            sem_wait(&mutex);
        }

        // If the buffer is empty and the producers are done, break out of the loop.
        if (buffer.empty())
        {
            // The sem_post function is used to signal that the semaphore is available.
            sem_post(&mutex);
            sem_post(&full);
            if (producer_done) {
                break;
            }
            continue;
        }

        // Pop the top integer from the buffer and add it to the sum.
        int num = buffer.top();
        sum += num;
        buffer.pop();

        // Release the mutex semaphore.
        sem_post(&mutex);

        // Signal the empty semaphore to wake up the producers.
        sem_post(&empty);

        // Print a message indicating the popped integer was removed from the buffer.
        std::cout << "Consumer " << id << " popped " << num << " from the stack." << std::endl;
    }
}

// The main function initializes the semaphores, creates the producer and consumer threads,
// and waits for them to finish.
int main()
{
    // Getting the current time and storing it in the variable start.
    std::chrono::time_point<std::chrono::system_clock> start, end;
    start = std::chrono::system_clock::now();

    /* The variable `ret` is used to store the return value of the semaphore function `sem_init()`.
     * Always check the return value of semaphore functions and to handle any errors that may occur.
     */
    int ret;

    // Initialize the semaphores.
    ret = sem_init(&mutex, 0, 1);
    if (ret == -1) {
        std::cout << "Semaphore mutex initialization failed" << std::endl;
        return -1;
    }
    ret = sem_init(&empty, 0, max_size);
    if (ret == -1) {
        std::cout << "Semaphore empty initialization failed" << std::endl;
        return -1;
    }
    ret = sem_init(&full, 0, max_size);
    if (ret == -1) {
        std::cout << "Semaphore full initialization failed" << std::endl;
        return -1;
    }

    // Create the producer and consumer threads.
    std::thread producer1(producer, 1);
    std::thread producer2(producer, 2);
    std::thread consumer1(consumer, 1);
    std::thread consumer2(consumer, 2);

    // Wait for the threads to finish.
    producer1.join();
    producer2.join();
    consumer1.join();
    consumer2.join();

    std::cout << "=====================================" << std::endl;

    // When finished, destroy the semaphores in order to avoid resource leaks and potential problems.
    sem_destroy(&mutex);
    sem_destroy(&empty);
    sem_destroy(&full);

    // Print the sum and expected sum.
    std::cout << "Total Sum: " << sum << std::endl;
    std::cout << "Expected Sum: " << expected_sum << std::endl;

    // Print a message indicating whether the sum is correct or not.
    if (sum == expected_sum)
        std::cout << ":) The sum is correct." << std::endl;
    else
        std::cout << ":( The sum is incorrect." << std::endl;

    // Printing out the time that the program finished running and the amount of time it took to run.
    end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = end - start;
    std::time_t end_time = std::chrono::system_clock::to_time_t(end);
    std::cout << "Finished computation at " << std::ctime(&end_time)
              << "Elapsed time: " << elapsed_seconds.count() << "s\n";

    // If the semaphore object was created and initialized successfully
    return 0;
}
