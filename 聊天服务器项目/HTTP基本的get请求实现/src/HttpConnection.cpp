#include "HttpConnection.h"
HttpConnection::HttpConnection(boost::asio::ip::tcp::socket socket)
  : socket_(std::move(socket))
{

}
void HttpConnection::Start() {
  auto self = shared_from_this();
  http::async_read(socket_,buffer,request_,[self](auto err,auto len){ // The len parameter indicates the length of the data received
    try {
      if (err){
        std::cout<<"HttpConnection::Start --> "<<err<<std::endl;
        return ;
      }

      boost::ignore_unused(len);  // 用不到的参数，可以选择忽略；因为 http 不需要处理粘包问题，忽略
      self->HandleReq();          // 处理请求
      self->CheckDeadline();      // 等接受连接并处理回复之后，超时检测，60s 客户端没有回应就关闭 套接字了

    } catch (std::exception& ex) {
      std::cout<<"HttpConnection::Start --> "<<ex.what()<<std::endl;
    }
  });
}
void HttpConnection::HandleReq() {
  response_.version(request_.version());
  response_.keep_alive(false);  // 设置为短连接

  // 判断客户端的请求类型，交给 LogicSystem 处理
  if (request_.method() == http::verb::get){
    std::string url = request_.target().to_string();
    // 判断是 url 是否有参数，如果有参数，需要进行解析
    bool success = Singleton<LogicSystem>::getInstance()->getHandler(url, shared_from_this());
    if (!success){
      response_.result(http::status::not_found);
      response_.set(http::field::content_type, "text/plain");
      beast::ostream(response_.body()) << "Url Not Found\r\n";
    }else{
      response_.result(http::status::ok);
      response_.set(http::field::server,"SheepChatServer");
    }
    WriteResponse();
    return;
  }
}
void HttpConnection::CheckDeadline() {
  auto self = shared_from_this();

  deadline_.async_wait([self](auto err){  // 60s 之后 ，没有出错就关闭 连接
    if (!err){
      self->socket_.close();
    }
  });
}
void HttpConnection::WriteResponse() {
  auto self = shared_from_this();
  response_.content_length(response_.body().size());
  http::async_write(
      socket_,
      response_,
      [self](beast::error_code ec, std::size_t)
      {
        self->socket_.shutdown(tcp::socket::shutdown_send, ec); // 服务器这边关闭写端，代表不会再给客户端发送任何消息
        self->deadline_.cancel();
      });
}

