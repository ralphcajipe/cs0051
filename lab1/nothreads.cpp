/*
Create a C++ program named nothreads.cpp because there is no thread that prints the word

"I " 1000 times,
"GOT " 1000 times,
"NOBODY" 1000 times,
"BUT" 1000 times, 
"YOU" 1000 times 

sequentially no new lines in between using any loop
*/

#include <iostream>

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
    printI();
    printGOT();
    printNOBODY();
    printBUT();
    printYOU();

    return 0;
}
