#include <iostream>
#include <vector>
#include <sstream>
#include <queue>

using namespace std;

struct Edge {
  int from = 0;
  int to = 0;
  int distance = 0;

  Edge(int from, int to, int distance) : from(from), to(to), distance(distance) {}
};

using NodeInfo = pair<int, int>;    // distance（距离） from（顶点）

vector<int> dijkstra(int start, vector<vector<Edge>> &graph) {
	// 起初，起点到自己的距离为 0，其余为正无穷
	int n = graph.size();
	vector<int> dist(n, numeric_limits<int>::max());
	dist[start] = 0;
	// 把起始顶点加入优先级队列，探索开始
	priority_queue<NodeInfo, vector<NodeInfo>, greater<>> pq;
	pq.emplace(0, start);

	while (!pq.empty()) {
		auto [cur_distance, cur_from] = pq.top();
		pq.pop();

		if (cur_distance > dist[cur_from]) continue;    // 已经有更短的路径

		for (auto &edge : graph[cur_from]) {    // 访问 node 的所有邻居
			auto [from, to, distance] = edge;
			if (dist[to] > dist[cur_from] + distance) {    // 有更短的路径出现，更新
				dist[to] = dist[cur_from] + distance;
				pq.emplace(dist[to], to);
			}
		}
	}

	return dist;
}

int main() {
	int n = 5;
	int start = 0;
	vector<vector<Edge>> graph(n);

	// 使用 Edge 结构体存储边
	graph[0].emplace_back(0, 1, 4);
	graph[0].emplace_back(0, 3, 2);
	graph[1].emplace_back(1, 3, 5);
	graph[1].emplace_back(1, 4, 10);
	graph[3].emplace_back(3, 1, 1);
	graph[3].emplace_back(3, 4, 2);
	graph[4].emplace_back(4, 2, 7);

	auto dist = dijkstra(start, graph);

	for (int i = 0; i < n; i++)
		cout << "start : " <<start << " --> " <<" dest : "<< i << "  min distance : " << dist[i] << endl;

	return 0;
}