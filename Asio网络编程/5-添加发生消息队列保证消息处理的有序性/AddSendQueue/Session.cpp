#include "Session.h"
#include "Server.h"

Session::Session(boost::asio::io_context& ioc, Server* server):socket_(ioc),server_(server)
{
	boost::uuids::uuid  a_uuid = boost::uuids::random_generator()();
	uuid_ = boost::uuids::to_string(a_uuid);
}

Session::~Session()
{
	std::cout << "~Session" << std::endl;
}

tcp::socket& Session::Socket()
{
	return socket_;
}


std::string& Session::getUUID() {
	return uuid_;
}


void Session::Start()
{
	memset(data_, 0, BUFFSIZE);
	std::cout << "监听 read event" << std::endl;
	socket_.async_read_some(boost::asio::buffer(data_, BUFFSIZE),	// 接收客户端发生来的数据
		std::bind(&Session::handle_read, shared_from_this(), std::placeholders::_1, std::placeholders::_2));
}

void Session::handle_read(const boost::system::error_code& error, size_t bytes_transfered)
{
	std::cout << "触发 read event" << std::endl;

	if (error) {
		std::cout << "handle_read error = " << error.message() << std::endl;
		server_->ClearSession(uuid_);
		return;
	}

	std::cout << "recv : " << data_ << std::endl;

	Send(data_, bytes_transfered);	// 返回 相同数据给 客户端

	std::cout << "继续监听 read event" << std::endl;
	memset(data_, 0, BUFFSIZE);	

	socket_.async_read_some(boost::asio::buffer(data_, BUFFSIZE),	// 接收客户端发生来的数据
		std::bind(&Session::handle_read, shared_from_this(), std::placeholders::_1, std::placeholders::_2));
}

void Session::handle_write(const boost::system::error_code& error)
{
	std::cout << "触发 write event" << std::endl;

	if (error) {
		std::cout << "handle_write error = " << error << std::endl;
		server_->ClearSession(uuid_);
		return;
	}
	std::lock_guard<std::mutex> send_lock(mtx_);
	sendQue_.pop();
	if (!sendQue_.empty()) {
		auto& msgNode = sendQue_.front();
		boost::asio::async_write(socket_, boost::asio::buffer(msgNode->data_, msgNode->max_len_),
			std::bind(&Session::handle_write, shared_from_this(), std::placeholders::_1));
	}
}

void Session::Send(char* msg, int max_len)
{
	bool pending = false;
	// 异步编程，会大量调用 Send 接口，这需要加锁保证添加消息的安全性
	std::lock_guard<std::mutex> send_lock(mtx_);
	if (!sendQue_.empty()) {	// 缓冲队列有未处理的数据，后续只加入数据，但不处理，需要等 handle_write 把上次的数据处理完
		pending = true;
	}
	sendQue_.push(std::make_shared<MsgNode>(msg, max_len));
	if (pending) {	
		return;
	}

	socket_.async_write_some(boost::asio::buffer(msg,max_len),	
		std::bind(&Session::handle_write, shared_from_this(), std::placeholders::_1));
}

