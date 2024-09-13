#include "LogicSystem.h"
#include "Session.h"
#include "MsgNode.h"
#include <iostream>

LogicSystem::~LogicSystem()
{
	isStop_ = true;
	consume_.notify_one();	// 记得发出信号唤醒线程
	workThread_.join();
}
void LogicSystem::postMsgToQue(std::shared_ptr<LogicNode> msg)	// 给 外界传递 任务的
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
	RegisterCallBacks();	// 注册回调
	workThread_ = std::thread(&LogicSystem::dealMsg,this);
}

// 工作线程的处理函数，循环从队列中去任务来执行
// 但我们需要利用条件变量和互斥锁保证没有任务可取时阻塞，而不返回循环执行
void LogicSystem::dealMsg()
{
	for (;;) {

		std::unique_lock<std::mutex> uk(mtx_);
		consume_.wait(uk, [&]() {
			return !msgQue_.empty() && !isStop_;	
			}); 


		if (isStop_) {	// 如果 服务已经关闭，把剩下的任务全部完成
			while (!msgQue_.empty()) {
				auto msg_node = msgQue_.front();
				
				std::cout << "recv_msg id is"<<msg_node->recvnode_->msgID_<<std::endl;
				auto FunCall_Item = eventMap_.find(msg_node->recvnode_->msgID_);
				if (FunCall_Item != eventMap_.end()) {	// 找到了
					FunCall_Item->second(msg_node->session_, msg_node->recvnode_->msgID_,
						std::string(msg_node->recvnode_->data_,msg_node->recvnode_->total_len_));	
				}
				msgQue_.pop();
			}
		}

		// 如果服务没有关闭，说明队列中肯定有数据，正常取出来执行即可（取一个，我们本来就在循环中）
		auto msg_node = msgQue_.front();
		std::cout << "recv_msg id is " << msg_node->recvnode_->msgID_ << std::endl;
		auto FunCall_Item = eventMap_.find(msg_node->recvnode_->msgID_);
		if (FunCall_Item != eventMap_.end()) {	// 找到了
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
	// 回复 客户端消息

	// 解析 JSON 数据（消息体）


	json recv_data_json = json::parse(msg_data);	// 反序列化
	std::string name = recv_data_json["name"].get<std::string>();
	int studentID = recv_data_json["studentID"].get<int>();
	std::cout << "recv json ---> name = " << name << ", studentID = " << studentID << std::endl;

	session->Send(msg_data, studentID);
}
