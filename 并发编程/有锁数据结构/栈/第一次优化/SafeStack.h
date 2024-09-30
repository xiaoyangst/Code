#ifndef CONCURRENCESAFE_CONCURRENCE_SAFESTACK_H_
#define CONCURRENCESAFE_CONCURRENCE_SAFESTACK_H_

#include <execution>
#include <stack>
#include <memory>
#include <mutex>


struct empty_stack : std::exception{
  [[nodiscard]] const char* what() const noexcept override {
    return exception::what();
  }
};

template<typename T>
class SafeStack
{
 public:
  SafeStack() = default;
  ~SafeStack() = default;

  bool empty();
  T top();
  void push(T data);
  std::shared_ptr<T> pop();
  void pop(T& data);
  int size();
  SafeStack(const SafeStack& other);		// 支持拷贝构造函数
  SafeStack& operator=(const SafeStack&) = delete;	// 不支持赋值构造函数
 private:
  std::mutex m_mtx;
  std::stack<T> m_stack;
};


template<typename T>
inline bool SafeStack<T>::empty()
{
  std::lock_guard<std::mutex> lg(m_mtx);
  return m_stack.empty();
}

template<typename T>
inline int SafeStack<T>::size()
{
  std::lock_guard<std::mutex> lg(m_mtx);
  return m_stack.size();
}

template<typename T>
inline T SafeStack<T>::top()
{
  std::lock_guard<std::mutex> lg(m_mtx);
  return m_stack.top();
}

template<typename T>
inline void SafeStack<T>::push(T data)
{
  std::lock_guard<std::mutex> lg(m_mtx);
  m_stack.push(std::move(data));
}

template<typename T>
void SafeStack<T>::pop(T &data) {
  std::lock_guard<std::mutex> lg(m_mtx);
  if (m_stack.empty()) throw empty_stack();
  data = std::move(m_stack.top());
  m_stack.pop();
}

template<typename T>
inline std::shared_ptr<T> SafeStack<T>::pop()
{
  std::lock_guard<std::mutex> lg(m_mtx);
  if (m_stack.empty()) throw empty_stack();
  std::shared_ptr<T> result(std::make_shared<T>(std::move(m_stack.top())));	// 之前实现的拷贝构造函数在这里发挥作用了
  m_stack.pop();
  return result;
}

template<typename T>
inline SafeStack<T>::SafeStack(const SafeStack& other)
{
  std::lock_guard<std::mutex> lg(other.m_mtx);
  m_stack = other.m_stack;
}

#endif //CONCURRENCESAFE_CONCURRENCE_SAFESTACK_H_
