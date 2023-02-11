#include <iostream>
#include <thread>
#include <mutex>
#include <stack>
#include <condition_variable>
#include <chrono>
#include <random>

std::stack<int> stack;
std::mutex mtx;
std::condition_variable cv;
int sum = 0;
int total = 0;

/**
 * It pushes a random number to the stack, and then waits for 100 milliseconds
 * 
 * @param id The id of the producer.
 * 
 * @return A lambda function.
 */
void producer(int id)
{
    /* A random number generator that produces non-deterministic random numbers. */
    std::random_device rd;

    // mt19937 is a standard mersenne_twister_engine which means it is a good random number generator.
    std::mt19937 eng(rd());
    
    // uniform_int_distribution is a distribution that produces random integers uniformly on a given range.
    std::uniform_int_distribution<> distr(1, 10);

    for (int i = 0; i < 10000; i++)
    {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [] { return stack.size() < 10000; });
        int num = distr(eng);
        stack.push(num);
        total += num;
        std::cout << "Producer " << id << " pushed " << num << " to the stack." << std::endl;
        lock.unlock();
        cv.notify_all();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

/**
 * The consumer function waits until the stack is not empty, pops a number from the stack, adds it to
 * the sum, and then notifies all threads that the stack has changed
 * 
 * @param id The id of the consumer.
 * 
 * @return A lambda function.
 */
void consumer(int id)
{
    while (true)
    {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [] { return stack.size() > 0; });
        int num = stack.top();
        stack.pop();
        sum += num;
        std::cout << "Consumer " << id << " popped " << num << " from the stack." << std::endl;
        lock.unlock();
        cv.notify_all();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        if (sum == total)
            break;
    }
}

/**
 * The producer threads produce numbers and the consumer threads consume them.
 */
int main()
{
    /* Creating 4 threads as 2 producers and 2 consumers. */
    std::thread p1(producer, 1);
    std::thread p2(producer, 2);
    std::thread c1(consumer, 1);
    std::thread c2(consumer, 2);

    /* Waiting for the threads to finish. */
    p1.join();
    p2.join();
    c1.join();
    c2.join();

    std::cout << "Total: " << total << std::endl;
    std::cout << "Sum: " << sum << std::endl;
}

