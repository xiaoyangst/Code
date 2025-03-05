#include "Huffman.h"
#include <iostream>

int main() {
	string text = "huffman coding example";

	Huffman huffman(text);

	// 输出 Huffman 编码表
	cout << "Huffman Codes:\n";
	for (auto &entry : huffman.huffmanCodeTable()) {
		cout << entry.first << " : " << entry.second << endl;
	}

	// 编码
	string encoded = huffman.encode(text);
	cout << "\nEncoded: " << encoded << endl;

	// 解码
	string decoded = huffman.decode(encoded);
	cout << "\nDecoded: " << decoded << endl;

	return 0;
}