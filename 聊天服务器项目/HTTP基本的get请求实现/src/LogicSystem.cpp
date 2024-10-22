#include "LogicSystem.h"

LogicSystem::LogicSystem() {
  // 注册 url 对应的回调
  getRegister("/xy_test",[](const std::shared_ptr<HttpConnection>& conn){
    beast::ostream(conn->response_.body()) << "interview xy_test";
  });
}

void LogicSystem::getRegister(const std::string& url, const HttpHandler& cb) {
  auto it = getHandlers_.find(url);
  if (it == getHandlers_.end()){
    getHandlers_.insert(std::make_pair(url,cb));
  }
}
bool LogicSystem::getHandler(std::string url, std::shared_ptr<HttpConnection> conn) {
  auto it = getHandlers_.find(url);
  if (it == getHandlers_.end()){
    return false;
  }

  getHandlers_[url](conn);
  return true;
}



