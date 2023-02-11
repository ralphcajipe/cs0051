// Lab 4 - Conditional Variables
// Ralph Cajipe

// Importing the necessary libraries.
#include <iostream>
#include <thread>
#include <mutex>
#include <stack>
#include <condition_variable>
#include <chrono>
#include <random>

// Declaring a stack of integers, a mutex, and a condition variable.
std::stack<int> stack;
std::mutex mtx;
std::condition_variable cv;

// Declaring two global variables
int sum = 0;
int expected_sum = 0;

/**
 * It creates a random number generator, then loops 10,000 times, each time pushing a random number
 * onto the stack and adding that number to the expected sum
 *
 * @param id The id of the producer thread.
 */
void producer(int id)
{
    std::random_device rd;
    std::mt19937 eng(rd());
    std::uniform_int_distribution<> distr(1, 10);

    for (int i = 0; i < 10000; i++)
    {
        std::unique_lock<std::mutex> lock(mtx);

        if (stack.size() == 10000)
        {
            std::cout << "Stack Full Producer Waiting " << id << std::endl;
            cv.wait(lock);
        }

        int num = distr(eng);
        stack.push(num);
        expected_sum += num;
        std::cout << "Producer " << id << " pushed " << num << " to the stack." << std::endl;
        lock.unlock();
        cv.notify_all();
        //std::uniform_int_distribution<> sleep_distr(1, 100);
        //std::this_thread::sleep_for(std::chrono::milliseconds(sleep_distr(eng)));
    }
}

/**
 * The consumer function takes an integer id as a parameter and then creates a random number generator.
 * It then enters an infinite loop. Inside the loop, it locks the mutex, checks if the stack is empty,
 * and if it is, it waits for the condition variable to be notified. If the stack is not empty, it pops
 * the top element from the stack, adds it to the sum, and unlocks the mutex. It then notifies the
 * condition variable and sleeps for a random amount of time. If the stack is empty, it breaks out of
 * the loop
 *
 * @param id The id of the consumer.
 */
void consumer(int id)
{
    std::random_device rd;
    std::mt19937 eng(rd());

    while (true)
    {
        std::unique_lock<std::mutex> lock(mtx);

        if (stack.empty())
        {
            std::cout << "Stack Empty Consumer Waiting " << id << std::endl;
            cv.wait(lock);
        }

        int num = stack.top();
        stack.pop();
        sum += num;
        std::cout << "Consumer " << id << " popped " << num << " from the stack." << std::endl;
        lock.unlock();
        cv.notify_all();
		//std::uniform_int_distribution<> sleep_distr(1, 100);
        //std::this_thread::sleep_for(std::chrono::milliseconds(sleep_distr(eng)));
        if (stack.empty())
            break;
    }
}

/**
 * It creates two producer threads and two consumer threads, and then waits for them to finish
 *
 * @return The program is returning the sum of all the numbers from 1 to 1000000.
 */
int main()
{
    // Getting the current time and storing it in the variable start.
    std::chrono::time_point<std::chrono::system_clock> start, end;
    start = std::chrono::system_clock::now();

    // Creating 4 threads: 2 producer threads and 2 consumer threads.
    std::thread p1(producer, 1);
	std::thread p2(producer, 2);
	std::thread c1(consumer, 1);
	std::thread c2(consumer, 2);

	// Waiting for the threads to finish.
    p1.join();
	p2.join();
	c1.join();
	c2.join();

    // Printing out the expected sum and the actual sum.
    std::cout << "=========================================" << std::endl;
    std::cout << "Expected Sum: " << expected_sum << std::endl;
	std::cout << "Actual Sum: " << sum << std::endl;

    /* Checking if the expected sum and the actual sum are the same. If they are, it prints out a happy
    face. If they are not, it prints out a sad face. */
    if (expected_sum == sum)
	    std::cout << ":) Expected sum and actual sum match!" << std::endl;
	else
	    std::cout << ":( Expected sum and actual sum do not match!" << std::endl;

    // Printing out the time that the program finished running and the amount of time it took to run.
    end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = end - start;
    std::time_t end_time = std::chrono::system_clock::to_time_t(end);
    std::cout << "Finished computation at " << std::ctime(&end_time)
              << "Elapsed time: " << elapsed_seconds.count() << "s\n";

	return 0;
}
