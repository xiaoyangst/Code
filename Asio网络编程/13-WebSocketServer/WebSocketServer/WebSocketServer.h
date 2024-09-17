#pragma once

#include "Connection.h"

class WebSocketServer
{
public:
	WebSocketServer(const WebSocketServer&) = delete;
	WebSocketServer& operator = (const WebSocketServer&) = delete;

	WebSocketServer(net::io_context& ioc, unsigned short port);
	void StartAccept();
private:
	net::ip::tcp::acceptor acceptor_;
	net::io_context& ioc_;
};

