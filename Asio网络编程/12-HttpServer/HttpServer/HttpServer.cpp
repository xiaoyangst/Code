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

Question 1: std::make_shared<HttpConnection>(http_socket_)->start() Ϊ�β��ع������� ��

	�𣺵��� HttpConnection �� start ֮�󣬸����� �ͻ����ڲ����������ӵĶ�д�¼��������� HttpServer ����
*/