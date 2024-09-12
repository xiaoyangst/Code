#define BUFFSIZE 100

#include <iostream>
#include <string>
#include <boost/asio.hpp>
using namespace boost::asio;
using namespace boost;
using namespace std;
using namespace boost::asio::ip;

// 同步通信服务器

void Synchronous_comm_server() {
    const int BACKLOG_SIZE = 30;
    int server_port = 8080;
    tcp::endpoint s_ep(address_v4::any(), server_port);

    cout << "server is starting, ip = " << s_ep.address() << " port = " << s_ep.port() << endl;

    // 创建用于接收客户端连接的 socket 类型，即 acceptor
    io_context ic;
    tcp protocol = tcp::v4();
    tcp::acceptor acceptor(ic);
    acceptor.open(protocol);

    // 绑定、监听、接收客户端连接
    try {
        acceptor.bind(s_ep);
        acceptor.listen(BACKLOG_SIZE);

        tcp::socket c_socket(ic);
        acceptor.accept(c_socket);

        // 接收客户端信息
        char recv_buf[BUFFSIZE];
        memset(recv_buf, 0, BUFFSIZE);
        int recv_length = c_socket.receive(asio::buffer(recv_buf, BUFFSIZE));
        if (recv_length <= 0) {
            cout << "recv failed!!!" << endl;
        }
        else {
            recv_buf[recv_length] = '\0'; // 终止字符串
            cout << "recv_buf = " << recv_buf << endl;
        }

        // 回复客户端信息
        std::string buf = "My name is ServerA";
        int send_length = c_socket.send(buffer(buf.c_str(), buf.length()));
        if (send_length <= 0) {
            cout << "send failed!!!" << endl;
        }
    }
    catch (const std::exception& e) {
        cout << "Exception: " << e.what() << endl;
    }
}

int main() {
    Synchronous_comm_server();


    getchar();
    return 0;
}
