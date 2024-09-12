#include "MsgNode.h"

MsgNode::MsgNode(char* msg, int max_len)
	: cur_len_(0)
	, total_len_(max_len + HEAD_LENGTH)
{
	data_ = new char[total_len_ + 1]();
	// ��ʱ��װΪ ͷ��+����
	memcpy(data_, &max_len, HEAD_LENGTH);
	memcpy(data_ + HEAD_LENGTH, msg, max_len);
	data_[total_len_] = '\0';
}

MsgNode::MsgNode(int max_len)
	: cur_len_(0)
	, total_len_(max_len)
{
	data_ = new char[total_len_ + 1]();
}

MsgNode::~MsgNode()
{
	delete[] data_;
}

void MsgNode::clear()
{
	memset(data_, 0, total_len_);
	cur_len_ = 0;
}




/*

��¼��д����������������ɻ�

----------------------------------------------------------------------------------

question 1 ��data_ = new char[max_len]() �� data_ = new char[max_len] �к�����

answer��
	
	��ʼ��Ϊ0 : data_ = new char[max_len](); ��ʼ�������е�����Ԫ��Ϊ 0

	δ��ʼ��  : data_ = new char[max_len]; �����ڴ浫�������ݽ��г�ʼ��

----------------------------------------------------------------------------------

*/