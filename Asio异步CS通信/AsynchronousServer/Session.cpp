#include "Session.h"

Session::Session(boost::asio::io_context& ioc):socket_(ioc)
{
}

tcp::socket& Session::Socket()
{
	return socket_;
}

void Session::Start()
{
	memset(data_, 0, BUFFSIZE);
	socket_.async_read_some(boost::asio::buffer(data_, BUFFSIZE),	// ���տͻ��˷�����������
		std::bind(&Session::handle_read, this, std::placeholders::_1, std::placeholders::_2));
}

void Session::handle_read(const boost::system::error_code& error, size_t bytes_transfered)
{
	if (error) {
		delete this;	// ���ڰ�ȫ����
		return;
	}

	std::cout << "recv : " << data_ << std::endl;

	socket_.async_write_some(boost::asio::buffer(data_, bytes_transfered),	// �ѿͻ��˷��͵����ݻظ����ͻ���
		std::bind(&Session::handle_write, this, std::placeholders::_1));
}

void Session::handle_write(const boost::system::error_code& error)
{
	if (error) {
		delete this;	// ���ڰ�ȫ����
		return;
	}

	memset(data_, 0, BUFFSIZE);	// �Ȱ�����ȫ�����
	socket_.async_read_some(boost::asio::buffer(data_, BUFFSIZE),	// ���տͻ��˷�����������
		std::bind(&Session::handle_read, this, std::placeholders::_1, std::placeholders::_2));
}

