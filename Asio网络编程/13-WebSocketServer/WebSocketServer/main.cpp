#include "WebSocketServer.h"

int main()
{
	net::io_context ioc;
	WebSocketServer server(ioc, 8080);
	server.StartAccept();
	ioc.run();
}