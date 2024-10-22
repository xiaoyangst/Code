#include <iostream>
#include "CServer.h"
#include "header.h"


int main(int argc, char *argv[])
{

  try
  {
    auto port = static_cast<unsigned short>(8080);
    net::io_context ioc{ 1 };
    boost::asio::signal_set signals(ioc, SIGINT, SIGTERM);
    signals.async_wait([&ioc](const boost::system::error_code& error, int signal_number) {
      if (error) {
        return;
      }
      ioc.stop();
    });
    auto server = std::make_shared<CServer>(ioc, port);
    server->Start();
    ioc.run();
  }
  catch (std::exception const& e)
  {
    std::cerr << "Error: " << e.what() << std::endl;
    return EXIT_FAILURE;
  }

  return 0;
}