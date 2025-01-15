#ifndef INTERVIEW_BSTSEARCH_BSTMAP_H_
#define INTERVIEW_BSTSEARCH_BSTMAP_H_

#include <optional>
#include <iostream>
template<typename K, typename V>
struct TreeNode {
  std::pair<K, V> data;
  TreeNode *left;
  TreeNode *right;
  TreeNode *parent;
  TreeNode(const K &key, const V &val, TreeNode *parent = nullptr)
      : data(std::make_pair(key, val)), left(nullptr), right(nullptr), parent(parent) {}
};

template<typename K, typename V>
class BSTMap {
 public:
  explicit BSTMap() : root(nullptr) {}
  ~BSTMap() {
      clear(root);
      root == nullptr;
  }

  // 禁止拷贝构造和赋值
  BSTMap(const BSTMap &other) = delete;
  BSTMap &operator=(BSTMap &&other) = delete;

  TreeNode<K, V> *find(const K &target) {
      TreeNode<K, V> *find_node = root;

      while (find_node) {
          auto cur = find_node->data.first;
          if (target < cur) {
              find_node = find_node->left;
          } else if (target > cur) {
              find_node = find_node->right;
          } else {
              return find_node;
          }
      }

      return nullptr;
  }

  void insert(const K &key, const V &val) {
      auto new_node = new TreeNode<K, V>(key, val);
      if (root == nullptr) {
          root = new_node;
          return;
      }

      TreeNode<K, V> *cur = root;
      TreeNode<K, V> *parent = nullptr;

      while (cur) {
          auto cur_key = cur->data.first;
          parent = cur;
          if (key < cur_key) {
              cur = cur->left;
          } else if (key > cur_key) {
              cur = cur->right;
          } else {    /* 节点已存在，更新 val 即可 */
              root->data.second = val;
              return;
          }
      }
      if (key < parent->data.first) {
          parent->left = new_node;
      } else {
          parent->right = new_node;
      }
      new_node->parent = parent;
  }

  /*
  void erase(const K &key) {
      if (root == nullptr) return;

      TreeNode<K, V> *target = find(key);
      if (target == nullptr) return;


      // 优先处理两个子节点的，将其降级为单个子节点
      if (target->left && target->right){
          TreeNode<K, V> *successor = mininum(target->right);
          target->data = successor->data;
          target = successor;
      }

      TreeNode<K, V> *child = target->left ? target->left : target->right;
      TreeNode<K, V> *parent = target->parent;
      if (child) {
          if (parent) {
              parent->left == target ? parent->left = child : parent->right = child;
              child->parent = parent;
          } else {
              // chile 现在就为 root 节点，得更新相关信息
              child->parent = nullptr;
              root = child;
          }
      }else{
          if (parent) {
              parent->left == target ? parent->left = nullptr : parent->right = nullptr;
          }else{
              root = nullptr;
          }
      }

      delete target;
      target = nullptr;
  }
   */

  void erase(const K &key) {
      if (root == nullptr) return;

      TreeNode<K, V> *target = find(key);
      if (target == nullptr) return;

      // 优先处理两个子节点的，将其降级为单个子节点
      if (target->left && target->right) {
          TreeNode<K, V> *successor = mininum(target->right);
          target->data = successor->data;
          target = successor;
      }

      TreeNode<K, V> *child = target->left ? target->left : target->right;
      TreeNode<K, V> *parent = target->parent;

      // 如果是单个节点，更新最新的 parent 节点
      if (child) {
          child->parent = parent;
      }

      // 考虑根节点的情况
      if (parent) {
          parent->left == target ? parent->left = child : parent->right = child;
      } else {
          root = child;
      }

      delete target;
      target = nullptr;
  }

  void Print(TreeNode<K, V> *root_node) {
      if (root_node == nullptr) return;
      Print(root_node->left);
      std::cout << "<" << root_node->data.first << "," << root_node->data.second << ">" << " ";
      Print(root_node->right);
  }

  TreeNode<K, V> *getroot() {
      return root;
  }
 private:
  void clear(TreeNode<K, V> *root_node) {
      if (root_node == nullptr) return;
      clear(root_node->left);
      clear(root_node->right);
      delete root_node;
  }
  TreeNode<K, V> *mininum(TreeNode<K, V> *node) {
      while (node->left != nullptr) {
          node = node->left;
      }
      return node;
  }
  TreeNode<K, V> *Inorder_Successor(TreeNode<K, V> *node) {
      if (node->right) {  // 存在右子树
          return mininum(node->right);
      }

      TreeNode<K, V> *parent = node->parent;
      while (parent && parent->left != node) {
          node = parent;
          parent = parent->parent;
      }
      return parent;
  }
 private:
  TreeNode<K, V> *root;
};

#endif //INTERVIEW_BSTSEARCH_BSTMAP_H_
