#pragma once
#define HEAD_LENGTH 2
#define MAX_LENGTH  1024 * 2
#include <string.h>

class MsgNode
{
	friend class Session;
public:
	MsgNode(char* msg, int max_len);	// ���췢�ͽڵ�
	MsgNode(int max_len);	// ������սڵ�
	~MsgNode();
	void clear();	// ������ݣ������ι���ڵ�����Ŀ���

private:
	int cur_len_;	// ���ݵ�ǰ�Ѵ���ĳ���(�Ѿ����͵����ݻ����Ѿ����յ����ݳ���)
	int total_len_;	// ���ݵ��ܳ���
	char* data_;
};

