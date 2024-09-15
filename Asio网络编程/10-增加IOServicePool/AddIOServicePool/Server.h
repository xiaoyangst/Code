#pragma once

#include <memory>
#include <unordered_map>
#include "Session.h"

class Server
{
public:
	Server(boost::asio::io_context& ioc, short port);
	void ClearSession(const std::string& uuid);
private:
	void start_accept();
	void handle_accept(std::shared_ptr<Session> new_session,const boost::system::error_code& error);
	boost::asio::io_context& ioc_;
	tcp::acceptor acceptor_;
	
	std::unordered_map<std::string, std::shared_ptr<Session>> sessionMap_;
	
};

