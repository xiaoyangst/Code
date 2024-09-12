#pragma once

#include <string.h>

class MsgNode
{
	friend class Session;
public:
	MsgNode(char* msg, int max_len);
	~MsgNode();

private:
	int cur_len_;	// ���ݵ�ǰ�Ѵ���ĳ���(�Ѿ����͵����ݻ����Ѿ����յ����ݳ���)
	int max_len_;	// ���ݵ��ܳ���
	char* data_;
};

