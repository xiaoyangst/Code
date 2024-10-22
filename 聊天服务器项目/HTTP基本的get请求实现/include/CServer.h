/**
  ******************************************************************************
  * @file           : CServer.h
  * @author         : xy
  * @brief          : CServer类构造函数接受一个端口号，创建acceptor接受新到来的连接
  * @attention      : None
  * @date           : 2024/10/15
  ******************************************************************************
  */

#ifndef SHEEPCHATSERVER__CSERVER_H_
#define SHEEPCHATSERVER__CSERVER_H_

#include <unordered_map>
#include "header.h"
#include "HttpConnection.h"

class CServer : public std::enable_shared_from_this<CServer> {
 public:
  CServer(net::io_context &io_context, unsigned short &port);
  void Start();
 private:
  tcp::acceptor acceptor_;
  net::io_context &io_context_;
  boost::asio::ip::tcp::socket socket_;

};

#endif //SHEEPCHATSERVER__CSERVER_H_

/*
  * 为什么 io_context_ 是引用类型 ？
  * 因为 io_context_ 不存在拷贝构造和拷贝赋值，也就只能有一个实例
  * 这个参数需要外面传进来，保证它的唯一性
*/

/*
 * 为什么 CServer 构造函数 中 acceptor_ 成员初始化成功之后 就已经开始监听 ？
 * 请看 tcp::acceptor 对应构造函数的源码
 * 自动 就 完成 open set_option bind listen 操作，还剩下 accept 操作我们自行调用 来监听客户端连接
 * 即
basic_socket_acceptor(const executor_type& ex,
      const endpoint_type& endpoint, bool reuse_addr = true)
    : impl_(ex)
  {
    boost::system::error_code ec;
    const protocol_type protocol = endpoint.protocol();
    impl_.get_service().open(impl_.get_implementation(), protocol, ec);
    boost::asio::detail::throw_error(ec, "open");
    if (reuse_addr)
    {
      impl_.get_service().set_option(impl_.get_implementation(),
          socket_base::reuse_address(true), ec);
      boost::asio::detail::throw_error(ec, "set_option");
    }
    impl_.get_service().bind(impl_.get_implementation(), endpoint, ec);
    boost::asio::detail::throw_error(ec, "bind");
    impl_.get_service().listen(impl_.get_implementation(),
        socket_base::max_listen_connections, ec);
    boost::asio::detail::throw_error(ec, "listen");
}
*/

/*
 * 本次 async_accept 用到的构造函数
 * 构造函数有很多，常用的是下面这个
 * 第一个参数是 socket，第二个参数是 回调函数
  template <typename Protocol1, typename Executor1, typename AcceptHandler>
  BOOST_ASIO_INITFN_RESULT_TYPE(AcceptHandler,
      void (boost::system::error_code))
  async_accept(basic_socket<Protocol1, Executor1>& peer,
      BOOST_ASIO_MOVE_ARG(AcceptHandler) handler,
      typename enable_if<
        is_convertible<Protocol, Protocol1>::value
      >::type* = 0)
  {
    return async_initiate<AcceptHandler, void (boost::system::error_code)>(
        initiate_async_accept(), handler, this,
        &peer, static_cast<endpoint_type*>(0));
  }
*/

/*
 * 为什么 boost::asio::ip::tcp::socket 对象 需要移动 ？
 * 因为 它 没有拷贝构造和拷贝赋值，只能通过 std::move 来完成转移
 */

/*
 * 为什么 要用 unordered_map 保存 HttpConnection 对象 ？
 * CServer 始终手握 HttpConnection 对象的最后一次计数
 */