#include "HttpServer.h"

int main()
{
	try
	{
		auto const address = net::ip::make_address("127.0.0.1");
		unsigned short port = static_cast<unsigned short>(8080);

		net::io_context ioc{ 1 };

		tcp::acceptor acceptor{ ioc, {address, port} };
		tcp::socket socket{ ioc };
		std::shared_ptr<HttpServer> http_server = std::make_shared<HttpServer>(acceptor, socket);
		http_server->start();
		std::cout << "HTTP Server on Port 8080" << std::endl;
		ioc.run();
	}
	catch (std::exception const& e)
	{
		std::cerr << "Error: " << e.what() << std::endl;
		return EXIT_FAILURE;
	}
}