#include "MyString.h"

// ���Ժ���
void testMyString() {
	MyString s1("Hello");
	MyString s2("World");

	MyString s3 = s1 + s2; // ���� operator+
	cout << "s3: " << s3.size() << " " << s3 << " characters" << endl;

	MyString s4(s3); // ���Կ�������
	cout << "s4: " << s4.size() << " " << s4 << " characters" << endl;

	MyString s5;
	s5 = s1; // ���Կ�����ֵ
	cout << "s5: " << s5.size() << " " << s5 << " characters" << endl;

	cout << "s1 == s5? " << (s1 == s5 ? "Yes" : "No") << endl; // ���� operator==

	MyString s6 = std::move(s2); // �����ƶ�����
	cout << "s6: " << s6.size() << " " << s6 << " characters" << endl;
	cout << "s2: " << s2.size() << " " << s2 << " characters" << endl;

	MyString s7;
	s7 = std::move(s3); // �����ƶ���ֵ
	cout << "s7: " << s7.size() << " " << s7 << " characters" << endl;
	cout << "s3: " << s3.size() << " " << s3 << " characters" << endl;
}


void TestMyString() {
    // ����Ĭ�Ϲ��캯��
    MyString s1;
    cout << "s1: " << (s1.size() == 0 ? "Empty" : "Not Empty") << endl;

    // ���Դ����ָ��Ĺ��캯��
    MyString s2(nullptr);
    cout << "s2: " << (s2.size() == 0 ? "Empty" : "Not Empty") << endl;

    // ������ͨ�ַ�������
    MyString s3("Hello");
    cout << "s3: " << s3.size() << " " << s3 << " characters" << endl;

    // ���� operator+ (ƴ�������ַ���)
    MyString s4 = s3 + MyString(" World");
    cout << "s4: " << s4.size() << " " << s4 << " characters" << endl;

    // ���� operator+ (ƴ�ӿ��ַ���)
    MyString s5 = s3 + MyString("");
    cout << "s5: " << s5.size() << " " << s5 << " characters" << endl;

    // ���� operator+ (ƴ�� null �ַ���)
    MyString s6 = s3 + MyString(nullptr);
    cout << "s6: " << s6.size() << " " << s6 << " characters" << endl;

    // ���Կ�������
    MyString s7(s4);
    cout << "s7: " << s7.size() << " " << s7 << " characters" << endl;

    // ���Կ�����ֵ
    MyString s8;
    s8 = s4;
    cout << "s8: " << s8.size() << " " << s8 << " characters" << endl;

    // ���Կ�����ֵ�Ϳ��ַ����ĸ�ֵ
    MyString s9;
    s9 = MyString(""); // ��ֵ���ַ���
    cout << "s9: " << (s9.size() == 0 ? "Empty" : "Not Empty") << endl;

    // ������Ȳ����� (���)
    MyString s10("Hello");
    cout << "s3 == s10? " << (s3 == s10 ? "Yes" : "No") << endl;

    // ������Ȳ����� (�����)
    cout << "s3 == s4? " << (s3 == s4 ? "Yes" : "No") << endl;

    // �����ƶ�����
    MyString s11 = std::move(s10);
    cout << "s11: " << s11.size() << " " << s11 << " characters" << endl;

    // �����ƶ���ֵ
    MyString s12;
    s12 = std::move(s4);
    cout << "s12: " << s12.size() << " " << s12 << " characters" << endl;

    // ���Դӿ��ַ����ƶ�����
    MyString s13 = std::move(s9);
    cout << "s13: " << (s13.size() == 0 ? "Empty" : "Not Empty") << endl;
}

int main() {
	testMyString();
    cout << endl;
    TestMyString();
	return 0;
}