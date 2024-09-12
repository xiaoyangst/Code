#pragma once
#define HEAD_LENGTH 2
#define MAX_LENGTH  1024 * 2
#include <string.h>

class MsgNode
{
	friend class Session;
public:
	MsgNode(char* msg, int max_len);	// 构造发送节点
	MsgNode(int max_len);	// 构造接收节点
	~MsgNode();
	void clear();	// 清空数据，避免多次构造节点带来的开销

private:
	int cur_len_;	// 数据当前已处理的长度(已经发送的数据或者已经接收的数据长度)
	int total_len_;	// 数据的总长度
	char* data_;
};

