// Sharing Code/Data and Mutual Exclusion
// Improved version 2

#include <iostream>
#include <vector>
#include <chrono>
#include <cstdlib>
#include <thread>

using namespace std;

const long MAX = 500000000;
const int RANGE = 6;

vector<int> num(MAX);
unsigned long long int sum = 0;
int histogram[RANGE] = {0};

void count_histogram(){
    for(int i=0; i < MAX; i++){
        histogram[num[i]]++;
    }
}

void get_sum(){
    for (int i = 0; i < MAX; i++)
    {
        sum = sum + num[i];
    }
}

int main(){
    for (int i = 0; i < MAX; i++)
    {
        num[i] = rand() % RANGE;
    }
    auto start = chrono::high_resolution_clock::now();

    thread t1(count_histogram);
    thread t2(get_sum);

    t1.join();
    t2.join();

    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);

    cout << "Histogram Result is : " << endl;
    for(int i = 0; i < RANGE; i++){
        cout << i << "  ----> " << histogram[i] << " SUM : " << histogram[i]*i << endl;
    }

    unsigned long long int histogramsum = 0;
    for(int i = 0; i < RANGE; i++){
        histogramsum += histogram[i]*i;
    }
    cout << "The sum of the vector is : " << sum << " and the sum of the histogram is " << histogramsum << endl;
    cout << "The time taken is : " << duration.count() << " milliseconds" << endl;

     // calculate the percentage of enhancement
    double original_time = 8854;
    double enhancement = ((original_time - duration.count()) / original_time) * 100;
    cout << "The program has been enhanced by " << enhancement << "%" << endl;

    return 0;
