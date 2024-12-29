#include "Dictionary.h"
#include <fstream>
#include <sstream>
#include <string>

Dictionary::Dictionary(const std::string& src, const std::string& dst)
{
	read(src);
	store(dst);
}

void
Dictionary::read(const std::string& filename) {
	ifstream fstream(filename);
	string line;
	while (getline(fstream,line))
	{
		istringstream sstream(line);
		string word;
		while (sstream >> word) {
			if (isWord(word))
			{
				auto its = m_dict.find(word);
				if (its != m_dict.end()) {
					its->second++;
					continue;
				}
				m_dict.insert(make_pair(word, 1));
			}
		}
	}
	fstream.close();
}

void Dictionary::store(const std::string& filename)
{
	// 打开一个文件
	ofstream sstream(filename, ios::app);
	// 遍历 map ，逐一写入文件
	for (auto its : m_dict) {
		sstream << its.first << " " << its.second<< "\n";
	}
	sstream.close();
}

bool 
Dictionary::isWord(const std::string& word)
{
	for (auto c : word)
	{
		if (!isalpha(c))
		{
			return false;
		}
	}
	return true;
}

