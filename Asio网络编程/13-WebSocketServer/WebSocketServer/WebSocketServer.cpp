#include "WebSocketServer.h"


WebSocketServer::WebSocketServer(net::io_context& ioc, unsigned short port)
	:acceptor_(ioc, net::ip::tcp::endpoint(net::ip::tcp::v4(), port))
	,ioc_(ioc)
{
	std::cout << "Server start on port : " << port << std::endl;
}

// �ⲿ�ִ����֮ǰд HttpServer û������

void WebSocketServer::StartAccept() {
	auto conn = std::make_shared<Connection>(ioc_);
	acceptor_.async_accept(conn->GetSocket(), [this,conn](error_code err) {
		try {
			if (!err) {
				conn->AsyncAccept();
			}
			else {
				std::cout << "async_accept err is " << err.what() << std::endl;
			}
			StartAccept();	
		}
		catch (std::exception& ep) {
			std::cout << "async_accept exception is " << ep.what() << std::endl;
		}
		});
}