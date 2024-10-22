/**
  ******************************************************************************
  * @file           : HttpConnection.h
  * @author         : xy
  * @brief          : 代表一个 Http 连接对象，但自身不处理请求，而是交给 LogicSystem 去处理
  * @attention      : None
  * @date           : 2024/10/15
  ******************************************************************************
  */

#ifndef SHEEPCHATSERVER__HTTPCONNECTION_H_
#define SHEEPCHATSERVER__HTTPCONNECTION_H_

#include "header.h"
#include "LogicSystem.h"
#define FLAT_BUFFER 8192

class HttpConnection : public std::enable_shared_from_this<HttpConnection>{
  friend class LogicSystem;
 public:
  explicit HttpConnection(boost::asio::ip::tcp::socket socket);
  void Start();
 private:
  void CheckDeadline();
  void WriteResponse();
  void HandleReq();



 private:
  boost::asio::ip::tcp::socket socket_;
  beast::flat_buffer buffer {FLAT_BUFFER};
  http::request<http::dynamic_body> request_;
  http::response<http::dynamic_body> response_;
  net::steady_timer deadline_{  // 定时器，60s 检查一次
    socket_.get_executor(),std::chrono::seconds(60)
  };
};

#endif //SHEEPCHATSERVER__HTTPCONNECTION_H_

/*
 * async_read 参数解读

async_read(
    AsyncReadStream& stream,
    DynamicBuffer& buffer,
    basic_parser<isRequest>& parser,
    ReadHandler&& handler)

第一个参数为异步可读的数据流，大家可以理解为socket.
第二个参数为一个buffer，用来存储接受的数据，因为http可接受文本，图像，音频等多种资源文件，所以是Dynamic动态类型的buffer。
第三个参数是请求参数，我们一般也要传递能接受多种资源类型的请求参数。
第四个参数为回调函数，接受成功或者失败，都会触发回调函数，我们用lambda表达式就可以了。
 */