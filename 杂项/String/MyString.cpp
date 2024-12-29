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
	str_ = new char[len + 1]; // �ַ���ԭ���� + 1 ��Ϊ�� �洢 ���ַ�
	strcpy(str_, str);
}

MyString::MyString(const MyString& other)	// ��������
{
	if (other.str_ == nullptr) {
		str_ = nullptr;
		return;
	}
	size_t len = strlen(other.str_);
	str_ = new char[len + 1]; 
	strcpy(str_, other.str_);
	cout << "��������" << endl;
}

MyString& MyString::operator=(const MyString& other)	// ������ֵ
{
	if (this != &other) {
		// ����� other �� ��Ա str_ �ĳ��ȣ��Ȱѵ�ǰ���� str_ �ռ��ͷţ���������� other �� ��Ա str_ ��ͬ�ĳ���
		delete[] str_;
		if (other.str_ == nullptr) {
			str_ = nullptr;
			return *this;
		}
		size_t len = strlen(other.str_);
		str_ = new char[len + 1];
		strcpy(str_, other.str_);
		cout << "������ֵ" << endl;
	}
	return *this;
}

MyString::MyString(MyString&& other) noexcept	// �ƶ�����
{
	str_ = other.str_;		// �ӹ� other ӵ�е���Դ
	other.str_ = nullptr;	// �Ƴ� other ӵ�е���Դ
	cout << "�ƶ�����" << endl;
}

MyString& MyString::operator=(MyString&& other) noexcept	// �ƶ���ֵ
{
	if (this != &other) {
		delete[] str_;			// ���ͷ����е���Դ
		str_ = other.str_;		// �ӹ� other ӵ�е���Դ
		other.str_ = nullptr;	// �Ƴ� other ӵ�е���Դ
		cout << "�ƶ���ֵ" << endl;
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

	// ����һ���µ� MyString ���󲢷���
	MyString result(new_str);
	delete[] new_str;  // �ͷ���ʱ������ڴ�
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

