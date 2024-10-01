#ifndef CONCURRENCESAFE_CONCURRENCE_SAFEQUEUE_H_
#define CONCURRENCESAFE_CONCURRENCE_SAFEQUEUE_H_

#include <memory>
#include <mutex>
#include <condition_variable>
#include <queue>

template<typename T>
class SafeQueue{
 public:
  SafeQueue() = default;
  ~SafeQueue() = default;

  bool empty();
  void push(T data);

  std::shared_ptr<T> try_pop();
  bool try_pop(T& data);

  // 没有元素可 pop 会阻塞
  std::shared_ptr<T> wait_pop();
  void wait_pop(T& data);

  SafeQueue(const SafeQueue<T> & other);
  SafeQueue& operator=(const SafeQueue<T>&) = delete;

 private:
  std::mutex m_mtx;
  std::condition_variable m_have;
  std::queue<std::shared_ptr<T>> m_queue;
};
template<typename T>
void SafeQueue<T>::wait_pop(T &data) {
  std::unique_lock<std::mutex> ul(m_mtx);
  m_have.wait(ul,[this](){
    return !m_queue.empty();
  }); // m_queue have data
  data = std::move(*m_queue.front());
  m_queue.pop();
}
template<typename T>
std::shared_ptr<T> SafeQueue<T>::wait_pop() {
  std::unique_lock<std::mutex> ul(m_mtx);
  m_have.wait(ul,[this](){
    return !m_queue.empty();
  }); // m_queue have data
  auto result = m_queue.front();
  m_queue.pop();
  return result;
}
template<typename T>
bool SafeQueue<T>::try_pop(T &data) {
  std::lock_guard<std::mutex> lg(m_mtx);
  if (m_queue.empty()){
    return false;
  }
  data = std::move(*m_queue.front());
  m_queue.pop();
  return true;
}
template<typename T>
std::shared_ptr<T> SafeQueue<T>::try_pop() {
  std::lock_guard<std::mutex> lg(m_mtx);
  if (m_queue.empty()){
    return std::shared_ptr<T>();
  }
  auto result = m_queue.front();
  m_queue.pop();
  return result;
}
template<typename T>
SafeQueue<T>::SafeQueue(const SafeQueue<T> &other) {
  std::lock_guard<std::mutex> lg(other.m_mtx);
  m_queue = other.m_queue;
}

template<typename T>
void SafeQueue<T>::push(T data) {
  std::shared_ptr<T> new_data(std::make_shared<T>(std::move(data)));
  std::lock_guard<std::mutex> lg(m_mtx);
  m_queue.push(std::move(new_data));
  m_have.notify_one();
}

template <typename T>
bool SafeQueue<T>::empty() {
  std::lock_guard<std::mutex> lg(m_mtx);
  return m_queue.empty();
}

#endif //CONCURRENCESAFE_CONCURRENCE_SAFEQUEUE_H_
