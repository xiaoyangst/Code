#ifndef INTERVIEW_HUFFMAN_HUFFMAN_H_
#define INTERVIEW_HUFFMAN_HUFFMAN_H_
#include <string>
#include <unordered_map>
#include <queue>
using namespace std;

class Huffman {
  struct Node {
	char data_;
	int frequency_;
	Node *left_;
	Node *right_;

	Node(char data, int frequency) : data_(data), frequency_(frequency), left_(nullptr), right_(nullptr) {}

	bool operator<(const Node &right) const {    // 频率越低，越靠前
		return this->frequency_ > right.frequency_;
	}
  };
 public:
  Huffman(const string &str) {
	  buildHuffman(str);
	  generateCodes(root_, "");
  }

  ~Huffman() {
	  clear(root_);
  }

  string encode(const string &text) const {    // 把字符串编码
	  string result;
	  for (char c : text) {
		  result += huffmanCodeTable_.at(c);    // at 确保能匹配到，否则报错
	  }
	  return result;
  }

  string decode(const string &encodedString) const {    // 解码--》字符串
	  string result;
	  auto cur_node = root_;

	  for (char c : encodedString) {
		  if (c == '0') {
			  cur_node = cur_node->left_;
		  } else {
			  cur_node = cur_node->right_;
		  }

		  if (cur_node->left_ == nullptr && cur_node->right_ == nullptr) {    // 解析出一个字符
			  result += cur_node->data_;
			  cur_node = root_;    // 回到根节点，因为任何字符串编码都是从根节点开始的
		  }
	  }
	  return result;
  }

  unordered_map<char, string> huffmanCodeTable(){
	  return huffmanCodeTable_;
  }

 private:
  void buildHuffman(const string &str) {
	  // 统计词频
	  std::unordered_map<char, int> wordFre;
	  for (char c : str) {
		  wordFre[c]++;
	  }
	  // 加入优先级队列
	  priority_queue<Node *> NodeQue;
	  for (const auto &word : wordFre) {
		  NodeQue.push(new Node(word.first, word.second));
	  }
	  // 构建哈夫曼树
	  // 不断取出优先级队列中的两个节点，并创建一个节点记录两个节点的权值和，直到只剩下一个节点才终止
	  while (NodeQue.size() > 1) {
		  auto leftNode = NodeQue.top();
		  NodeQue.pop();
		  auto rightNode = NodeQue.top();
		  NodeQue.pop();

		  auto parent_node = new Node('\0', leftNode->frequency_ + rightNode->frequency_);
		  parent_node->left_ = leftNode;
		  parent_node->right_ = rightNode;
		  NodeQue.push(parent_node);
	  }
	  root_ = NodeQue.top();
	  NodeQue.pop();
  }

  // 生成哈夫曼表
  void generateCodes(Node *node, string code) {
	  if (node == nullptr) return;
	  if (node->left_ == nullptr && node->right_ == nullptr) {    // 叶子节点，记录字符的编码
		  huffmanCodeTable_[node->data_] = code;
	  }
	  generateCodes(node->left_, code + "0");
	  generateCodes(node->right_, code + "1");
  }

  void clear(Node *Node) {
	  if (Node == nullptr) {
		  return;
	  }
	  clear(Node->left_);
	  clear(Node->right_);
	  delete Node;
	  Node = nullptr;
  }

 private:
  Node *root_;
  unordered_map<char, string> huffmanCodeTable_; // Huffman 编码表
};

#endif //INTERVIEW_HUFFMAN_HUFFMAN_H_
