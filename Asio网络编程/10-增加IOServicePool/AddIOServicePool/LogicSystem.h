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
	std::thread workThread_;	// �����̣߳��������߼�������ȡ���ݲ�ִ�лص�����
	std::queue<std::shared_ptr<LogicNode>> msgQue_;
	std::mutex mtx_;
	std::condition_variable consume_;
	bool isStop_;	// ��ʾ�յ��ⲿ��ֹͣ�źţ��߼���Ҫ��ֹ�����̲߳������˳�
	std::map<int, eventCallBack> eventMap_;
};

