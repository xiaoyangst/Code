#include "Session.h"
#include "Server.h"

Session::Session(boost::asio::io_context& ioc, Server* server):socket_(ioc),server_(server)
{
	boost::uuids::uuid  a_uuid = boost::uuids::random_generator()();
	uuid_ = boost::uuids::to_string(a_uuid);
}

Session::~Session()
{
	std::cout << "count = " << server_->test(uuid_) << std::endl;
	std::cout << "~Session" << std::endl;
}

tcp::socket& Session::Socket()
{
	return socket_;
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
		server_->ClearSession(this->getUUID());
		return;
	}

	std::cout << "recv : " << data_ << std::endl;

	std::cout << "继续监听 read event" << std::endl;
	memset(data_, 0, BUFFSIZE);	// 先把数据全部清空
	socket_.async_read_some(boost::asio::buffer(data_, BUFFSIZE),	// 接收客户端发生来的数据
		std::bind(&Session::handle_read, shared_from_this(), std::placeholders::_1, std::placeholders::_2));

	std::cout << "监听 write event" << std::endl;
	socket_.async_write_some(boost::asio::buffer("My name is ClientA"),	// 把客户端发送的数据回复给客户端
		std::bind(&Session::handle_write, shared_from_this(), std::placeholders::_1));
}

void Session::handle_write(const boost::system::error_code& error)
{
	std::cout << "触发 write event" << std::endl;

	if (error) {
		std::cout << "handle_write error = " << error << std::endl;
		server_->ClearSession(this->getUUID());
		return;
	}
}

