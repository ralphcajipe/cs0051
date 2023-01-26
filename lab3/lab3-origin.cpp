// Sharing Code/Data and Mutual Exclusion
// Original version

/*
NOTE:
The original program is not fast enough because it is performing two separate sequential loops, 
one for calculating the sum of the vector, and another for counting the histogram of the vector.
Both of these operations have a time complexity of O(n), where n is the size of the vector. 
As a result, the total time complexity of the program is O(n) + O(n) = O(2n), 
which can take a long time to run with a large vector size of 500000000. 
Additionally, the program is not utilizing any parallelism or multi-threading, 
which could potentially speed up the execution time.
*/

#include <iostream>
#include <vector>
#include <chrono>
#include <cstdlib>

using namespace std;

int main()
{
    const long MAX = 500000000;
    const int RANGE = 6;
    
    vector<int> num(MAX);
    unsigned long long int sum = 0;
    int h1=0;
    int h2=0;
    int h3=0;
    int h4=0;
    int h5=0;

    for (int i = 0; i < MAX; i++)
    {
        num[i] = rand() % RANGE;

        //cout << num[i] << endl; //generates random numbers 1 to 5
    }

    auto start = chrono::high_resolution_clock::now();

    for (int i = 0; i < MAX; i++)
    {
        sum = sum + num[i];
    }

    for(int i=0; i < MAX; i++)
    {
        switch(num[i])
        {
            case 1: h1++; break;
            case 2: h2++; break;
            case 3: h3++; break;
            case 4: h4++; break;
            case 5: h5++; break;
        }
    }

    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);

    cout << "Histogram Result is : " << endl;
    cout << "1  ----> " << h1 << " SUM : " << h1 << endl;
    cout << "2  ----> " << h1 << " SUM : " << h2*2 << endl;
    cout << "3  ----> " << h1 << " SUM : " << h3*3 << endl;
    cout << "4  ----> " << h1 << " SUM : " << h4*4 << endl;
    cout << "5  ----> " << h1 << " SUM : " << h5*5 << endl;

    unsigned long long int histogramsum = h1 + (h2*2) + (h3*3) + (h4*4) + (h5*5); 
    cout << "The sum of the vector is : " << sum << " and the sum of the histogram is " << histogramsum << endl;
    cout << "TOTAL RUNNING TIME IS : " << duration.count() << " milliseconds....";

    return 0;
}
