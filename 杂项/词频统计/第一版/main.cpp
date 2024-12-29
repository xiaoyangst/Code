#include <iostream>
using namespace std;

#include "Dictionary.h"

int main(int argc,char* argv[]) {

	if (argc != 3) {
		cerr << "./Dictionary file_src file_dst" << endl;
		return -1;
	}

	string src = string(argv[1]);
	string dst = string(argv[2]);

	Dictionary dir(src,dst);
	
	return 0;
}