#define HEAD_ID_LEN 2
#define HEAD_DATA_LEN 2
#define HEAD_TOTAL_LEN 4
#define MAX_LENGTH  1024 * 2

#include <iostream>
#include <string>
#include <thread>
#include <mutex>
#include <vector>
#include <atomic>
#include "public.h"
#include "json.hpp"
#include <boost/asio.hpp>

using namespace boost::asio;
using namespace boost;
using namespace std;
using namespace boost::asio::ip;
using json = nlohmann::json;

std::atomic<int> studentID{ 1 };

void send_and_receive(tcp::socket& sock) {
    
    try {
        for (int i = 0; i < 1000; i++) {
            this_thread::sleep_for(std::chrono::milliseconds(10));

            // 准备发送的数据
            int currentStudentID = studentID++;
            std::string data = "xy " + to_string(currentStudentID);
            json js;
            js["name"] = data;
            js["studentID"] = currentStudentID;
            std::string request = js.dump();

            // 消息头部构建
            int msgID = MSG_HELLO_WORD;
            int request_length = request.length();
            int data_length_len = htons(request_length);
            int msgId_len = htons(msgID);

            char send_data[MAX_LENGTH] = { 0 };
            memcpy(send_data, &msgId_len, HEAD_ID_LEN);
            memcpy(send_data + HEAD_ID_LEN, &data_length_len, HEAD_DATA_LEN);
            memcpy(send_data + HEAD_TOTAL_LEN, request.c_str(), request_length);

            // 发送数据
            boost::asio::write(sock, boost::asio::buffer(send_data, request_length + HEAD_TOTAL_LEN));

            // 接收服务器返回的数据
            char header[HEAD_TOTAL_LEN] = { 0 }; // 头部缓冲区
            boost::asio::read(sock, boost::asio::buffer(header, HEAD_TOTAL_LEN));

            // 解析头部
            int recv_msgID;
            int recv_data_length;
            memcpy(&recv_msgID, header, HEAD_ID_LEN);
            memcpy(&recv_data_length, header + HEAD_ID_LEN, HEAD_DATA_LEN);

            recv_msgID = ntohs(recv_msgID);  // 转换为主机字节序
            recv_data_length = ntohs(recv_data_length);  // 转换为主机字节序

            if (recv_data_length > 0 && recv_data_length <= MAX_LENGTH - HEAD_TOTAL_LEN) {
                std::vector<char> body(recv_data_length);
                boost::asio::read(sock, boost::asio::buffer(body.data(), recv_data_length));

                // 转换消息体为字符串
                std::string body_str(body.begin(), body.end());

                // 打印接收到的数据
                // std::cout << "Received Data: " << body_str << std::endl;
            }
        }
        //std::cout << "Data: 10000" << std::endl;
    }
    catch (const boost::system::system_error& e) {
        std::cerr << "send_and_receive error: " << e.what() << std::endl;
    }
}

void Synchronous_comm_client() {
    string ip = "127.0.0.1";
    int port = 8080;
    tcp::endpoint ep(address::from_string(ip), port);

    io_context ic;
    auto start = std::chrono::high_resolution_clock::now();
    try {
        std::vector<std::thread> vec_threads;
        for (int i = 0; i < 100; ++i) {
            vec_threads.emplace_back([&]() {
                tcp::socket sock(ic);
                sock.connect(ep);
                send_and_receive(sock);
                });
        }

        for (auto& t : vec_threads) {
            t.join();
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto result = std::chrono::duration_cast<std::chrono::seconds>(end - start);
    std::cout << "total time = " << result.count() << std::endl;

    getchar();
}

int main() {
    Synchronous_comm_client();
    return 0;
}
