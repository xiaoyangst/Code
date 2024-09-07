#pragma once
#define BUFFSIZE 1024

#include <iostream>
#include <memory>
#include <boost/asio.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <queue>
#include <mutex>
#include "MsgNode.h"

using namespace boost::asio::ip;

class Server;
class Session : public std::enable_shared_from_this<Session>
{
public:
	Session(boost::asio::io_context& ioc, Server* server);
	~Session();
	tcp::socket& Socket();	// socket 不可以被拷贝复制，所以返回引用
	void Start();
	std::string& getUUID();
private:
	void handle_read(const boost::system::error_code & error,size_t bytes_transfered);
	void handle_write(const boost::system::error_code& error);
	void Send(char* msg,int max_len);
	tcp::socket socket_;	// 一个会话 Session 对应一个 Socket
	char data_[BUFFSIZE];	// 暂时以固定消息长度来传递消息，后续再进行扩展
	std::string uuid_;
	Server* server_;
	std::queue<std::shared_ptr<MsgNode>> sendQue_;	// 发送缓冲队列
	std::mutex mtx_;
};

