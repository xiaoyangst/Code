#include <iostream>
#include <memory>
using namespace std;
#include <vector>

class UnionFind {
 public:
  explicit UnionFind(int size) {
	  parent_.resize(size);
	  rank_.resize(size, 1);    // 最初，所有的秩为 1
	  for (int i = 0; i < size; ++i) {
		  parent_[i] = i;
	  }
  }

  // 查找
  int find(int index) {
	  while (index != parent_[index]) {
		  parent_[index] = find(parent_[index]);
	  }
	  return parent_[index];
  }

  // 合并两个集合
  void unionSet(int x, int y) {
	  if (x < 0 || y < 0 || x >= parent_.size() || y >= parent_.size()) {
		  return;
	  }

	  // 找到各自的根节点
	  auto x_root = find(x);
	  auto y_root = find(y);

	  // 按秩合并
	  if (x_root != y_root) {
		  if (rank_[x_root] < rank_[y_root]) {
			  parent_[x_root] = y_root;
		  } else if (rank_[y_root] < rank_[x_root]) {
			  parent_[y_root] = x_root;
		  } else {
			  parent_[y_root] = x_root;
			  rank_[x_root]++;
		  }
	  }
  }

  // 判断两个元素是否属于同一个集合
  bool isConnected(int x, int y) {
	  return find(x) == find(y);
  }

  void printParentArray() {
	  cout << "parent: ";
	  for (int i : parent_) {
		  cout << i << " ";
	  }
	  cout << endl;
  }

 private:
  std::vector<int> parent_;
  std::vector<int> rank_;
};

int main() {

	UnionFind union_find(10);

	union_find.unionSet(1, 2);
	union_find.unionSet(3, 4);
	union_find.unionSet(3, 5);

	union_find.unionSet(1, 3);
	union_find.printParentArray();

	return 0;
}
