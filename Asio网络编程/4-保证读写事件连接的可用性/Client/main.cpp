#define BUFFSIZE 100

#include <iostream>
#include <string>
#include <boost/asio.hpp>
using namespace boost::asio;
using namespace boost;
using namespace std;
using namespace boost::asio::ip;

// ͬ��ͨ�ſͻ���

void Synchronous_comm_client() {
    //  �����˵㣬��¼����������Ϣ   
    string ip = "127.0.0.1";
    int port = 8080;
    tcp::endpoint ep(address::from_string(ip), port);

    //  ���ӵ�������
    io_context ic;
    tcp::socket sock(ic, ep.protocol());

    try {
        sock.connect(ep);
        printf("���ӳɹ�...\n");

        // ������Ϣ��������
        std::string buf = "My name is ClientA";
        int send_length = sock.send(buffer(buf.c_str(), buf.length()));
        if (send_length <= 0) {
            cout << "send failed!!!" << endl;
        }

        // ���շ���������Ϣ
        char recv_buf[BUFFSIZE];
        memset(recv_buf, 0, BUFFSIZE);
        int recv_length = sock.receive(asio::buffer(recv_buf, BUFFSIZE));
        if (recv_length <= 0) {
            cout << "recv failed!!!" << endl;
        }
        else {
            recv_buf[recv_length] = '\0'; // ȷ���ַ�����ֹ
            cout << "recv_buf = " << recv_buf << endl;
        }

        // �ٴη�����Ϣ
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
