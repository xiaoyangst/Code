#include <iostream>
#include <string>
#include <boost/asio.hpp>

#include "Server.h"
#include "AsioIOServicePool.h"

using namespace boost::asio;
using namespace boost;
using namespace std;
using namespace boost::asio::ip;

int main()
{
	try {
		printf("Server port 8080\n");
		auto pool = AsioIOServicePool::getInstance();
		boost::asio::io_context ioc;
		boost::asio::signal_set sgst(ioc, SIGINT, SIGTERM);
		sgst.async_wait([&ioc,pool](auto error,auto num) {
			ioc.stop();
			pool->Stop();
			});
		Server s(ioc, 8080);
		ioc.run();
	}
	catch (std::exception& e) {
		std::cerr << "Exception: " << e.what() << "\n";
	}

    return 0;
}