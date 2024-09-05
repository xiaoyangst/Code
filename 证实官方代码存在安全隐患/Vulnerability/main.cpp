#include <iostream>
#include <string>
#include <boost/asio.hpp>

#include "Server.h"

using namespace boost::asio;
using namespace boost;
using namespace std;
using namespace boost::asio::ip;

int main()
{
	try {
		printf("Server port 8080\n");
		boost::asio::io_context ioc;
		Server s(ioc, 8080);
		ioc.run();
	}
	catch (std::exception& e) {
		std::cerr << "Exception: " << e.what() << "\n";
	}

    return 0;
}