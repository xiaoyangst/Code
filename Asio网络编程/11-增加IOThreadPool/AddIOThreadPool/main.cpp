#include <iostream>
#include <string>
#include <boost/asio.hpp>

#include "Server.h"
#include "AsioIOThreadPool.h"

using namespace boost::asio;
using namespace boost;
using namespace std;
using namespace boost::asio::ip;

int main()
{
	try {
		printf("Server port 8080\n");
		auto pool = AsioThreadPool::getInstance();
		boost::asio::io_context ioc;	// 让主程序不会终止
		boost::asio::signal_set sgst(ioc, SIGINT, SIGTERM);
		sgst.async_wait([&ioc,pool](auto error,auto num) {
			ioc.stop();
			pool->Stop();
			});
		Server s(pool->getIOService(), 8080);	// 传递pool->getIOService() 就无需改变 之前（LogicalDesign） Server 的代码
		ioc.run();
	}
	catch (std::exception& e) {
		std::cerr << "Exception: " << e.what() << "\n";
	}

    return 0;
}