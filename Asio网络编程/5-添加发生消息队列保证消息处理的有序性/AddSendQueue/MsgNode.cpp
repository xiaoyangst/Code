#include "MsgNode.h"

MsgNode::MsgNode(char* msg, int max_len)
{
	data_ = new char[max_len];
	memcpy(data_, msg, max_len);
}

MsgNode::~MsgNode()
{
	delete[] data_;
}
