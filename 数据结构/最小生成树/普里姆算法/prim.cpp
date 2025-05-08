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

int prim(int n, vector<vector<Edge>> &edges, list<Edge> &mst) {
	// 把所有定点设置为从未访问
	vector<bool> visited(n, false);
	int total_weight = 0;

	// 随意选择一个顶点
	// 将选择的顶点设置为已访问，并且把能连接的边加入优先队列，选择权重最小的一条边
	visited[0] = true;
	priority_queue<Edge> pq;
	for (const Edge &e : edges[0]) {
		pq.push(e);
	}

	while (!pq.empty()) {
		auto [from, to, weight] = pq.top();
		pq.pop();

		if (visited[to]) continue;    // 已经访问过

		mst.emplace_back(from, to, weight);    // 记录已访问的边
		total_weight += weight;

		// 下面就是访问 to 顶点
		visited[to] = true;
		for (const auto &edge : edges[to]) {
			if (!visited[edge.to]) {    // 只加入未访问的边
				pq.push(edge);
			}
		}
	}
	return total_weight;
}


int main() {
	int n = 5;  // 顶点数
	vector<vector<Edge>> graph(n);

	// 添加无向图的边（双向）
	graph[0].emplace_back(0, 1, 2);
	graph[1].emplace_back(1, 0, 2);

	graph[0].emplace_back(0, 3, 6);
	graph[3].emplace_back(3, 0, 6);

	graph[1].emplace_back(1, 2, 3);
	graph[2].emplace_back(2, 1, 3);

	graph[1].emplace_back(1, 3, 8);
	graph[3].emplace_back(3, 1, 8);

	graph[1].emplace_back(1, 4, 5);
	graph[4].emplace_back(4, 1, 5);

	graph[2].emplace_back(2, 4, 7);
	graph[4].emplace_back(4, 2, 7);

	graph[3].emplace_back(3, 4, 9);
	graph[4].emplace_back(4, 3, 9);

	list<Edge> mst;
	int total_weight = prim(n, graph, mst);

	cout << "Prim 最小生成树的总权重: " << total_weight << endl;
	cout << "边如下：" << endl;
	for (const Edge& e : mst) {
		cout << e.from << " --> " << e.to << " (权重: " << e.weight << ")" << endl;
	}

	return 0;
}