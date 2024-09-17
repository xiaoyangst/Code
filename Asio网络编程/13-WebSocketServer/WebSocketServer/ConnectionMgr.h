#pragma once
#include "Connection.h"
#include <unordered_map>
class ConnectionMgr
{
public:
	static ConnectionMgr& GetInstance();
	void AddConnection(std::shared_ptr<Connection> conptr);
	void RmvConnection(std::string uuid);
private:
	ConnectionMgr(const ConnectionMgr&) = delete;
	ConnectionMgr& operator = (const ConnectionMgr&) = delete;
	ConnectionMgr();
	std::unordered_map<std::string, std::shared_ptr<Connection>>  map_cons_;
};

/*
	��Ϊ WebSocket �ǳ����ӣ������� map �����洢����������������ָ�����

	�� Http �Ƕ����ӣ�Ҳ�Ͳ���Ҫ�����ӽ��й���ֻ��Ҫдһ����ʱ���������Լ���Ƿ�ʱ����ʱ�͹ر����Ӽ���
	
*/
