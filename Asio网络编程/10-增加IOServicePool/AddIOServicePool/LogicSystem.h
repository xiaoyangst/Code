#pragma once
#include <queue>
#include <map>
#include <memory>
#include <functional>
#include "Singleton.h"
#include "Session.h"
#include "public.h"


class LogicSystem : public Singleton<LogicSystem>
{
	friend class Singleton<LogicSystem>;
	friend class Session;
public:
	using eventCallBack = std::function<void(std::shared_ptr<Session>, int msg_id, std::string msg_data)>;
	~LogicSystem();
	void postMsgToQue(std::shared_ptr<LogicNode> msg);
	LogicSystem();	

private:
	
	void dealMsg();
	void RegisterCallBacks();
	void HelloWordCallBack(std::shared_ptr<Session> session, int msg_id, std::string msg_data);
	std::thread workThread_;	// 工作线程，用来从逻辑队列中取数据并执行回调函数
	std::queue<std::shared_ptr<LogicNode>> msgQue_;
	std::mutex mtx_;
	std::condition_variable consume_;
	bool isStop_;	// 表示收到外部的停止信号，逻辑类要中止工作线程并优雅退出
	std::map<int, eventCallBack> eventMap_;
};

