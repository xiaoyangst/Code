#pragma once
#include <boost/asio.hpp>
#include <memory>
#include <vector>
#include <iostream>
#include <thread>
#include "Singleton.h"
class AsioThreadPool : public Singleton<AsioThreadPool>
{
	friend class Singleton<AsioThreadPool>;
public:
	using IOService = boost::asio::io_context;
	using Work = boost::asio::io_context::work;
	using WorkPtr = std::unique_ptr<Work>;

	~AsioThreadPool();
	AsioThreadPool(const AsioThreadPool&) = delete;
	AsioThreadPool& operator=(const AsioThreadPool&) = delete;
	
	boost::asio::io_context& getIOService();	
	void Stop();	

	std::size_t getID() { return nextIOService_; }

	AsioThreadPool(std::size_t size = std::thread::hardware_concurrency());
private:
	

	IOService ioService_;
	WorkPtr work_;
	std::vector<std::thread> threadVec_;
	std::size_t nextIOService_;
	std::size_t size_;
};

