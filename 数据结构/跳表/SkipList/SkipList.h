/**
  ******************************************************************************
  * @file           : SkipList.h
  * @author         : xy
  * @brief          : None
  * @attention      : None
  * @date           : 2025/3/20
  ******************************************************************************
  */

#ifndef INTERVIEW_SKIPLIST_SKIPLIST_H_
#define INTERVIEW_SKIPLIST_SKIPLIST_H_

#include <iostream>
#include <functional>
#include <random>
#include <climits>

struct Node {
  int key_;
  int value_;
  int level_;
  Node **forward_;    // 存储该节点在不同层次上的后继节点的指针，即当前节点的下一个节点

  Node(int key, int value, int level) : key_(key), value_(value), level_(level) {
	  forward_ = new Node *[level + 1];	// 得 + 1 是因为 level 是从 0 开始的
	  for (int i = 0; i < level; ++i) {
		  forward_[i] = nullptr;
	  }
  }

  ~Node() {
	  delete[] forward_;
  }
};

class SkipList {
 public:
  explicit SkipList() : level_(0), head_(new Node(INT_MIN, 0, MAX_LEVEL)) {
	  std::random_device rd;
	  gen_.seed(rd());
	  dist_ = std::bernoulli_distribution(0.5);
  }

  ~SkipList() {
	  Node *cur_node = head_->forward_[0];
	  while (cur_node != nullptr) {
		  Node *next_node = cur_node->forward_[0];
		  delete cur_node;
		  cur_node = next_node;
	  }
	  delete head_;
  }

  void insert(int key, int value) {
	  Node *update[MAX_LEVEL + 1];    // 保存每层的前驱节点，方便插入

	  Node *cur_node = head_;
	  for (int cur_level = level_; cur_level >= 0; --cur_level) {    // 从跳表当前最高层开始查找
		  while (cur_node->forward_[cur_level] != nullptr
			  && cur_node->forward_[cur_level]->key_ < key) {
			  cur_node = cur_node->forward_[cur_level];
		  }
		  update[cur_level] = cur_node;
	  }
	  // 存在就更新
	  cur_node = cur_node->forward_[0];
	  if (cur_node != nullptr && cur_node->key_ == key) {
		  cur_node->value_ = value;
		  return;
	  }

	  // 开始插入
	  int new_level = randomLevel();
	  // 如果新层大于当前层，需要更新 update，记录新层的前驱节点为 head_
	  if (new_level > level_) {
		  for (int i = level_ + 1; i <= new_level; ++i) {
			  update[i] = head_;
		  }
		  level_ = new_level;
	  }

	  Node *new_node = new Node(key, value, new_level);

	  // 开始在完整保存前驱节点的 update 数组中插入新节点
	  for (int i = 0; i <= new_level; ++i) {
		  new_node->forward_[i] = update[i]->forward_[i];    // 同层中 新节点执行其前驱节点的下一个节点
		  update[i]->forward_[i] = new_node;    // 同层中 前驱节点的下一个节点指向新节点
	  }
  }

  void remove(int key) {
	  Node *update[MAX_LEVEL + 1];    // 保存每层的前驱节点，方便插入

	  Node *cur_node = head_;
	  for (int cur_level = level_; cur_level >= 0; --cur_level) {    // 从跳表当前最高层开始查找
		  while (cur_node->forward_[cur_level] != nullptr
			  && cur_node->forward_[cur_level]->key_ < key) {
			  cur_node = cur_node->forward_[cur_level];
		  }
		  update[cur_level] = cur_node;
	  }
	  // 存在才考虑删除
	  cur_node = cur_node->forward_[0];
	  if (cur_node != nullptr && cur_node->key_ == key) {
		  for (int i = 0; i <= level_; ++i) {
			  if (update[i]->forward_[i] != cur_node) break;
			  update[i]->forward_[i] = cur_node->forward_[i];
		  }
		  delete cur_node;

		  // 调整跳表层数（可能存在删除节点后，跳表层数减少的情况）
		  while (level_ > 0 && head_->forward_[level_] == nullptr) {
			  level_--;
		  }
	  }
  }

  Node *search(int key) {
	  Node *cur_node = head_;
	  for (int cur_level = level_; cur_level >= 0; --cur_level) {    // 从跳表当前最高层开始查找
		  while (cur_node->forward_[cur_level] != nullptr
			  && cur_node->forward_[cur_level]->key_ < key) {
			  cur_node = cur_node->forward_[cur_level];
		  }
	  }
	  // 此时 cur_node 处在 跳表的最底层
	  cur_node = cur_node->forward_[0];
	  if (cur_node != nullptr && cur_node->key_ == key) {
		  return cur_node;
	  }
	  return nullptr;
  }

  void display() {
	  for (int i = level_; i >= 0; --i) {
		  Node *cur_node = head_->forward_[i];
		  std::cout << "Level " << i << ": ";
		  while (cur_node != nullptr) {
			  std::cout << cur_node->key_ << " ";
			  cur_node = cur_node->forward_[i];
		  }
		  std::cout << std::endl;
	  }
  }

 private:
  int randomLevel() {
	  auto level = 1;
	  while (dist_(gen_) && level < MAX_LEVEL) {    // 生成一个随机数，如果是 1，level+1
		  level++;
	  }
	  return level;
  }
 private:
  int level_;
  static constexpr int MAX_LEVEL = 32;
  Node *head_;
  std::mt19937 gen_;
  std::bernoulli_distribution dist_;
};

#endif //INTERVIEW_SKIPLIST_SKIPLIST_H_
