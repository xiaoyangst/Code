#include "HttpConnection.h"

HttpConnection::HttpConnection(tcp::socket& socket)
	:http_socket_(std::move(socket))
{
}

namespace my_program_state
{
	std::size_t
		request_count()
	{
		static std::size_t count = 0;
		return ++count;
	}

	std::time_t
		now()
	{
		return std::time(0);
	}
}

void HttpConnection::start()
{
	read_request();
	check_deadline();
}


void HttpConnection::read_request()
{
	auto self = shared_from_this();
	http::async_read(http_socket_, buffer_, request_, [self](auto err,auto len) {
		boost::ignore_unused(len);	// 用来在代码中忽略未使用的变量或参数，避免编译器发出 "unused variable" 或 "unused parameter" 的警告
		if (!err) {
			self->process_request();
		}
		else {
			std::cout << err << std::endl;
		}
		});
}

void HttpConnection::write_response()
{
	auto self = shared_from_this();
	//response_.content_length(response_.body().size());		// 手动设置了响应的内容长度
	response_.prepare_payload();	// 该方法可以自动设置响应的内容长度
	http::async_write(http_socket_, response_, [self](auto err,auto len) {
		self->http_socket_.shutdown(tcp::socket::shutdown_send, err);
		self->deadline_.cancel();
		});
}

void HttpConnection::check_deadline()
{
	auto self = shared_from_this();
	deadline_.async_wait([self](auto err) {
		if (!err) {
			self->http_socket_.close();
		}
		else {
			std::cout << err << std::endl;
		}
		});
}

void HttpConnection::process_request()
{
	response_.keep_alive(false);
	//response_.result(request_.version);
	switch (request_.method())
	{
	case http::verb::get:
		response_.result(http::status::ok);
		response_.set(http::field::server, "Beast");
		create_get_response();
		break;
	case http::verb::post:
		response_.result(http::status::ok);
		response_.set(http::field::server, "Beast");
		create_post_response();
		break;
	default:
		response_.result(http::status::bad_request);
		response_.set(http::field::content_type, "text/plain");
		beast::ostream(response_.body())
			<< "Invalid request-method '"
			<< std::string(request_.method_string())
			<< "'";
		break;
	}

	write_response();
}

void HttpConnection::create_get_response()
{
	std::cout << "get request" << std::endl;
	if (request_.target() == "/count")
	{
		response_.set(http::field::content_type, "text/html");
		beast::ostream(response_.body())
			<< "<html>\n"
			<< "<head><title>Request count</title></head>\n"
			<< "<body>\n"
			<< "<h1>Request count</h1>\n"
			<< "<p>There have been "
			<< my_program_state::request_count()
			<< " requests so far.</p>\n"
			<< "</body>\n"
			<< "</html>\n";
	}
	else if (request_.target() == "/time")
	{
		response_.set(http::field::content_type, "text/html");
		beast::ostream(response_.body())
			<< "<html>\n"
			<< "<head><title>Current time</title></head>\n"
			<< "<body>\n"
			<< "<h1>Current time</h1>\n"
			<< "<p>The current time is "
			<< my_program_state::now()
			<< " seconds since the epoch.</p>\n"
			<< "</body>\n"
			<< "</html>\n";
	}
	else
	{
		response_.result(http::status::not_found);
		response_.set(http::field::content_type, "text/plain");
		beast::ostream(response_.body()) << "File not found\r\n";
	}
}

void HttpConnection::create_post_response()
{
	std::cout << "post request" << std::endl;
	if (request_.target() == "/email")
	{
		auto& body = this->request_.body();
		auto body_str = boost::beast::buffers_to_string(body.data());
		std::cout << "receive body is " << body_str << std::endl;
		this->response_.set(http::field::content_type, "text/json");

		try {
			json post_message = json::parse(body_str);	// string --> json 数据

			auto email = post_message.at("email");
			std::cout << "email is " << email << std::endl;

			json reponse_message;
			reponse_message["error"] = 0;
			reponse_message["email"] = email;
			reponse_message["msg"] = "recevie email post success";
			std::string jsonstr = reponse_message.dump();	// json --> string 数据
			beast::ostream(this->response_.body()) << jsonstr;
		}
		catch (const json::parse_error& e) {
			std::cerr << "JSON解析失败: " << e.what() << std::endl;
			response_.result(http::status::bad_request);
			beast::ostream(response_.body()) << "无效的JSON格式";
			return;
		}
	}
	else
	{
		response_.result(http::status::not_found);
		response_.set(http::field::content_type, "text/plain");
		beast::ostream(response_.body()) << "File not found\r\n";
	}
}
