#include <iostream>
#include "BSTMap.h"

int main() {
    // 创建 BSTSearch 对象
    BSTMap<int, std::string> bst;

    // 插入数据
    bst.insert(4, "Node4");
    bst.insert(8, "Node8");
    bst.insert(20, "Node20");
    bst.insert(22, "Node22");
    bst.insert(12, "Node12");
    bst.insert(10, "Node10");
    bst.insert(14, "Node14");
    bst.insert(1, "Node1");

    // 查找数据
    auto node = bst.find(4);
    if (node) {
        std::cout << "Found node with key 4: " << node->data.second << std::endl;
    } else {
        std::cout << "Node with key 4 not found." << std::endl;
    }

    // 打印所有节点
    bst.Print(bst.getroot());
    std::cout << std::endl;

    {   // 删除叶子节点
        std::cout << "删除叶子节点" << std::endl;
        bst.erase(1);
        bst.Print(bst.getroot());
        std::cout << std::endl;
    }

    {   // 删除只有单个子节点
        std::cout << "删除只有单个子节点" << std::endl;
        bst.erase(8);
        bst.Print(bst.getroot());
        std::cout << std::endl;
    }

    {   // 删除有两个子节点
        std::cout << "删除有两个子节点" << std::endl;
        bst.erase(12);
        bst.Print(bst.getroot());
        std::cout << std::endl;
    }

    {   // 删除只有单个子节点，根节点
        std::cout << "删除只有单个子节点" << std::endl;
        bst.erase(4);
        bst.Print(bst.getroot());
        std::cout << std::endl;
    }

    {   // 删除有两个子节点，根节点
        std::cout << "删除有两个子节点" << std::endl;
        bst.erase(20);
        bst.Print(bst.getroot());
        std::cout << std::endl;
    }

    bst.erase(14);
    bst.Print(bst.getroot());
    std::cout << std::endl;
    bst.erase(22);
    bst.Print(bst.getroot());
    std::cout << std::endl;
    bst.erase(10);  // 删除最后一个节点
    bst.Print(bst.getroot());
    std::cout << std::endl;

    return 0;
}
