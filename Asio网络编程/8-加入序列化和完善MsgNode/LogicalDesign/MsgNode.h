#pragma once
#define HEAD_ID_LEN 2
#define HEAD_DATA_LEN 2
#define HEAD_TOTAL_LEN 4
#define MAX_LENGTH  1024 * 2
#include <string.h>
#include <boost/asio.hpp>

class RecvNode;
class SendNode;
class MsgNode
{
	friend class Session;
	friend class RecvNode;
	friend class SendNode;
public:
	MsgNode(short max_len);	
	~MsgNode();
	void clear();	// 清空数据，避免多次构造节点带来的开销

private:
	short cur_len_;	// 数据当前已处理的长度(已经发送的数据或者已经接收的数据长度)
	short total_len_;	// 数据的总长度
	char* data_;
};

class RecvNode : public MsgNode {	// 接收节点
	friend class Session;
public:
	RecvNode(short max_len, short msgID);
private:
	short msgID_;
};

class SendNode : public MsgNode {	// 发送节点
	friend class Session;
public:
	SendNode(const char* msg, short max_len, short msgID);
private:
	short msgID_;
};