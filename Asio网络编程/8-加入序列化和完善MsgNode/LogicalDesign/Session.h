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
	tcp::socket& Socket();	// socket 不可以被拷贝复制，所以返回引用
	void Start();
	void Close();
	std::string& getUUID();
private:
	void handle_read(const boost::system::error_code & error,size_t bytes_transfered);
	void handle_write(const boost::system::error_code& error);
	void Send(char* msg,short max_len,short msg_id);
	void Send(std::string msg, short msg_id);
	void HandleReadMsg(const boost::system::error_code& error, size_t  bytes_transferred);
	
	
	tcp::socket socket_;	// 一个会话 Session 对应一个 Socket
	char data_[MAX_LENGTH];	// 暂时以固定消息长度来传递消息，后续再进行扩展
	std::string uuid_;
	Server* server_;
	std::queue<std::shared_ptr<SendNode>> sendQue_;	// 发送缓冲队列
	std::mutex mtx_;

	// 解析数据
	std::shared_ptr<RecvNode> recv_msg_node_;	//存放数据
	std::shared_ptr<MsgNode> recv_head_node_;	//存放数据的长度头部信息
	bool is_parseHead_;	// 头部解析是否完成
	bool is_close;
};

