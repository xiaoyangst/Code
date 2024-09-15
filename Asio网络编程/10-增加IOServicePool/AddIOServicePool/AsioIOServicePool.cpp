#include "AsioIOServicePool.h"

AsioIOServicePool::AsioIOServicePool(std::size_t size)
	:ioServiceVec_(size),workVec_(size),threadVec_(size),nextIOService_(0),size_(size)
{
	// 一个 work 负责唤醒 一个 ioService
	for (std::size_t i = 0; i < size; i++) {
		workVec_[i] = std::unique_ptr<Work>(new Work(ioServiceVec_[i]));
	}

	// 一个 线程上 启动一个 ioService
	for (std::size_t i = 0; i < size; i++) {
		threadVec_.emplace_back([this,i]() {
			std::this_thread::sleep_for(std::chrono::milliseconds(300));
			std::cout << "ioService"<< i <<" Start " << std::endl;
			ioServiceVec_[i].run();
			});
	}
}

AsioIOServicePool::~AsioIOServicePool()
{
	std::cout << "~AsioIOServicePool" << std::endl;
}

boost::asio::io_context& AsioIOServicePool::getIOService()
{
	nextIOService_ = nextIOService_ % size_;
	nextIOService_++;

	return ioServiceVec_[nextIOService_];
}

void AsioIOServicePool::Stop()
{
	// 先关闭 work
	for (auto& work : workVec_) {
		work.reset();
	}

	// 再关闭 thread
	for (auto& thread : threadVec_) {
		thread.join();
	}
}