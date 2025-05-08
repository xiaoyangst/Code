#include <iostream>
#include <vector>
#include <queue>
#include <string>
#include <list>
using namespace std;

struct Edge {
  int from;
  int to;
  int weight;

  Edge(int from, int to, int weight) : from(from), to(to), weight(weight) {}

  bool operator<(const Edge &other) const {
	  return weight > other.weight;
  }
};

class UnionFind {
 public:
  explicit UnionFind(int size) : parent(size) {
	  for (int i = 0; i < size; ++i) {
		  parent[i] = i;
	  }
  }

  int find(int x) {
	  if (x != parent[x]) {
		  parent[x] = find(parent[x]);
	  }
	  return parent[x];
  }

  void merge(int x1, int x2) {
	  auto new_x1 = find(x1);
	  auto new_x2 = find(x2);
	  if (new_x1 != new_x2) {
		  parent[new_x1] = new_x2;
	  }
  }

 private:
  vector<int> parent;
};

int kruskal(int n, priority_queue<Edge> &edges, list<Edge> &mst) {
	UnionFind union_find(n);
	int total_weight = 0;

	while (!edges.empty()) {
		auto item = edges.top();
		edges.pop();
		if (union_find.find(item.from) != union_find.find(item.to)) {    // 不在同一个集合中
			union_find.merge(item.from, item.to);
			mst.push_back(item);            // 记录优先加入的边
			total_weight += item.weight;    // 记录总的权重

			if (mst.size() == n - 1) {
				break;
			}
		}
	}
	return total_weight;
}

int main() {
	int n = 4;  // 节点数（编号从 0 到 n-1）
	vector<Edge> edges = {
		{0, 1, 10},
		{0, 2, 6},
		{0, 3, 5},
		{1, 3, 15},
		{2, 3, 4}
	};

	priority_queue<Edge> pq;
	for (const Edge &e : edges) {
		pq.push(e);
	}

	list<Edge> mst;
	int total = kruskal(n, pq, mst);

	cout << "最小生成树总权重: " << total << endl;
	cout << "选中的边:" << endl;
	for (const Edge &e : mst) {
		cout << e.from << " --> " << e.to << " (权重 " << e.weight << ")" << endl;
	}

	return 0;
}
