#pragma once
#include <string>
#include <map>
using namespace std;


class Dictionary
{
public:
	Dictionary(const std::string& src, const std::string& dst);
	void read(const std::string& filename);
	void store(const std::string& filename);
	bool isWord(const std::string& word);
private:
	map<string,unsigned long int> m_dict;
};

