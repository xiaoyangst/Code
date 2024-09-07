#define BUFFSIZE 100

#include <iostream>
#include <string>
#include <boost/asio.hpp>
using namespace boost::asio;
using namespace boost;
using namespace std;
using namespace boost::asio::ip;

// 同步通信客户端

void Synchronous_comm_client() {
    //  创建端点，记录服务器的信息   
    string ip = "127.0.0.1";
    int port = 8080;
    tcp::endpoint ep(address::from_string(ip), port);

    //  连接到服务器
    io_context ic;
    tcp::socket sock(ic, ep.protocol());

    try {
        sock.connect(ep);
        printf("连接成功...\n");

        // 发送消息给服务器
        std::string buf = "My name is ClientA";
        int send_length = sock.send(buffer(buf.c_str(), buf.length()));
        if (send_length <= 0) {
            cout << "send failed!!!" << endl;
        }

        // 接收服务器的消息
        char recv_buf[BUFFSIZE];
        memset(recv_buf, 0, BUFFSIZE);
        int recv_length = sock.receive(asio::buffer(recv_buf, BUFFSIZE));
        if (recv_length <= 0) {
            cout << "recv failed!!!" << endl;
        }
        else {
            recv_buf[recv_length] = '\0'; // 确保字符串终止
            cout << "recv_buf = " << recv_buf << endl;
        }

        // 再次发送消息
        send_length = sock.send(buffer(buf.c_str(), buf.length()));
        if (send_length <= 0) {
            cout << "send failed!!!" << endl;
        }

        getchar();

    }
    catch (const std::exception& e) {
        cout << "Exception: " << e.what() << endl;
    }

    
}

int main() {
    Synchronous_comm_client();

   

    getchar();
    return 0;
}
