#include "AsioIOThreadPool.h"

AsioThreadPool::AsioThreadPool(std::size_t size)
	:threadVec_(size),nextIOService_(0),size_(size)
	,work_(new Work(ioService_))
{

	// 一个 线程上 启动同一个 ioService
	for (std::size_t i = 0; i < size; i++) {
		threadVec_.emplace_back([this,i]() {
			std::cout << "thread" << i << " Start " << std::endl;
			ioService_.run();
			});
	}
}

AsioThreadPool::~AsioThreadPool()
{
	std::cout << "~AsioIOServicePool" << std::endl;
}

boost::asio::io_context& AsioThreadPool::getIOService()
{
	return ioService_;
}

void AsioThreadPool::Stop()
{
	ioService_.stop();
	work_.reset();
	for (auto& thread : threadVec_) {
		thread.join();
	}
}