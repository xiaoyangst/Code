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
	// ��ת��Ϊ �����ֽ���

	short msgID_host = htons(msgID);
	short max_len_host = htons(max_len);

	// ��װ

	memcpy(data_, &msgID_host, HEAD_ID_LEN);
	memcpy(data_ + HEAD_ID_LEN, &max_len_host, HEAD_ID_LEN);
	memcpy(data_ + HEAD_ID_LEN + HEAD_ID_LEN, msg, max_len);
}



/*

��¼��д����������������ɻ�

----------------------------------------------------------------------------------

question 1 ��data_ = new char[max_len]() �� data_ = new char[max_len] �к�����

answer��
	
	��ʼ��Ϊ0 : data_ = new char[max_len](); ��ʼ�������е�����Ԫ��Ϊ 0

	δ��ʼ��  : data_ = new char[max_len]; �����ڴ浫�������ݽ��г�ʼ��

----------------------------------------------------------------------------------

question 2 ��Ϊʲô������Ҫ���ø���Ĺ��캯�� ��

answer��

	����̳и���ĳ�Ա��������Ҫ���ø���Ĺ��캯�����г�ʼ������

	���򣬸�������ݳ�Ա���ܻᴦ��δ����״̬���Ӷ����³��������ȶ�
----------------------------------------------------------------------------------

*/


