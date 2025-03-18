/**
  ******************************************************************************
  * @file           : Trie.h
  * @author         : xy
  * @brief          : None
  * @attention      : None
  * @date           : 2025/3/18
  ******************************************************************************
  */

#ifndef INTERVIEW_TRIETREE_TRIE_H_
#define INTERVIEW_TRIETREE_TRIE_H_

#include <iostream>
#include <unordered_map>
#include <vector>

using namespace std;

struct Node {
  explicit Node(char ch, size_t freq = 0) : ch_(ch), freq_(freq) {}

  char ch_;
  size_t freq_;
  unordered_map<char, Node *> children_;
};

class Trie {
 public:
  Trie() : root_(new Node(' ', 0)) {}

  ~Trie() {
	  clean(root_);
  }

  void insert(const string &word) {
	  if (word.empty()) return;

	  auto curNode = root_;
	  for (const auto &ch : word) {
		  auto it = curNode->children_.find(ch);
		  if (it == root_->children_.end()) {    // 不存在就新增节点
			  curNode->children_[ch] = new Node(ch);
		  }
		  curNode = curNode->children_[ch];        // 移动到下一个节点
	  }
	  curNode->freq_++;
  }

  void remove(const string &word) {
	  if (word.empty()) return;
	  if (search(word) == 0) return;    // 确保删除的单词是存在的

	  remove(root_, word, 0);
  }

  int search(const string &word) {
	  if (word.empty()) return 0;
	  auto curNode = root_;
	  for (const auto &ch : word) {
		  auto it = curNode->children_.find(ch);
		  if (it == root_->children_.end()) {
			  return 0;
		  }
		  curNode = curNode->children_[ch];
	  }
	  return curNode->freq_;
  }

  vector<string> prefix(const string &word) {    // 根据前缀匹配单词集合
	  vector<string> res;
	  if (word.empty()) return res;

	  auto curNode = root_;
	  for (const auto ch : word) {
		  auto it = curNode->children_.find(ch);
		  if (it == root_->children_.end()) {    // 没有完全匹配该前缀
			  return res;
		  }
		  curNode = curNode->children_[ch];
	  }

	  if (curNode->freq_ > 0) {    // 如果前缀本身也构成一个单词，加入
		  res.push_back(word);
	  }

	  //拿到前缀节点后，收集后面所有的完整字符
	  nodeBack(curNode, const_cast<string &>(word), res);
	  return res;
  }

  void preOrder() {        //前序遍历输出所有单词
	  vector<string> res;
	  string word;
	  auto root = root_;
	  nodeBack(root, word, res);
	  for (const auto &str : res) {
		  cout << str << endl;
	  }
  }

 private:
  void clean(Node *root) {
	  if (root == nullptr) return;
	  for (auto &child : root->children_) {
		  clean(child.second);
	  }
	  delete root;
  }

  void nodeBack(Node *curNode, string &word, vector<string> &wordVec) {
	  if (curNode == nullptr) return;
	  for (auto &child : curNode->children_) {
		  word.push_back(child.first);
		  nodeBack(child.second, word, wordVec);
		  if (child.second->freq_ > 0) {
			  wordVec.push_back(word);
		  }
		  word.pop_back();
	  }
  }

  Node *remove(Node *curNode, const string &word, size_t depth) {
	  if (curNode == nullptr) return nullptr;
	  if (depth == word.size()) {
		  if (curNode->freq_ > 0) {
			  curNode->freq_ = 0;    // 表示删除字符串
		  }
		  if (curNode->children_.empty()) {    // 后面没有连接任何字符，该节点可以移除了
			  delete curNode;
			  curNode = nullptr;
		  }
		  return curNode;
	  }

	  auto it = curNode->children_.find(word[depth]);
	  auto node = remove(it->second, word, depth + 1);
	  if (node == nullptr) {
		  curNode->children_.erase(it);	// 删除当前节点记录的某个子节点
	  }

	  if (curNode->children_.empty() && curNode->freq_ < 1) {	// 表面当前节点没有存在的必要，可删除了
		  delete curNode;
		  curNode = nullptr;
	  }
	  return curNode;
  }

 private:
  Node *root_;
};

#endif //INTERVIEW_TRIETREE_TRIE_H_
