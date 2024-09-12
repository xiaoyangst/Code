#define MAX_LENGTH 1024 * 2
#define HEAD_LENGTH 2
#include <iostream>
#include <string>
#include <thread>
#include <mutex>
#include <boost/asio.hpp>
using namespace boost::asio;
using namespace boost;
using namespace std;
using namespace boost::asio::ip;

std::mutex mtx;

void send_thread(tcp::socket& sock) {
    for (;;) {
        this_thread::sleep_for(std::chrono::milliseconds(2));
        const char* request = "hello world!";
        short request_length = strlen(request);
        char send_data[MAX_LENGTH] = { 0 };
        memcpy(send_data, &request_length, HEAD_LENGTH);
        memcpy(send_data + HEAD_LENGTH, request, request_length);

        std::lock_guard<std::mutex> lock(mtx);
        boost::asio::write(sock, boost::asio::buffer(send_data, request_length + HEAD_LENGTH));
    }
}

/*
void recv_thread(tcp::socket& sock) {
    for (;;) {
        this_thread::sleep_for(std::chrono::milliseconds(10));
        char reply_head[HEAD_LENGTH];
        size_t reply_length = boost::asio::read(sock, boost::asio::buffer(reply_head, HEAD_LENGTH));

        short msglen = 0;
        memcpy(&msglen, reply_head, HEAD_LENGTH);
        std::cout << "len = " << msglen << std::endl;
        if (msglen > MAX_LENGTH) {
            std::cerr << "Error: Message length exceeds buffer size.\n";
            continue;
        }

        char msg[MAX_LENGTH] = { 0 };
        size_t  msg_length = boost::asio::read(sock, boost::asio::buffer(msg, msglen));

        std::cout << "Reply is: ";
        std::cout.write(msg, msglen) << endl;
        std::cout << "Reply len is " << msglen;
        std::cout << "\n";
    }
}
*/

void Synchronous_comm_client() {
    string ip = "127.0.0.1";
    int port = 8080;
    tcp::endpoint ep(address::from_string(ip), port);

    io_context ic;
    tcp::socket sock(ic, ep.protocol());

    try {
        sock.connect(ep);
        cout << "连接成功...\n";

        thread t1(&send_thread, ref(sock));

        t1.join();
    }
    catch (const std::exception& e) {
        cout << "Exception: " << e.what() << endl;
    }
}

int main() {
    Synchronous_comm_client();
    return 0;
}
