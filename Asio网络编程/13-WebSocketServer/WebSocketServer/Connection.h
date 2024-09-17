#pragma once

#include <iostream>
#include <boost/beast.hpp>
#include <boost/asio.hpp>
#include <memory.h>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <queue>
#include <mutex>

namespace net = boost::asio;
namespace beast = boost::beast;
using namespace boost::beast;
using namespace boost::beast::websocket;

class Connection : public std::enable_shared_from_this<Connection>
{
public:
	Connection(net::io_context& ioc);
	void AsyncAccept();
	std::string  GetUid();
	void Start();
	void AsyncSend(std::string msg);
	void SendCallBack(std::string msg);
	boost::asio::ip::tcp::socket& GetSocket();
private:
	net::io_context& ioc_;
	std::string uuid_;
	flat_buffer recv_buffer_;
	std::queue<std::string> send_que_;
	std::mutex send_mtx_;
	std::unique_ptr<stream<tcp_stream>> ws_ptr_;
};

