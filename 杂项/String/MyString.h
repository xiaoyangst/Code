#pragma once
#include<iostream>
using namespace std;
class MyString
{
public:
	MyString();
	~MyString();
	explicit MyString(const char* str);
	MyString(const MyString& other);
	MyString& operator=(const MyString& other);
	MyString(MyString&& other) noexcept;
	MyString& operator=(MyString&& other)noexcept;

	bool operator==(const MyString& other);
	MyString operator+(const MyString& other);
	friend ostream& operator<<(ostream& os, const MyString& myStr);
	size_t size();
private:
	char* str_;
};

