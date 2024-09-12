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
	void clear();	// ������ݣ������ι���ڵ�����Ŀ���

private:
	short cur_len_;	// ���ݵ�ǰ�Ѵ���ĳ���(�Ѿ����͵����ݻ����Ѿ����յ����ݳ���)
	short total_len_;	// ���ݵ��ܳ���
	char* data_;
};

class RecvNode : public MsgNode {	// ���սڵ�
	friend class Session;
public:
	RecvNode(short max_len, short msgID);
private:
	short msgID_;
};

class SendNode : public MsgNode {	// ���ͽڵ�
	friend class Session;
public:
	SendNode(const char* msg, short max_len, short msgID);
private:
	short msgID_;
};