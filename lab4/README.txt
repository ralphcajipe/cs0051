1. Implement Producer-Consumer Pattern in a bounded buffer. The bounded buffer of choice is a stack (std::stack). Make the buffer size (stack’s maximum capacity) not to exceed having 10000 elements inside. This is your shared data.

2. Create a method called producer(). This method will generate a random number from 1 to 10, and whatever number it generated, will be pushed into the stack buffer. Sleep for a random time between (1 to 100 ms) before adding another number again to the buffer do this 10000 times. 

3. Create a method called consumer(). This method will remove an item in the stack buffer and 
accumulate the removed (popped) value on a certain variable called SUM.  Sleep for a random time between (1 to 100 ms) before removing another number again from the buffer.

4. Create a main method to produce four threads (2) Producers and (2) Consumer and 
take note of the following additional tasks:

5. Since there are 2 producer() threads that produce 10000 random numbers each for a total of 20000 random numbers, there would come to a point (may or may not) that it would reach the maximum limit of our stack buffer which is 10000. Therefore a producer thread has to wait() before generating again a random number and placing it into the stack. 
It must print “Stack Full Producer Waiting [thread-id]” when the producer is waiting.

6. The 2 consumer threads will continuously remove an item from the stack, there (may or may not) come to a point where the stack buffer is empty thus the consumer thread has to wait until such time that the stack buffer has content. It must print “Stack Empty Consumer Waiting [thread-id]”.

7. The program terminates when both producers finish their job of adding random numbers 
into the stack and the consumer fully consumes all of the numbers from the stack. You have to implement a mechanism for detecting this one.

8. Display the "Total Sum" of all of the numbers before the program ends. To determine if the sum is correct, create a temporary variable "Correct/Expected Sum" that accumulates too every time the producer generates a number both of them should match at the end of the program. Display both values to check if the sum is correct.