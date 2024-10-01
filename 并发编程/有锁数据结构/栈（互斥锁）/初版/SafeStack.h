#ifndef CONCURRENCESAFE_CONCURRENCE_SAFESTACK_H_
#define CONCURRENCESAFE_CONCURRENCE_SAFESTACK_H_

#include <stack>
#include <mutex>
template<typename T>
class SafeStack
{
 public:
  SafeStack() = default;
  ~SafeStack() = default;

  bool empty();
  int	size();
  T top();
  void push(T& data);
  T pop();

  SafeStack(const SafeStack& other);		// 支持拷贝构造函数
  SafeStack& operator=(const SafeStack&) = delete;	// 不支持赋值构造函数
 private:
  std::mutex m_mtx;
  std::stack<T> m_stack;
  int m_size = { 0 };
};

template<typename T>
inline bool SafeStack<T>::empty()
{
  std::lock_guard<std::mutex> lg(m_mtx);
  return m_size == 0;
}

template<typename T>
inline int SafeStack<T>::size()
{
  std::lock_guard<std::mutex> lg(m_mtx);
  return m_size;
}

template<typename T>
inline T SafeStack<T>::top()
{
  std::lock_guard<std::mutex> lg(m_mtx);
  return m_stack.top();
}

template<typename T>
inline void SafeStack<T>::push(T& data)
{
  std::lock_guard<std::mutex> lg(m_mtx);
  m_stack.push(data);
  m_size++;
}

template<typename T>
inline T SafeStack<T>::pop()
{
  std::lock_guard<std::mutex> lg(m_mtx);
  auto result = m_stack.top();
  m_stack.pop();
  m_size--;
  return result;
}

// 如何理解这一点
template<typename T>
inline SafeStack<T>::SafeStack(const SafeStack& other)
{
  std::lock_guard<std::mutex> lg(other.m_mtx);
  m_stack = other.m_stack;
}

#endif //CONCURRENCESAFE_CONCURRENCE_SAFESTACK_H_
