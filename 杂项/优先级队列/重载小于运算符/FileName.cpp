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
	// ���� operator <		// Ƶ��Խ�ͣ�ʱ��ԽС�����ȱ�����
	bool operator <(const Node& rhs) const {
		if (node.freq != rhs.node.freq) {	// Ƶ�ʵ͵�Ӧ�ñ���������˰�Ƶ�ʵ͵ķ��ұ�
			return node.freq > rhs.node.freq;
		}
		return node.times > rhs.node.times;	// ʱ��С��Ӧ�ñ���������˰�ʱ��С�ķ��ұ�
	}
};

int main() {

	priority_queue<Node> pq;
	
	// Ƶ����ͬ��ʱ�䲻ͬ
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

	// Ƶ�ʲ�ͬ��ʱ����ͬ
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