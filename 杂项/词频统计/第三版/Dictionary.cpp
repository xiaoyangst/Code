#include "Dictionary.h"
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>

Dictionary::Dictionary(const std::string& src)
{
	read(src);
}

void
Dictionary::read(const std::string& filename) {
	ifstream fstream(filename);
	string line;
	int line_index = 0;
	while (getline(fstream,line))
	{
		lines.push_back(line);
		istringstream sstream(line);
		string word;
		while (sstream >> word) {
			if (isWord(word))
			{
				auto its = m_dict.find(word);
				
				if (its != m_dict.end()) {
					++its->second;
				}
				else {
					m_dict.insert(make_pair(word, 1));
				}

				auto wordNumber = m_wordNumbers.find(word);
				if (wordNumber != m_wordNumbers.end()) {
					wordNumber->second.insert(line_index);
				}
				else {
					m_wordNumbers.insert(make_pair(word, set<int>{line_index}));
				}
			}
		}
		++line_index;
	}
}

void Dictionary::query(const string& word)
{

	auto its = m_dict.find(word);
	if (its == m_dict.end()) {
		cout << word << " don't exist" << endl;
		return;
	}

	cout << word <<" occurs "<< its->second<<" times." << endl;

	for (auto line : m_wordNumbers[word]) {
		cout << "("<< line <<")" << lines[line] <<endl;
	}
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

