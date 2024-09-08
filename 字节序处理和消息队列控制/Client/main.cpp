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
    try {
        for (;;) {
            this_thread::sleep_for(std::chrono::milliseconds(100)); // Adjust sleep time as needed
            const char* request = "hello world!";
            short request_length = strlen(request);
            short request_length_len = htons(request_length);
            char send_data[MAX_LENGTH] = { 0 };
            memcpy(send_data, &request_length, HEAD_LENGTH);
            memcpy(send_data + HEAD_LENGTH, request, request_length);

            std::lock_guard<std::mutex> lock(mtx);
            boost::asio::write(sock, boost::asio::buffer(send_data, request_length + HEAD_LENGTH));
        }
    }
    catch (const boost::system::system_error& e) {
        std::cerr << "send_thread error: " << e.what() << std::endl;
    }
}

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

        // Wait for the thread to finish or be interrupted
        t1.join();
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
