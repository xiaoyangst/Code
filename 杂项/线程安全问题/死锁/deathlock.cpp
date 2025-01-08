#include <iostream>
#include <thread>
#include <mutex>

std::mutex mutexA;
std::mutex mutexB;

void task1() {
    std::lock_guard<std::mutex> lockA(mutexA);
    std::this_thread::sleep_for(std::chrono::milliseconds(100)); // 模拟一些操作
    std::cout << "Task 1 locked mutexA" << std::endl;

    // 尝试获取 mutexB，导致等待
    std::lock_guard<std::mutex> lockB(mutexB);
    std::cout << "Task 1 locked mutexB" << std::endl;
}

void task2() {
    std::lock_guard<std::mutex> lockB(mutexB);
    std::this_thread::sleep_for(std::chrono::milliseconds(100)); // 模拟一些操作
    std::cout << "Task 2 locked mutexB" << std::endl;

    // 尝试获取 mutexA，导致等待
    std::lock_guard<std::mutex> lockA(mutexA);
    std::cout << "Task 2 locked mutexA" << std::endl;
}

int main() {
    std::thread t1(task1);
    std::thread t2(task2);

    t1.join();
    t2.join();

    std::cout << "Finished without deadlock (unlikely)" << std::endl;
    return 0;
}
