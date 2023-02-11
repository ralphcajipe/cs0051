/*
Topic: Conditional Variables in Parallel and Distributed Computing

Implement the Producer-Consumer Pattern in a bounded-buffer. 
The bounded-buffer of choice is a stack (std::stack). 
Make the buffer size (stack’s maximum capacity) to not to exceed having 10000 elements inside. 
This is your shared data.

Create a method called producer().
This method will generate a random number from 1 to 10,
and whatever number it generated it will be pushed into the stack buffer.
Sleep for a random time between (1 to 100ms) before adding another number again to the buffer do this for 10000 times. 

Create a method called consumer() this method will remove an item in the stack buffer and 
accumulate the removed(popped) value on a certain variable called SUM.  
Sleep for a random time between (1 to 100ms) before removing another number again from the buffer


Create your main method to produce four threads (2) Producers and (2) Consumer and 
take note of the following additional task:

1. Since there are 2 producer() threads that produces 10000 random number each 
for total of 20000 random numbers, there would come to a point (may or may not) 
that it would reach the maximum limit of our stack buffer which is 10000. 
Therefore a producer thread has to wait() before generating again a random number and 
placing it into the stack. 
Print “Stack Full Producer Waiting [thread-id]” when producer is waiting.

2. The 2 consumer threads will continuously remove an item from the stack, 
there (may or may not) come to a point where the stack buffer is empty thus 
the consumer thread has to wait until such time that the stack buffer has content. 
Print “Stack Empty Consumer Waiting [thread-id]”.

3. The program terminates when both producer finishes its job of adding random numbers 
into the stack and consumer fully consumes all of the numbers from the stack. 
You have to think of your own mechanism for detecting this one.

4. Display the total sum of all of the numbers before the program ends. 
(To determine if the sum is correct, create a temporary variable that 
accumulates too everytime the producer generates a number both of them should match 
at the end of the program, much like our previous laboratory). 
Display both values to check if the sum is correct.

Do not use legacy C++ threads, use modern C++ threads instead. 
Make the program as efficient as possible and easy to read.
*/

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

