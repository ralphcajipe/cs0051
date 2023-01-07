// Check thread support
#include <iostream>
#include <thread>

using namespace std;

int main()
{
    cout << "Hello, world!" << endl;
    cout << this_thread::get_id() << endl;
}