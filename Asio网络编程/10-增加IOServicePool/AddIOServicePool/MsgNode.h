#pragma once

#include <string.h>
#include <boost/asio.hpp>
#include "public.h"

class MsgNode
{
	friend class Session;
	friend class LogicNode;
	friend class LogicSystem;
	friend class RecvNode;
	friend class SendNode;
public:
	MsgNode(short max_len);	
	~MsgNode();
	void clear();	// ������ݣ������ι���ڵ�����Ŀ���
protected:
	short cur_len_;	// ���ݵ�ǰ�Ѵ���ĳ���(�Ѿ����͵����ݻ����Ѿ����յ����ݳ���)
	short total_len_;	// ���ݵ��ܳ���
	char* data_;
};

class RecvNode : public MsgNode {	// ���սڵ�
	friend class Session;
	friend class LogicNode;
	friend class LogicSystem;
public:
	RecvNode(short max_len, short msgID);
private:
	short msgID_;
};

class SendNode : public MsgNode {	// ���ͽڵ�
	friend class Session;
	friend class LogicNode;
	friend class LogicSystem;
public:
	SendNode(const char* msg, short max_len, short msgID);
private:
	short msgID_;
};