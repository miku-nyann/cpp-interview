#include <thread>
#include <iostream>
#include <atomic>

using namespace std;

void data_race() {
    int sum = 0;
    auto adding = [&]() {
        for (int i = 0; i < 100000; ++i)
            ++sum;
    };
    thread t1(adding);
    thread t2(adding);
    t1.join();
    t2.join();
    cout << sum << endl;
}

void use_atomic_to_avoid_data_race() {
    atomic<int> sum = 0;
    auto adding = [&]() {
        for (int i = 0; i < 100000; ++i)
            sum.fetch_add(1, memory_order_relaxed);
    };
    thread t1(adding);
    thread t2(adding);
    t1.join();
    t2.join();
    cout << sum << endl;
}

int main() {
    data_race();
    use_atomic_to_avoid_data_race();
    return 1;
} 
