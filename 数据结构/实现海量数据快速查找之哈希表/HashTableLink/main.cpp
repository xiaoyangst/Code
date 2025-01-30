#include "HashTable.h"
#include <iostream>
#include <string>

int main() {
    HashTableLink<int, std::string> hashTable;

    // 插入测试
    hashTable.insert(1, "One");
    hashTable.insert(2, "Two");
    hashTable.insert(3, "Three");
    hashTable.insert(4, "Four");
    hashTable.insert(5, "Five");

    std::cout << "初始哈希表：" << std::endl;
    hashTable.print();

    // 查找测试
    std::string val;
    if (hashTable.find(3, val)) {
        std::cout << "找到键 3，对应值：" << val << std::endl;
    } else {
        std::cout << "未找到键 3" << std::endl;
    }

    // 删除测试
    hashTable.remove(2);
    std::cout << "删除键 2 后的哈希表：" << std::endl;
    hashTable.print();

    // 触发 rehash
    for (int i = 6; i <= 15; ++i) {
        hashTable.insert(i, "Value" + std::to_string(i));
    }
    std::cout << "rehash 后的哈希表：" << std::endl;
    hashTable.print();

    return 0;
}
