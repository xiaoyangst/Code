#pragma once
#define BUFFER_SIZE 8192

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio.hpp>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <memory>
#include <string>
#include "json.hpp"


namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace http = beast::http;           // from <boost/beast/http.hpp>
namespace net = boost::asio;            // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>
using json = nlohmann::json;

class HttpConnection : public std::enable_shared_from_this<HttpConnection>
{
public:
	HttpConnection(tcp::socket& socket) ;
	void start();
private:
	void process_request();	// 处理请求
	void create_get_response();
	void create_post_response();
	void read_request();	// 读取（监听）请求
	void write_response();	// 回复请求
	void check_deadline();	// 超时 就关闭当前连接

private:
	tcp::socket http_socket_;
	http::request<http::dynamic_body> request_;
	http::response<http::dynamic_body> response_;
	beast::flat_buffer buffer_{ BUFFER_SIZE };
	net::steady_timer deadline_{ http_socket_.get_executor(),std::chrono::seconds(60) };
};

