#pragma once

#include <string.h>

class MsgNode
{
	friend class Session;
public:
	MsgNode(char* msg, int max_len);
	~MsgNode();

private:
	int cur_len_;	// 数据当前已处理的长度(已经发送的数据或者已经接收的数据长度)
	int max_len_;	// 数据的总长度
	char* data_;
};

