/*
Matrix Multiplication is one operation that would benefit 
if were designed in a concurrent way because of its independent operation. 
Consider the program below.

#include <iostream>

using namespace std;

int main()

{

    int m1[5][5] = {{1, 2, 3, 4, 5},

                    {6, 7, 8, 9, 10},

                    {11, 12, 13, 14, 15},

                    {16, 17, 18, 19, 20},

                    {21, 22, 23, 24, 25}};

    int m2[5][3] = {{26, 27, 28},

                    {29, 30, 31},

                    {32, 33, 34},

                    {35, 36, 37},

                    {38, 39, 40}};

    int result[5][3] = {{0}};

    for (int i = 0; i < 5; i++)

    {

        for (int j = 0; j < 3; j++)

        {

            for (int k = 0; k < 5; k++)
            {
                result[i][j] += m1[i][k] * m2[k][j];
            }

        }

    }

    for (int i = 0; i < 5; ++i) {

      for (int j = 0; j < 3; ++j) {

         cout << result[i][j] << " ";

         if (j == 2)

            cout << "\n";

      }

    }

    return 0;

}

Output:

510 525 540 
1310 1350 1390
2110 2175 2240
2910 3000 3090
3710 3825 3940

Design your own program that implements the matrix multiplication achieving the same output using multiple threads.
*/

#include <iostream>
#include <thread>

using namespace std;

/* Initializing a 2D array. */
int m1[5][5] = {{1, 2, 3, 4, 5},

                {6, 7, 8, 9, 10},

                {11, 12, 13, 14, 15},

                {16, 17, 18, 19, 20},

                {21, 22, 23, 24, 25}};

/* Initializing a 2D array. */
int m2[5][3] = {{26, 27, 28},

                {29, 30, 31},

                {32, 33, 34},

                {35, 36, 37},

                {38, 39, 40}};

/* Initializing the 2D array with 0. */
int result[5][3] = {{0}};

/**
 * Multiply the row i of m1 by the column j of m2 and store the result in result[i][j].
 * 
 * @param i the row of the first matrix
 * @param j the column of the result matrix
 */
void multiply(int i, int j) {
    for (int k = 0; k < 5; k++)
    {
        result[i][j] += m1[i][k] * m2[k][j];
    }
}

int main() {

    /* Creating 15 threads and passing the arguments to the function multiply. */
    thread t1(multiply, 0, 0);
    thread t2(multiply, 0, 1);
    thread t3(multiply, 0, 2);
    thread t4(multiply, 1, 0);
    thread t5(multiply, 1, 1);
    thread t6(multiply, 1, 2);
    thread t7(multiply, 2, 0);
    thread t8(multiply, 2, 1);
    thread t9(multiply, 2, 2);
    thread t10(multiply, 3, 0);
    thread t11(multiply, 3, 1);
    thread t12(multiply, 3, 2);
    thread t13(multiply, 4, 0);
    thread t14(multiply, 4, 1);
    thread t15(multiply, 4, 2);

    /* Waiting for the threads to finish. */
    t1.join();
    t2.join();
    t3.join();
    t4.join();
    t5.join();
    t6.join();
    t7.join();
    t8.join();
    t9.join();
    t10.join();
    t11.join();
    t12.join();
    t13.join();
    t14.join();
    t15.join();

    /* Printing the result matrix. */
    for (int i = 0; i < 5; ++i) {

      for (int j = 0; j < 3; ++j) {
         cout << result[i][j] << " ";
         if (j == 2)
            cout << "\n";
      }

    }

    return 0;
}