#define _CRT_SECURE_NO_WARNINGS
#include "MyString.h"
#include <string.h>

MyString::MyString():str_(nullptr) {

}

MyString::~MyString()
{
	delete[] str_;
}

MyString::MyString(const char* str){
	if (str == nullptr) {
		str_ = nullptr;
		return;
	}
	size_t len = strlen(str);	
	str_ = new char[len + 1]; // 字符串原长度 + 1 是为了 存储 空字符
	strcpy(str_, str);
}

MyString::MyString(const MyString& other)	// 拷贝构造
{
	if (other.str_ == nullptr) {
		str_ = nullptr;
		return;
	}
	size_t len = strlen(other.str_);
	str_ = new char[len + 1]; 
	strcpy(str_, other.str_);
	cout << "拷贝构造" << endl;
}

MyString& MyString::operator=(const MyString& other)	// 拷贝赋值
{
	if (this != &other) {
		// 不清楚 other 的 成员 str_ 的长度，先把当前对象 str_ 空间释放，重新申请和 other 的 成员 str_ 相同的长度
		delete[] str_;
		if (other.str_ == nullptr) {
			str_ = nullptr;
			return *this;
		}
		size_t len = strlen(other.str_);
		str_ = new char[len + 1];
		strcpy(str_, other.str_);
		cout << "拷贝赋值" << endl;
	}
	return *this;
}

MyString::MyString(MyString&& other) noexcept	// 移动构造
{
	str_ = other.str_;		// 接管 other 拥有的资源
	other.str_ = nullptr;	// 移除 other 拥有的资源
	cout << "移动构造" << endl;
}

MyString& MyString::operator=(MyString&& other) noexcept	// 移动赋值
{
	if (this != &other) {
		delete[] str_;			// 先释放已有的资源
		str_ = other.str_;		// 接管 other 拥有的资源
		other.str_ = nullptr;	// 移除 other 拥有的资源
		cout << "移动赋值" << endl;
	}
	return *this;
}

bool MyString::operator==(const MyString& other)
{
	if (str_ == nullptr && other.str_ == nullptr) return true;
	if (str_ == nullptr || other.str_ == nullptr) return false;
	return strcmp(str_, other.str_) == 0;
}

MyString MyString::operator+(const MyString& other) {
	if (other.str_ == nullptr) return *this;  

	size_t len = (str_ ? strlen(str_) : 0) + strlen(other.str_);
	char* new_str = new char[len + 1];  

	if (str_ != nullptr) {
		strcpy(new_str, str_);
	}

	strcat(new_str, other.str_);

	// 创建一个新的 MyString 对象并返回
	MyString result(new_str);
	delete[] new_str;  // 释放临时分配的内存
	return result;
}

ostream& operator<<(ostream& os, const MyString& myStr) {
	if (myStr.str_ != nullptr) {
		os << myStr.str_;
	}
	else {
		os << "(null)";
	}
	return os;
}

size_t MyString::size()
{
	return str_ ? strlen(str_) : 0;
}

