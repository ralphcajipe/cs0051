// Sharing Code/Data and Mutual Exclusion
// Threaded version
// By Ralph Cajipe

/*
NOTE: This program is a modification of the original `lab3-origin.cpp` program.
To overcome the slowness, I split the data between the two threads and have each 
thread work on its own set of data. 

The new version of the program is using the code sharing technique for the threads, 
by dividing the data between the two threads and having each thread work on its own 
set of data. As a result, the two threads are not competing for the same resources 
and the performance is improved. Because the two threads are working on different 
sets of data, it seems there is little to no risk of race conditions.
*/

#include <iostream>
#include <vector>
#include <chrono>
#include <cstdlib>
#include <thread>

using namespace std;

// These are constants that are used in the program.
const long MAX = 500000000;
const int RANGE = 6;
const long HALF = MAX/2;

/* This is declaring a vector of integers with a size of MAX and an unsigned long long int with a value
of 0. */
vector<int> num(MAX);
unsigned long long int sum = 0;

// This is declaring 5 integers with a value of 0.
int h1=0;
int h2=0;
int h3=0;
int h4=0;
int h5=0;

/**
 * The function takes in two integers, start and end, and counts the number of times each number
 * appears in the array num between the indices start and end
 * 
 * @param start the starting index of the array
 * @param end the end of the array
 */
void count_histogram(int start, int end){
    for(int i=start; i < end; i++){
        switch(num[i]){
            case 1: h1++; break;
            case 2: h2++; break;
            case 3: h3++; break;
            case 4: h4++; break;
            case 5: h5++; break;
        }
    }
}

/**
 * The function get_sum() takes two arguments, start and end, and adds the numbers in the num array
 * from start to end
 * 
 * @param start the starting index of the array
 * @param end the index of the last element in the array
 */
void get_sum(int start, int end){
    for (int i = start; i < end; i++)
    {
        sum = sum + num[i];
    }
}

int main(){
    /* This is a for loop that is iterating through the array num and assigning a random number between
    0 and 5 to each element. */
    for (int i = 0; i < MAX; i++)
    {
        num[i] = rand() % RANGE;
    }
    // Getting the current time and storing it in the variable start.
    auto start = chrono::high_resolution_clock::now();

    /* This is creating two threads, t1 and t2, and assigning them to the functions count_histogram and
    get_sum. */
    thread t1(count_histogram, 0, HALF);
    thread t2(get_sum, HALF, MAX);

    // Wait for a thread to finish its execution.
    t1.join();
    t2.join();

    /* This is getting the current time and storing it in the variable end.
    Then, it is calculating the difference between the current time and the start time and storing
    it in the variable duration. */
    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);

    // Print out the histogram result.
    cout << "Histogram Result is : " << endl;
    cout << "1  ----> " << h1 << " SUM : " << h1 << endl;
    cout << "2  ----> " << h1 << " SUM : " << h2*2 << endl;
    cout << "3  ----> " << h1 << " SUM : " << h3*3 << endl;
    cout << "4  ----> " << h1 << " SUM : " << h4*4 << endl;
    cout << "5  ----> " << h1 << " SUM : " << h5*5 << endl;

    // This is calculating the sum of the histogram.
    unsigned long long int histogramsum = h1 + (h2*2) + (h3*3) + (h4*4) + (h5*5);
    
    /* This is printing out the sum of the vector and the sum of the histogram. It is also printing out
    the total running time of the program. */
    cout << "The sum of the vector is : " << sum << " and the sum of the histogram is " << histogramsum << endl;
    cout << "TOTAL RUNNING TIME IS : " << duration.count() << " milliseconds....";

    // Calculate the percentage of enhancement
    double original_time = 8854; /* insert the value of total run on the original program here */
    double enhancement = ((original_time - duration.count()) / original_time) * 100;
    cout << "The program has been enhanced by " << enhancement << "%" << endl;
    
    return 0;
}
