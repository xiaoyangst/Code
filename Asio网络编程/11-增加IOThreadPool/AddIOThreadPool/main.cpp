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
		boost::asio::io_context ioc;	// �������򲻻���ֹ
		boost::asio::signal_set sgst(ioc, SIGINT, SIGTERM);
		sgst.async_wait([&ioc,pool](auto error,auto num) {
			ioc.stop();
			pool->Stop();
			});
		Server s(pool->getIOService(), 8080);	// ����pool->getIOService() ������ı� ֮ǰ��LogicalDesign�� Server �Ĵ���
		ioc.run();
	}
	catch (std::exception& e) {
		std::cerr << "Exception: " << e.what() << "\n";
	}

    return 0;
}