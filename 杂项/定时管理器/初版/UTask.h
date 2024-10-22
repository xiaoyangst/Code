/**
  ******************************************************************************
  * @file           : UTask.h
  * @author         : xy
  * @brief          : 定时任务
  * @attention      : None
  * @date           : 2024/10/16
  ******************************************************************************
  */

#ifndef SHEEPCHATSERVER_UTILS_UTASK_H_
#define SHEEPCHATSERVER_UTILS_UTASK_H_

#include "header.h"

namespace Utils {
class UTask {

 public:
  using TaskCallback = std::function<void(void)>;

  UTask(TaskCallback &cb, int64_t interval);
  UTask(TaskCallback &&cb, int64_t interval);
  void Start();
  void Stop();
  void ReStart();
 private:
  TaskCallback cb_;
  std::thread WorkThread_;
  std::atomic<bool> is_run;
  std::atomic<int64_t> interval_; // 时间间隔
};
}
#endif //SHEEPCHATSERVER_UTILS_UTASK_H_


/*
这个类的功能大致如下：
  1-接受一个函数任务并指定一个时间间隔，定时执行该任务。
  2-提供启动和停止定时任务的方法。
  3-可以安全地停止任务。
 */