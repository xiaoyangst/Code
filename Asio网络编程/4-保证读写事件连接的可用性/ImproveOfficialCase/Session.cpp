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
	std::cout << "���� read event" << std::endl;
	socket_.async_read_some(boost::asio::buffer(data_, BUFFSIZE),	// ���տͻ��˷�����������
		std::bind(&Session::handle_read, shared_from_this(), std::placeholders::_1, std::placeholders::_2));
}

void Session::handle_read(const boost::system::error_code& error, size_t bytes_transfered)
{
	std::cout << "���� read event" << std::endl;

	if (error) {
		std::cout << "handle_read error = " << error.message() << std::endl;
		server_->ClearSession(this->getUUID());
		return;
	}

	std::cout << "recv : " << data_ << std::endl;

	std::cout << "�������� read event" << std::endl;
	memset(data_, 0, BUFFSIZE);	// �Ȱ�����ȫ�����
	socket_.async_read_some(boost::asio::buffer(data_, BUFFSIZE),	// ���տͻ��˷�����������
		std::bind(&Session::handle_read, shared_from_this(), std::placeholders::_1, std::placeholders::_2));

	std::cout << "���� write event" << std::endl;
	socket_.async_write_some(boost::asio::buffer("My name is ClientA"),	// �ѿͻ��˷��͵����ݻظ����ͻ���
		std::bind(&Session::handle_write, shared_from_this(), std::placeholders::_1));
}

void Session::handle_write(const boost::system::error_code& error)
{
	std::cout << "���� write event" << std::endl;

	if (error) {
		std::cout << "handle_write error = " << error << std::endl;
		server_->ClearSession(this->getUUID());
		return;
	}
}

