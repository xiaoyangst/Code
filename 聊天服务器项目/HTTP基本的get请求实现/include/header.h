//
// Created by xy on 2024/10/15.
//

#ifndef SHEEPCHATSERVER_INCLUDE_HEADER_H_
#define SHEEPCHATSERVER_INCLUDE_HEADER_H_

#include <memory>
#include <sys/time.h>
#include <vector>
#include <mutex>
#include <chrono>
#include <functional>
#include <iostream>
#include <boost/beast/http.hpp>
#include <boost/beast.hpp>
#include <boost/asio.hpp>

#include "utils.h"

namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace http = beast::http;           // from <boost/beast/http.hpp>
namespace net = boost::asio;            // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>



#endif //SHEEPCHATSERVER_INCLUDE_HEADER_H_
