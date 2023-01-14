#include <iostream>
#include <thread>

using namespace std;

int counter = 0;

void increment(){
    for (int i = 0; i < 100; i++){
        counter++;
    }
}

void decrement(){
    for (int i = 0; i < 100; i++){
        counter--;
    }
}

int main(){
    thread t1 (increment);
    thread t2(decrement);

    t1.join()
    t2.join()
    
    cout << counter << endl;
    return 0;
}