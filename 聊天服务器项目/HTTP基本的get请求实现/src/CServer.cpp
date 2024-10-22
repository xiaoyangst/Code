#include <iostream>
#include "CServer.h"
CServer::CServer(net::io_context& io_context, unsigned short &port)
  : io_context_(io_context)
  , acceptor_(io_context,tcp::endpoint(tcp::v4(),port))
  , socket_(io_context_)
{

}

/*
 * 不管处于何种情况，都将再次调用 Start 方法，以实现反复 async_accept 新的客户端连接
 */
void CServer::Start() {
  auto self = shared_from_this();
  acceptor_.async_accept(socket_,[self, this](auto err){ // A client establishes a connection
    try {
      if (err){
        std::cout<<"CServer::Start --> "<<err<<std::endl;
        self->Start();
        return ;
      }

      auto http_conn = std::make_shared<HttpConnection>(std::move(socket_));
      http_conn->Start();

      self->Start();
    }catch (std::exception& ex){
      std::cout<<"CServer::Start --> "<<ex.what()<<std::endl;
      self->Start();
    }
  });
}
