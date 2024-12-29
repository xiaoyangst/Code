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
	map<string,int> m_dict;	// <单词,出现次数>
	map<string, set<int>> m_wordNumbers;	// <单词，行号>
	vector<string> lines;	// 行号对应的原始数据

};


/*
	内容由 单词 空格 换行 数字


	该程序将读取用户指定的任意文本文件【当前目录下的china_daily.txt】，
然后允许用户从该文件中查找单词。查询的结果是该单词出现的次数，并列
出每次出现所在的行。如果某单词在同一行中多次出现，程序将只显示该行
一次。行号按升序显示。

要求：
a. 它必须允许用户指明要处理的文件名字。

b. 程序将存储该文件的内容，以便输出每个单词所在的原始行。
vector<string> lines;//O(1)

c. 它必须将每一行分解为各个单词，并记录每个单词所在的所有行。
在输出行号时，应保证以升序输出，并且不重复。

map<string, set<int> > wordNumbers;
map<string, int> dict;

d. 对特定单词的查询将返回出现该单词的所有行的行号。

e. 输出某单词所在的行文本时，程序必须能根据给定的行号从输入
文件中获取相应的行。
*/