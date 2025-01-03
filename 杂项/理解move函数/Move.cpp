#include <iostream>
#include <utility>

class MyClass {
public:
    // ���캯��
    MyClass(int value) : data(value) {
        std::cout << "��ͨ���캯��������" << std::endl;
    }
 
    MyClass(const MyClass& other) : data(other.data) {
        std::cout << "�������캯��������" << std::endl;
    }

    MyClass& operator=(const MyClass& other) {
        if (this != &other) {
            std::cout << "������ֵ�����������" << std::endl;
            data = other.data;
        }
        return *this;
    }


    // �ƶ����캯����ʵ���ƶ�����
    MyClass(MyClass&& other) noexcept : data(std::move(other.data)) {
        std::cout << "�ƶ����캯��������" << std::endl;
    }

    // �ƶ���ֵ�������ʵ���ƶ�����
    MyClass& operator=(MyClass&& other) noexcept {
        if (this != &other) {
            std::cout << "�ƶ���ֵ�����������" << std::endl;
            data = std::move(other.data);
        }
        return *this;
    }

    ~MyClass() {
        std::cout << "��������������" << std::endl;
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

    obj5 = std::move(obj1);  // ��������ƶ���ֵ�����

	obj1.Test();    

    return 0;
}