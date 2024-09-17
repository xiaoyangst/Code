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
	因为 WebSocket 是长连接，所以用 map 容器存储起来，并且是智能指针管理

	而 Http 是短连接，也就不需要对连接进行管理，只需要写一个定时器，周期性检查是否超时，超时就关闭连接即可
	
*/
