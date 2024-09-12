#include "Server.h"

Server::Server(boost::asio::io_context& ioc, short port):
	ioc_(ioc),
	acceptor_(ioc,tcp::endpoint(tcp::v4(),port))
{
	start_accept();
}

void Server::start_accept()
{
	Session* new_session = new Session(ioc_);
	acceptor_.async_accept(new_session->Socket(),
		std::bind(&Server::handle_accept, this, new_session, std::placeholders::_1));
}

void Server::handle_accept(Session* new_session, const boost::system::error_code& error)
{
	if (error) {
		delete new_session;
		return;
	}

	new_session->Start();	

	start_accept();
}
