#pragma once
#include <boost/asio.hpp>
#include <memory>
#include <vector>
#include <iostream>
#include <thread>
#include "Singleton.h"
class AsioIOServicePool : public Singleton<AsioIOServicePool>
{
	friend class Singleton<AsioIOServicePool>;
public:
	using IOService = boost::asio::io_context;
	using Work = boost::asio::io_context::work;
	using WorkPtr = std::unique_ptr<Work>;

	~AsioIOServicePool();
	AsioIOServicePool(const AsioIOServicePool&) = delete;
	AsioIOServicePool& operator=(const AsioIOServicePool&) = delete;
	
	boost::asio::io_context& getIOService();	// ¬÷—Ø∑÷≈‰
	void Stop();	

	std::size_t getID() { return nextIOService_; }

	AsioIOServicePool(std::size_t size = std::thread::hardware_concurrency());
private:
	

	std::vector<IOService> ioServiceVec_;
	std::vector<WorkPtr> workVec_;
	std::vector<std::thread> threadVec_;
	std::size_t nextIOService_;
	std::size_t size_;
};

