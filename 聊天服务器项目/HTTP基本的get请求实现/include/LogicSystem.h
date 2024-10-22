/**
  ******************************************************************************
  * @file           : LogicSystem.h
  * @author         : xy
  * @brief          : 处理 HTTP 请求
  * @attention      : None
  * @date           : 2024/10/15
  ******************************************************************************
  */

#ifndef SHEEPCHATSERVER__LOGICSYSTEM_H_
#define SHEEPCHATSERVER__LOGICSYSTEM_H_

#include "header.h"
#include "HttpConnection.h"
#include "Singleton.h"

class HttpConnection;
using HttpHandler = std::function<void(std::shared_ptr<HttpConnection>)>;

class LogicSystem : public Singleton<LogicSystem>{
  friend class Singleton<LogicSystem>;

 public:
  ~LogicSystem() = default;
  bool getHandler(std::string url,std::shared_ptr<HttpConnection> conn);  // 执行 get 回调
  void getRegister(const std::string& url,const HttpHandler& cb); // 注册 get 回调
 private:
  LogicSystem();
  std::unordered_map<std::string,HttpHandler> getHandlers_;
  std::unordered_map<std::string,HttpHandler> postHandlers_;
};

#endif //SHEEPCHATSERVER__LOGICSYSTEM_H_

/*
 * 为什么 LogicSystem 构造函数是私有，别人如何获得对象呢？
 *  LogicSystem 构造函数 可以 由 Singleton 类调用，不提供给外界，外界只需要 通过 Singleton 的 getInstance 获取
 */