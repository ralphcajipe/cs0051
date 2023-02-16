/*
 * Lab 5 Semaphores
 * Ralph Cajipe
 * This program demonstrates the producer-consumer problem using semaphores.
 *
 * The modifications to the code include:
 *   - Changing the loop limit in the `producer` function to `max_size / 2` to ensure that each thread produces `max_size` elements in total.
 *   - Adding a boolean flag `producer_done` to signal when the producer threads have finished pushing data into the buffer.
 *   - Modifying the initialization of the `full` semaphore to `max_size`.
 *   - Adding a check for the `producer_done` flag in the `consumer` function to avoid deadlock.
 *   - Adding more print statements to help with debugging.
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

// The semaphores used for synchronization.
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

    // Destroy the semaphores.
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

    return 0;
}
