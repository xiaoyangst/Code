#include "Server.h"
#include "AsioIOServicePool.h"

Server::Server(boost::asio::io_context& ioc, short port):
	ioc_(ioc),
	acceptor_(ioc,tcp::endpoint(tcp::v4(),port))
{
	start_accept();
}

void Server::ClearSession(const std::string& uuid)
{
	sessionMap_.erase(uuid);
}

void Server::start_accept()
{
	// 从 IOServiceVec_ 中轮询出一个 用来监听
	std::size_t AsioIOServerID = AsioIOServicePool::getInstance()->getID();
	auto& ioc_ = AsioIOServicePool::getInstance()->getIOService();
	std::shared_ptr<Session> new_session = std::make_shared<Session>(ioc_,this);
	std::cout << "AsioIOServicePool ID = " << AsioIOServerID<<" manange Session "<< new_session->getUUID();
	acceptor_.async_accept(new_session->Socket(),
		std::bind(&Server::handle_accept, this, new_session, std::placeholders::_1));
}

void Server::handle_accept(std::shared_ptr<Session> new_session, const boost::system::error_code& error)
{
	if (!error) {
	new_session->Start();		
	sessionMap_.insert(std::make_pair(new_session->getUUID(), new_session));
	start_accept();
	}
}
