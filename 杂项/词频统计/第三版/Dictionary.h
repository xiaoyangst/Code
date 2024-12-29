#pragma once
#include <string>
#include <map>
#include <set>
#include <vector>

using namespace std;


class Dictionary
{
public:
	//......
	Dictionary(const std::string& src);
	void read(const std::string& filename);
	void query(const string& word);
	bool isWord(const std::string& word);
private:
	map<string,int> m_dict;	// <����,���ִ���>
	map<string, set<int>> m_wordNumbers;	// <���ʣ��к�>
	vector<string> lines;	// �кŶ�Ӧ��ԭʼ����

};


/*
	������ ���� �ո� ���� ����


	�ó��򽫶�ȡ�û�ָ���������ı��ļ�����ǰĿ¼�µ�china_daily.txt����
Ȼ�������û��Ӹ��ļ��в��ҵ��ʡ���ѯ�Ľ���Ǹõ��ʳ��ֵĴ���������
��ÿ�γ������ڵ��С����ĳ������ͬһ���ж�γ��֣�����ֻ��ʾ����
һ�Ρ��кŰ�������ʾ��

Ҫ��
a. �����������û�ָ��Ҫ������ļ����֡�

b. ���򽫴洢���ļ������ݣ��Ա����ÿ���������ڵ�ԭʼ�С�
vector<string> lines;//O(1)

c. �����뽫ÿһ�зֽ�Ϊ�������ʣ�����¼ÿ���������ڵ������С�
������к�ʱ��Ӧ��֤��������������Ҳ��ظ���

map<string, set<int> > wordNumbers;
map<string, int> dict;

d. ���ض����ʵĲ�ѯ�����س��ָõ��ʵ������е��кš�

e. ���ĳ�������ڵ����ı�ʱ����������ܸ��ݸ������кŴ�����
�ļ��л�ȡ��Ӧ���С�
*/