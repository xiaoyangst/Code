#include "HttpServer.h"
#include "HttpConnection.h"

HttpServer::~HttpServer()
{
	std::cout << "~HttpServer()" << std::endl;
}

void HttpServer::start()
{
	http_accept_.async_accept(http_socket_, [&](auto err) {
		if (!err) {
			std::make_shared<HttpConnection>(http_socket_)->start();	
			start();
		}
		});
}

HttpServer::HttpServer(tcp::acceptor& accept, tcp::socket& socket)
	:http_accept_(std::move(accept)), http_socket_(std::move(socket))
{

}


/*

Question 1: std::make_shared<HttpConnection>(http_socket_)->start() 为何不必管理死活 ？

	答：调用 HttpConnection 的 start 之后，该连接 就会在内部监听该连接的读写事件，不再由 HttpServer 管理
*/