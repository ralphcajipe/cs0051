/*
Create a C++ program named withthreads.cpp that does the same but each word is on a separate thread.
*/

#include <iostream>
#include <thread>

using namespace std;

void printI() {
    for (int i = 0; i < 1000; i++) {
        cout << "I ";
    }
}

void printGOT() {
    for (int i = 0; i < 1000; i++) {
        cout << "GOT ";
    }
}

void printNOBODY() {
    for (int i = 0; i < 1000; i++) {
        cout << "NOBODY ";
    }
}

void printBUT() {
    for (int i = 0; i < 1000; i++) {
        cout << "BUT ";
    }
}

void printYOU() {
    for (int i = 0; i < 1000; i++) {
        cout << "YOU ";
    }
}

int main() {
    thread t1(printI);
    thread t2(printGOT);
    thread t3(printNOBODY);
    thread t4(printBUT);
    thread t5(printYOU);

    t1.join();
    t2.join();
    t3.join();
    t4.join();
    t5.join();

    return 0;
}
