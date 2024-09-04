#pragma once

#include "Session.h"

class Server
{
public:
	Server(boost::asio::io_context& ioc, short port);
private:
	void start_accept();
	void handle_accept(Session* new_session,const boost::system::error_code& error);
	boost::asio::io_context& ioc_;
	tcp::acceptor acceptor_;
};

