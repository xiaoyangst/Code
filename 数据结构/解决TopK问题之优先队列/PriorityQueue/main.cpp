#include <iostream>
#include <string>
#include "PriorityQueue.h"

#include <cassert>

void testMaxHeap() {
    PriorityQueue<int> maxHeap;  // 默认最大堆
    maxHeap.push(10);
    maxHeap.push(20);
    maxHeap.push(5);
    maxHeap.push(30);

    assert(maxHeap.top() == 30);
    maxHeap.pop();
    assert(maxHeap.top() == 20);
    maxHeap.pop();
    assert(maxHeap.top() == 10);
    maxHeap.pop();
    assert(maxHeap.top() == 5);
    maxHeap.pop();
    assert(maxHeap.empty());

    std::cout << "MaxHeap test passed!" << std::endl;
}

void testMinHeap() {
    PriorityQueue<int, std::greater<>> minHeap; // 最小堆
    minHeap.push(10);
    minHeap.push(20);
    minHeap.push(5);
    minHeap.push(30);

    assert(minHeap.top() == 5);
    minHeap.pop();
    assert(minHeap.top() == 10);
    minHeap.pop();
    assert(minHeap.top() == 20);
    minHeap.pop();
    assert(minHeap.top() == 30);
    minHeap.pop();
    assert(minHeap.empty());

    std::cout << "MinHeap test passed!" << std::endl;
}

void testStringMinHeap() {
    PriorityQueue<std::string, std::greater<>> stringMinHeap;
    stringMinHeap.push("banana");
    stringMinHeap.push("apple");
    stringMinHeap.push("cherry");

    assert(stringMinHeap.top() == "apple");
    stringMinHeap.pop();
    assert(stringMinHeap.top() == "banana");
    stringMinHeap.pop();
    assert(stringMinHeap.top() == "cherry");
    stringMinHeap.pop();
    assert(stringMinHeap.empty());

    std::cout << "String MinHeap test passed!" << std::endl;
}

void testCustomComparator() {
    struct CustomCompare {
        bool operator()(int a, int b) const {
            return (a % 10) < (b % 10); // 按个位数排序
        }
    };

    PriorityQueue<int, CustomCompare> customHeap;
    customHeap.push(25);
    customHeap.push(14);
    customHeap.push(32);
    customHeap.push(47);

    assert(customHeap.top() == 47); // 个位数最大的数
    customHeap.pop();
    assert(customHeap.top() == 25);
    customHeap.pop();
    assert(customHeap.top() == 14);
    customHeap.pop();
    assert(customHeap.top() == 32);
    customHeap.pop();
    assert(customHeap.empty());

    std::cout << "Custom Comparator test passed!" << std::endl;
}

int main() {
    testMaxHeap();
    testMinHeap();
    testStringMinHeap();
    testCustomComparator();
    std::cout << "All tests passed!" << std::endl;
    return 0;
}
