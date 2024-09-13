#define HEAD_ID_LEN 2
#define HEAD_DATA_LEN 2
#define HEAD_TOTAL_LEN 4
#define MAX_LENGTH  1024 * 2
#include <iostream>
#include <string>
#include <thread>
#include <mutex>
#include "public.h"
#include "json.hpp"
#include <boost/asio.hpp>
using namespace boost::asio;
using namespace boost;
using namespace std;
using namespace boost::asio::ip;
using json = nlohmann::json;

std::mutex mtx;

void send_and_receive(tcp::socket& sock) {
    try {
        for (;;) {
            this_thread::sleep_for(std::chrono::milliseconds(100)); // ���� sleep ʱ��

            // ׼�����͵�����
            static int studentID = 1;
            std::string data = "xy " + to_string(studentID);
            json js;
            js["name"] = data;
            js["studentID"] = studentID++;
            std::string request = js.dump();

            // ��Ϣͷ������
            int msgID = MSG_HELLO_WORD;
            int request_length = request.length();
            int data_length_len = htons(request_length);
            int msgId_len = htons(msgID);

            char send_data[MAX_LENGTH] = { 0 };
            memcpy(send_data, &msgId_len, HEAD_ID_LEN);
            memcpy(send_data + HEAD_ID_LEN, &data_length_len, HEAD_DATA_LEN);
            memcpy(send_data + HEAD_TOTAL_LEN, request.c_str(), request_length);

            {
                std::lock_guard<std::mutex> lock(mtx);

                // ��ӡ���͵�����
                //std::cout << "Sending data: " << request << std::endl;

                // ��������
                boost::asio::write(sock, boost::asio::buffer(send_data, request_length + HEAD_TOTAL_LEN));
            }


            // ���շ��������ص�����
            char header[HEAD_TOTAL_LEN] = { 0 }; // ͷ��������
            boost::asio::read(sock, boost::asio::buffer(header, HEAD_TOTAL_LEN));

            // ����ͷ��
            int recv_msgID;
            int recv_data_length;
            memcpy(&recv_msgID, header, HEAD_ID_LEN);
            memcpy(&recv_data_length, header + HEAD_ID_LEN, HEAD_DATA_LEN);

            recv_msgID = ntohs(recv_msgID);  // ת��Ϊ�����ֽ���
            recv_data_length = ntohs(recv_data_length);  // ת��Ϊ�����ֽ���

            // ��ӡ�յ�����Ϣ ID �ͳ���
            std::cout << "Received MsgID: " << recv_msgID << ", Data length: " << recv_data_length << std::endl;

            if (recv_data_length > 0 && recv_data_length <= MAX_LENGTH - HEAD_TOTAL_LEN) {
                std::vector<char> body(recv_data_length);
                boost::asio::read(sock, boost::asio::buffer(body.data(), recv_data_length));

                // ת����Ϣ��Ϊ�ַ���
                std::string body_str(body.begin(), body.end());

                // ��ӡ���յ�������
                std::cout << "Received Data: " << body_str << std::endl;

                // ���� JSON
                //try {
                //    json js_recv = json::parse(body_str);
                //    std::cout << "Parsed JSON: " << js_recv.dump(4) << std::endl;
                //}
                //catch (json::parse_error& e) {
                //    std::cerr << "JSON Parse Error: " << e.what() << std::endl;
                //}
    
            }

        }
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
    tcp::socket sock(ic, ep.protocol());

    try {
        sock.connect(ep);
        cout << "���ӳɹ�...\n";

        thread t1(&send_and_receive, ref(sock));

        getchar();

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
