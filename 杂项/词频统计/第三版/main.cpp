#include <iostream>
using namespace std;

#include "Dictionary.h"

int main(int argc,char* argv[]) {

	if (argc != 2) {
		cerr << "./Dictionary file_src" << endl;
		return -1;
	}

	string src = string(argv[1]);

	Dictionary dir(src);

	string word;
	while (true) {
		cout << "please input £º";
		cin >> word;
		dir.query(word);
	}
	
	return 0;
}