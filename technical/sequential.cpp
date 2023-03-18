/*
Filename: sequential.cpp
Author: Ralph Cajipe
Date: March 18, 2023
Description: This program adds up the elements in an array using a sequential approach. 
The array is initialized with a repeating pattern of 0's and 1's, and the sum of the array 
is calculated using a for loop and a function that iterates through the array.

Functions:
int array_sum(int* arr, int size): calculates the sum of an array

Variables:
    SIZE: constant integer that represents the size of the array
    NUM_ITERATIONS: constant integer that represents the number of iterations to perform
    arr: integer pointer that dynamically allocates the array
    sum: integer that holds the sum of the array

Compilation: 
    g++ sequential.cpp -o sequential.
*/

#include <iostream>
#include <chrono>

using namespace std;

// Function to calculate the sum of an array
int array_sum(int* arr, int size) {
    int sum = 0;
    for(int i = 0; i < size; i++) {
        sum += arr[i];
    }
    return sum;
}

int main() {
    const int SIZE = 100000000; // Size of the array
    const int NUM_ITERATIONS = 100; // Number of iterations to perform
    int* arr = new int[SIZE]; // Dynamically allocate the array

    // Initialize the array with fixed values
    for(int i = 0; i < SIZE; i++) {
        arr[i] = i % 2; // use a repeating pattern of 0 and 1
    }

    // Measure the time taken for sequential execution
    auto start_time = chrono::high_resolution_clock::now();

    // Perform multiple iterations of the summing operation
    int sum = 0;
    for(int i = 0; i < NUM_ITERATIONS; i++) {
        sum = array_sum(arr, SIZE);
    }

    auto end_time = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(end_time - start_time).count();

    // Display the sum and time taken
    cout << "Sum: " << sum << endl;
    cout << "Time taken (sequential): " << duration << " microseconds" << endl;
    
    // Free the dynamically allocated memory
    delete[] arr;

    return 0;
}
