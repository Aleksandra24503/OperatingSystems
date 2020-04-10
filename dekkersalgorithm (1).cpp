#include <iostream>
#include <string>
#include <thread>

using namespace std;

volatile bool want_to_enter[2];
volatile int turn;
string text = "The quick brown fox jumps over the lazy dog\n";

void f(int nthread)
{
    int second_thread = 1 - nthread;

    want_to_enter[nthread] = true;
    while (want_to_enter[second_thread])
    {
        if (turn == second_thread)
        {
            want_to_enter[nthread] = false;
            while (turn != nthread);
            want_to_enter[nthread] = true;
        }
    }

    for (auto c: text)
        cout << c;

    turn = second_thread;
    want_to_enter[nthread] = false;
}

int main()
{

    std::thread t0(f, 0);
    std::thread t1(f, 1);

    t0.join();
    t1.join();

    return 0;
}
