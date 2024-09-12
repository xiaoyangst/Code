#pragma once

#define MAX_SENDQUE 300 

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
	void Close();
	std::string& getUUID();
private:
	void handle_read(const boost::system::error_code & error,size_t bytes_transfered);
	void handle_write(const boost::system::error_code& error);
	void Send(char* msg,short max_len,short msg_id);
	void Send(std::string msg, short msg_id);
	void HandleReadMsg(const boost::system::error_code& error, size_t  bytes_transferred);
	
	
	tcp::socket socket_;	// һ���Ự Session ��Ӧһ�� Socket
	char data_[MAX_LENGTH];	// ��ʱ�Թ̶���Ϣ������������Ϣ�������ٽ�����չ
	std::string uuid_;
	Server* server_;
	std::queue<std::shared_ptr<SendNode>> sendQue_;	// ���ͻ������
	std::mutex mtx_;

	// ��������
	std::shared_ptr<RecvNode> recv_msg_node_;	//�������
	std::shared_ptr<MsgNode> recv_head_node_;	//������ݵĳ���ͷ����Ϣ
	bool is_parseHead_;	// ͷ�������Ƿ����
	bool is_close;
};

