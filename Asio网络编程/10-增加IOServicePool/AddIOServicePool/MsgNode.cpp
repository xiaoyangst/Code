#include "MsgNode.h"

MsgNode::MsgNode(short max_len)
	: cur_len_(0)
	, total_len_(max_len)
{
	data_ = new char[total_len_ + 1]();
	data_[total_len_] = '\0';
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

RecvNode::RecvNode(short max_len, short msgID)
	: MsgNode(max_len)
	, msgID_(msgID)
{

}

SendNode::SendNode(const char* msg, short max_len, short msgID)	
	: MsgNode(max_len + HEAD_TOTAL_LEN)
	, msgID_(msgID)
{
	// 先转换为 网络字节序

	short msgID_host = htons(msgID);
	short max_len_host = htons(max_len);

	// 封装

	memcpy(data_, &msgID_host, HEAD_ID_LEN);
	memcpy(data_ + HEAD_ID_LEN, &max_len_host, HEAD_ID_LEN);
	memcpy(data_ + HEAD_ID_LEN + HEAD_ID_LEN, msg, max_len);
}



/*

记录在写代码过程中遇到的疑惑：

----------------------------------------------------------------------------------

question 1 ：data_ = new char[max_len]() 和 data_ = new char[max_len] 有何区别？

answer：
	
	初始化为0 : data_ = new char[max_len](); 初始化数组中的所有元素为 0

	未初始化  : data_ = new char[max_len]; 分配内存但不对内容进行初始化

----------------------------------------------------------------------------------

question 2 ：为什么子类需要调用父类的构造函数 ？

answer：

	子类继承父类的成员变量，需要调用父类的构造函数进行初始化操作

	否则，父类的数据成员可能会处于未定义状态，从而导致程序错误或不稳定
----------------------------------------------------------------------------------

*/


