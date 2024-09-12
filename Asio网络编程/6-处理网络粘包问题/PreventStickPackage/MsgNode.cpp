#include "MsgNode.h"

MsgNode::MsgNode(char* msg, int max_len)
	: cur_len_(0)
	, total_len_(max_len + HEAD_LENGTH)
{
	data_ = new char[total_len_ + 1]();
	// 暂时封装为 头部+数据
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

记录在写代码过程中遇到的疑惑：

----------------------------------------------------------------------------------

question 1 ：data_ = new char[max_len]() 和 data_ = new char[max_len] 有何区别？

answer：
	
	初始化为0 : data_ = new char[max_len](); 初始化数组中的所有元素为 0

	未初始化  : data_ = new char[max_len]; 分配内存但不对内容进行初始化

----------------------------------------------------------------------------------

*/