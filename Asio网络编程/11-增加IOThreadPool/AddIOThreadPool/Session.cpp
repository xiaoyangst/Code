
#include "Server.h"

Session::Session(boost::asio::io_context& ioc, Server* server)
		:socket_(ioc)
		,server_(server)
		,is_parseHead_(false)
		,is_close(false)
		,strand_(ioc.get_executor())
{
	boost::uuids::uuid  a_uuid = boost::uuids::random_generator()();
	uuid_ = boost::uuids::to_string(a_uuid);

	recv_head_node_ = std::make_shared<MsgNode>(HEAD_TOTAL_LEN);
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
	socket_.async_receive(boost::asio::buffer(recv_head_node_->data_, HEAD_TOTAL_LEN),	
		boost::asio::bind_executor(strand_,std::bind(&Session::handle_read, shared_from_this(), std::placeholders::_1, std::placeholders::_2)));
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

	// 解析 ID
	int msg_id = 0;
	memcpy(&msg_id, recv_head_node_->data_, HEAD_ID_LEN);
	msg_id = ntohs(msg_id);
	//std::cout << " msg_id length = " << msg_id << std::endl;
	if (msg_id > MAX_LENGTH) {	// 非法
		std::cout << "invalid id length is " << msg_id << std::endl;
		server_->ClearSession(uuid_);
		return;
	}

	// 解析 长度信息
	short data_len = 0;
	memcpy(&data_len, recv_head_node_->data_ + HEAD_ID_LEN, HEAD_DATA_LEN);
	data_len = ntohs(data_len);
	//std::cout << " data length = " << data_len << std::endl;
	if (data_len > MAX_LENGTH) {	// 非法
		std::cout << "invalid data length is " << data_len << std::endl;
		server_->ClearSession(uuid_);
		return;
	}
	recv_msg_node_ = std::make_shared<RecvNode>(data_len,msg_id);
	socket_.async_receive(boost::asio::buffer(recv_msg_node_->data_, data_len),
		boost::asio::bind_executor(strand_,std::bind(&Session::HandleReadMsg, shared_from_this(), std::placeholders::_1, std::placeholders::_2)));
}

void Session::HandleReadMsg(const boost::system::error_code& error, size_t bytes_transferred)
{
	if (error) {
		std::cout << "HandleReadMsg error = " << error << std::endl;
		Close();
		server_->ClearSession(uuid_);
		return;
	}

	
	LogicSystem::getInstance()->postMsgToQue(std::make_shared<LogicNode>(shared_from_this(), recv_msg_node_));

	socket_.async_receive(boost::asio::buffer(recv_head_node_->data_, HEAD_TOTAL_LEN),
		boost::asio::bind_executor(strand_,std::bind(&Session::handle_read, shared_from_this(), std::placeholders::_1, std::placeholders::_2)));
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
	if (!sendQue_.empty()) {
		auto& msgNode = sendQue_.front();
		boost::asio::async_write(socket_, boost::asio::buffer(msgNode->data_, msgNode->total_len_),
			boost::asio::bind_executor(strand_,std::bind(&Session::handle_write, shared_from_this(), std::placeholders::_1)));
		sendQue_.pop();
	}
}

void Session::Send(char* msg, int max_len, int msg_id)
{
	std::lock_guard<std::mutex> send_lock(mtx_);
	if (sendQue_.size() > MAX_SENDQUE) {	
		std::cout << "Session = "<< uuid_ << "Exceeding the maximum capacity of the SendQue = " << MAX_SENDQUE <<std::endl;
		return;
	}
	sendQue_.push(std::make_shared<SendNode>(msg, max_len, msg_id));
	auto& msgnode = sendQue_.front();
	socket_.async_write_some(boost::asio::buffer(msgnode->data_, msgnode->total_len_),
		boost::asio::bind_executor(strand_,std::bind(&Session::handle_write, shared_from_this(), std::placeholders::_1)));
}


void Session::Send(std::string msg, int msg_id)
{
	std::lock_guard<std::mutex> send_lock(mtx_);
	if (sendQue_.size() > MAX_SENDQUE) {
		std::cout << "Session = " << uuid_ << "Exceeding the maximum capacity of the SendQue = " << MAX_SENDQUE << std::endl;
		return;
	}
	sendQue_.push(std::make_shared<SendNode>(msg.c_str(), msg.length(), msg_id));

	auto& msgnode = sendQue_.front();
	socket_.async_write_some(boost::asio::buffer(msgnode->data_, msgnode->total_len_),
		boost::asio::bind_executor(strand_,std::bind(&Session::handle_write, shared_from_this(), std::placeholders::_1)));
}



LogicNode::LogicNode(std::shared_ptr<Session> session, std::shared_ptr<RecvNode> recvnode)
	: session_(session)
	, recvnode_(recvnode)
{
}
