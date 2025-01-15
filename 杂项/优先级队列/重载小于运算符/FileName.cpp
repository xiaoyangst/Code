#include <queue>
#include <iostream>
using namespace std;

#include <ctime>

struct FreNode {
	int freq;
	time_t times;
	FreNode() : freq(1), times(time(0)) {}
};

struct Node {
	int key;
	int value;
	FreNode node;
	Node(int k, int v) :key(k), value(v) {
	}
	~Node() {}
	int getvalue() {
		return value;
	}
	// 重载 operator <		// 频率越低，时间越小，优先被抛弃
	bool operator <(const Node& rhs) const {
		if (node.freq != rhs.node.freq) {	// 频率低的应该被抛弃，因此把频率低的放右边
			return node.freq > rhs.node.freq;
		}
		return node.times > rhs.node.times;	// 时间小的应该被抛弃，因此把时间小的放右边
	}
};

int main() {

	priority_queue<Node> pq;
	
	// 频率相同，时间不同
	Node n1(1, 1);
	n1.node.freq = 10;
	n1.node.times = 10;
	Node n2(2, 2);
	n2.node.freq = 10;
	n2.node.times = 20;

	pq.push(n1);
	pq.push(n2);

	pq.pop();
	pq.pop();

	// 频率不同，时间相同
	Node n3(3, 3);
	n3.node.freq = 40;
	n3.node.times = 40;
	Node n4(4, 4);
	n4.node.freq = 50;
	n4.node.times = 40;

	pq.push(n3);
	pq.push(n4);

	pq.pop();
	pq.pop();


	return 0;
}