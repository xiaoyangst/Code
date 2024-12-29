#pragma once
#include <string>
#include <unordered_map>

using namespace std;


class Dictionary
{
public:
	//......
	Dictionary(const std::string& src, const std::string& dst);
	void read(const std::string& filename);
	void store(const std::string& filename);
	bool isWord(const std::string& word);
private:
	unordered_map<string,unsigned long int> m_dict;
};


/*
	内容由 单词 空格 换行 数字
*/