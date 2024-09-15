#include "LogicSystem.h"
#include "Session.h"
#include "MsgNode.h"
#include <iostream>

LogicSystem::~LogicSystem()
{
	isStop_ = true;
	consume_.notify_one();	// �ǵ÷����źŻ����߳�
	workThread_.join();
}
void LogicSystem::postMsgToQue(std::shared_ptr<LogicNode> msg)	// �� ��紫�� �����
{
	std::lock_guard<std::mutex> lk(mtx_);
	msgQue_.push(msg);

	if (msgQue_.size() >= 1) {
		consume_.notify_one();
	}
}


LogicSystem::LogicSystem()
	: isStop_(false)
{
	RegisterCallBacks();	// ע��ص�
	workThread_ = std::thread(&LogicSystem::dealMsg,this);
}

// �����̵߳Ĵ�������ѭ���Ӷ�����ȥ������ִ��
// ��������Ҫ�������������ͻ�������֤û�������ȡʱ��������������ѭ��ִ��
void LogicSystem::dealMsg()
{
	for (;;) {

		std::unique_lock<std::mutex> uk(mtx_);
		consume_.wait(uk, [&]() {
			return !msgQue_.empty() && !isStop_;	
			}); 


		if (isStop_) {	// ��� �����Ѿ��رգ���ʣ�µ�����ȫ�����
			while (!msgQue_.empty()) {
				auto msg_node = msgQue_.front();
				
				std::cout << "recv_msg id is"<<msg_node->recvnode_->msgID_<<std::endl;
				auto FunCall_Item = eventMap_.find(msg_node->recvnode_->msgID_);
				if (FunCall_Item != eventMap_.end()) {	// �ҵ���
					FunCall_Item->second(msg_node->session_, msg_node->recvnode_->msgID_,
						std::string(msg_node->recvnode_->data_,msg_node->recvnode_->total_len_));	
				}
				msgQue_.pop();
			}
		}

		// �������û�йرգ�˵�������п϶������ݣ�����ȡ����ִ�м��ɣ�ȡһ�������Ǳ�������ѭ���У�
		auto msg_node = msgQue_.front();
		std::cout << "recv_msg id is " << msg_node->recvnode_->msgID_ << std::endl;
		auto FunCall_Item = eventMap_.find(msg_node->recvnode_->msgID_);
		if (FunCall_Item != eventMap_.end()) {	// �ҵ���
			FunCall_Item->second(msg_node->session_, msg_node->recvnode_->msgID_,
				std::string(msg_node->recvnode_->data_, msg_node->recvnode_->total_len_));
		}
		msgQue_.pop();
	}
}

void LogicSystem::RegisterCallBacks()
{
	eventMap_[MSG_HELLO_WORD] = std::bind(&LogicSystem::HelloWordCallBack, this,
		std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
}

void LogicSystem::HelloWordCallBack(std::shared_ptr<Session> session, int msg_id, std::string msg_data)
{
	// �ظ� �ͻ�����Ϣ

	// ���� JSON ���ݣ���Ϣ�壩


	json recv_data_json = json::parse(msg_data);	// �����л�
	std::string name = recv_data_json["name"].get<std::string>();
	int studentID = recv_data_json["studentID"].get<int>();
	std::cout << "recv json ---> name = " << name << ", studentID = " << studentID << std::endl;

	session->Send(msg_data, studentID);
}
