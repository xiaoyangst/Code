#include "Session.h"
#include "Server.h"

Session::Session(boost::asio::io_context& ioc, Server* server)
		:socket_(ioc)
		,server_(server)
		,is_parseHead_(false)
		,is_close(false)
{
	boost::uuids::uuid  a_uuid = boost::uuids::random_generator()();
	uuid_ = boost::uuids::to_string(a_uuid);

	recv_head_node_ = std::make_shared<MsgNode>(HEAD_LENGTH);
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
	memset(data_, 0, MAX_LENGTH);
	recv_head_node_->clear();
	socket_.async_receive(boost::asio::buffer(recv_head_node_->data_, HEAD_LENGTH),	// 读完 HEAD_LENGTH 字节才触发读回调
		std::bind(&Session::handle_read, shared_from_this(), std::placeholders::_1, std::placeholders::_2));
}

void Session::Close()
{
	socket_.close();
	is_close = true;
}

void Session::handle_read(const boost::system::error_code& error, size_t bytes_transfered)
{
	if (error) {
		std::cout << "handle_read error = " << error.message() << std::endl;
		Close();
		server_->ClearSession(uuid_);
		return;
	}

	int data_len = 0;
	memcpy(&data_len, recv_head_node_->data_, HEAD_LENGTH);
	std::cout << " data length = " << data_len << std::endl;

	if (data_len > MAX_LENGTH) {	// 非法
		std::cout << "invalid data length is " << data_len << std::endl;
		server_->ClearSession(uuid_);
		return;
	}

	recv_msg_node_ = std::make_shared<MsgNode>(data_len);
	socket_.async_receive(boost::asio::buffer(recv_msg_node_->data_, data_len),	// 读完 HEAD_LENGTH 字节才触发读回调
		std::bind(&Session::HandleReadMsg, shared_from_this(), std::placeholders::_1, std::placeholders::_2));


}

void Session::handle_write(const boost::system::error_code& error)
{
	if (error) {
		std::cout << "handle_write error = " << error << std::endl;
		Close();
		server_->ClearSession(uuid_);
		return;
	}
	std::lock_guard<std::mutex> send_lock(mtx_);
	sendQue_.pop();
	if (!sendQue_.empty()) {
		auto& msgNode = sendQue_.front();
		boost::asio::async_write(socket_, boost::asio::buffer(msgNode->data_, msgNode->total_len_),
			std::bind(&Session::handle_write, shared_from_this(), std::placeholders::_1));
	}
}

void Session::Send(char* msg, int max_len)
{
	bool pending = false;
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

void Session::HandleReadMsg(const boost::system::error_code& error, size_t bytes_transferred)
{
	if (error) {
		std::cout << "HandleReadMsg error = " << error << std::endl;
		Close();
		server_->ClearSession(uuid_);
		return;
	}

	std::cout << "recv data = " << recv_msg_node_->data_ << std::endl;

	Send(recv_msg_node_->data_,recv_msg_node_->total_len_);

	socket_.async_receive(boost::asio::buffer(recv_head_node_->data_, HEAD_LENGTH),	// 读完 HEAD_LENGTH 字节才触发读回调
		std::bind(&Session::handle_read, shared_from_this(), std::placeholders::_1, std::placeholders::_2));
}

