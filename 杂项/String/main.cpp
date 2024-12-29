#include "MyString.h"

// 测试函数
void testMyString() {
	MyString s1("Hello");
	MyString s2("World");

	MyString s3 = s1 + s2; // 测试 operator+
	cout << "s3: " << s3.size() << " " << s3 << " characters" << endl;

	MyString s4(s3); // 测试拷贝构造
	cout << "s4: " << s4.size() << " " << s4 << " characters" << endl;

	MyString s5;
	s5 = s1; // 测试拷贝赋值
	cout << "s5: " << s5.size() << " " << s5 << " characters" << endl;

	cout << "s1 == s5? " << (s1 == s5 ? "Yes" : "No") << endl; // 测试 operator==

	MyString s6 = std::move(s2); // 测试移动构造
	cout << "s6: " << s6.size() << " " << s6 << " characters" << endl;
	cout << "s2: " << s2.size() << " " << s2 << " characters" << endl;

	MyString s7;
	s7 = std::move(s3); // 测试移动赋值
	cout << "s7: " << s7.size() << " " << s7 << " characters" << endl;
	cout << "s3: " << s3.size() << " " << s3 << " characters" << endl;
}


void TestMyString() {
    // 测试默认构造函数
    MyString s1;
    cout << "s1: " << (s1.size() == 0 ? "Empty" : "Not Empty") << endl;

    // 测试传入空指针的构造函数
    MyString s2(nullptr);
    cout << "s2: " << (s2.size() == 0 ? "Empty" : "Not Empty") << endl;

    // 测试普通字符串构造
    MyString s3("Hello");
    cout << "s3: " << s3.size() << " " << s3 << " characters" << endl;

    // 测试 operator+ (拼接两个字符串)
    MyString s4 = s3 + MyString(" World");
    cout << "s4: " << s4.size() << " " << s4 << " characters" << endl;

    // 测试 operator+ (拼接空字符串)
    MyString s5 = s3 + MyString("");
    cout << "s5: " << s5.size() << " " << s5 << " characters" << endl;

    // 测试 operator+ (拼接 null 字符串)
    MyString s6 = s3 + MyString(nullptr);
    cout << "s6: " << s6.size() << " " << s6 << " characters" << endl;

    // 测试拷贝构造
    MyString s7(s4);
    cout << "s7: " << s7.size() << " " << s7 << " characters" << endl;

    // 测试拷贝赋值
    MyString s8;
    s8 = s4;
    cout << "s8: " << s8.size() << " " << s8 << " characters" << endl;

    // 测试拷贝赋值和空字符串的赋值
    MyString s9;
    s9 = MyString(""); // 赋值空字符串
    cout << "s9: " << (s9.size() == 0 ? "Empty" : "Not Empty") << endl;

    // 测试相等操作符 (相等)
    MyString s10("Hello");
    cout << "s3 == s10? " << (s3 == s10 ? "Yes" : "No") << endl;

    // 测试相等操作符 (不相等)
    cout << "s3 == s4? " << (s3 == s4 ? "Yes" : "No") << endl;

    // 测试移动构造
    MyString s11 = std::move(s10);
    cout << "s11: " << s11.size() << " " << s11 << " characters" << endl;

    // 测试移动赋值
    MyString s12;
    s12 = std::move(s4);
    cout << "s12: " << s12.size() << " " << s12 << " characters" << endl;

    // 测试从空字符串移动构造
    MyString s13 = std::move(s9);
    cout << "s13: " << (s13.size() == 0 ? "Empty" : "Not Empty") << endl;
}

int main() {
	testMyString();
    cout << endl;
    TestMyString();
	return 0;
}