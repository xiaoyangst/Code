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
	tcp::socket& Socket();	// socket �����Ա��������ƣ����Է�������
	void Start();
	std::string& getUUID();
private:
	void handle_read(const boost::system::error_code & error,size_t bytes_transfered);
	void handle_write(const boost::system::error_code& error);
	void Send(char* msg,int max_len);
	tcp::socket socket_;	// һ���Ự Session ��Ӧһ�� Socket
	char data_[BUFFSIZE];	// ��ʱ�Թ̶���Ϣ������������Ϣ�������ٽ�����չ
	std::string uuid_;
	Server* server_;
	std::queue<std::shared_ptr<MsgNode>> sendQue_;	// ���ͻ������
	std::mutex mtx_;
};

