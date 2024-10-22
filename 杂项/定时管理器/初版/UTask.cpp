#include "UTask.h"
namespace Utils {

UTask::UTask(UTask::TaskCallback &cb, int64_t interval)
    : cb_(cb)
    , is_run(false)
    , interval_(interval){

}
UTask::UTask(UTask::TaskCallback &&cb, int64_t interval)
    : cb_(std::move(cb))
    , is_run(false)
    , interval_(interval){

}
void UTask::Start() {
  if (is_run.load()) return;
  is_run.store(true);

  WorkThread_ = std::thread([&](){
    while (is_run.load()){
      std::this_thread::sleep_for(std::chrono::seconds(interval_));
      if (is_run.load() && cb_) cb_();
    }
  });
}
void UTask::ReStart() {
  Start();
}
void UTask::Stop() {
  is_run.store(false);
  if (WorkThread_.joinable()){
    WorkThread_.join();
  }
}

}