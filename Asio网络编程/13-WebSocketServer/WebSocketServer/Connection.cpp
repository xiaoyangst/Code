#include "Connection.h"
#include "ConnectionMgr.h"
Connection::Connection(net::io_context& ioc)
	:ioc_(ioc)
	,ws_ptr_(std::make_unique<stream<tcp_stream>>(make_strand(ioc)))
{
	// 生成随机的 UUID
	boost::uuids::random_generator generator;
	boost::uuids::uuid uuid = generator();

	// 将 UUID 转换为字符串
	uuid_ = boost::uuids::to_string(uuid);
}

void Connection::AsyncAccept()
{
	auto self = shared_from_this();
	ws_ptr_->async_accept([self](boost::system::error_code err) {
		try {
			if (!err) {
				ConnectionMgr::GetInstance().AddConnection(self);
				self->Start();
			}
			else {
				std::cout << "websocket accept failed, err is " << err.what() << std::endl;
			}
		}
		catch (std::exception& exp) {
			std::cout << "websocket async accept exception is " << exp.what();
		}
		});
}

std::string Connection::GetUid()
{
	return uuid_;
}

void Connection::Start()
{
	auto self = shared_from_this();
	ws_ptr_->async_read(recv_buffer_, [self](error_code  err, std::size_t  buffer_bytes) {
		try {
			if (err) {
				std::cout << "websocket async read error is " << err.what() << std::endl;
				ConnectionMgr::GetInstance().RmvConnection(self->GetUid());
				return;
			}
			self->ws_ptr_->text(self->ws_ptr_->got_text());
			std::string recv_data = boost::beast::buffers_to_string(self->recv_buffer_.data());
			self->recv_buffer_.consume(self->recv_buffer_.size());
			std::cout << "websocket receive msg is " << recv_data << std::endl;
			self->AsyncSend(std::move(recv_data));
			self->Start();
		}
		catch (std::exception& exp) {
			std::cout << "exception is " << exp.what() << std::endl;
			ConnectionMgr::GetInstance().RmvConnection(self->GetUid());
		}
		});
}

void Connection::AsyncSend(std::string msg)
{
	{
		std::lock_guard<std::mutex> lck_gurad(send_mtx_);
		int que_len = send_que_.size();
		send_que_.push(msg);
		if (que_len > 0) {
			return;
		}
	}

	SendCallBack(std::move(msg));
}

void Connection::SendCallBack(std::string msg)
{
	auto self = shared_from_this();
	ws_ptr_->async_write(boost::asio::buffer(msg.c_str(), msg.length()),
		[self](error_code  err, std::size_t  nsize) {
			try {
				if (err) {
					std::cout << "async send err is " << err.what() << std::endl;
					ConnectionMgr::GetInstance().RmvConnection(self->uuid_);
					return;
				}

				std::string send_msg;
				{
					std::lock_guard<std::mutex> lck_gurad(self->send_mtx_);
					self->send_que_.pop();
					if (self->send_que_.empty()) {
						return;
					}

					send_msg = self->send_que_.front();
				}

				self->SendCallBack(std::move(send_msg));
			}
			catch (std::exception& exp) {
				std::cout << "async send exception is " << exp.what() << std::endl;
				ConnectionMgr::GetInstance().RmvConnection(self->uuid_);
			}
		});
}

boost::asio::ip::tcp::socket& Connection::GetSocket()
{
	auto& socket = boost::beast::get_lowest_layer(*ws_ptr_).socket();	// 如何理解 ？
	return socket;
}
