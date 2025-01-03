#include <iostream>
#include <utility>

class MyClass {
public:
    // 构造函数
    MyClass(int value) : data(value) {
        std::cout << "普通构造函数被调用" << std::endl;
    }
 
    MyClass(const MyClass& other) : data(other.data) {
        std::cout << "拷贝构造函数被调用" << std::endl;
    }

    MyClass& operator=(const MyClass& other) {
        if (this != &other) {
            std::cout << "拷贝赋值运算符被调用" << std::endl;
            data = other.data;
        }
        return *this;
    }


    // 移动构造函数，实现移动语义
    MyClass(MyClass&& other) noexcept : data(std::move(other.data)) {
        std::cout << "移动构造函数被调用" << std::endl;
    }

    // 移动赋值运算符，实现移动语义
    MyClass& operator=(MyClass&& other) noexcept {
        if (this != &other) {
            std::cout << "移动赋值运算符被调用" << std::endl;
            data = std::move(other.data);
        }
        return *this;
    }

    ~MyClass() {
        std::cout << "析构函数被调用" << std::endl;
    }

    void Test() {
        data = 30;
		std::cout << "data = " << data << std::endl;
    }
private:
    int data;
};

int main() {
    MyClass obj1(10);
    MyClass obj5(30);

    obj5 = std::move(obj1);  // 按理调用移动赋值运算符

	obj1.Test();    

    return 0;
}