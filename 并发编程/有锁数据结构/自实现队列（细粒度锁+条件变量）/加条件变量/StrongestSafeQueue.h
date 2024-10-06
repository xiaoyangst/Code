#ifndef CONCURRENCESAFE_CONCURRENCE_STRONGESTSAFEQUEUE_H_
#define CONCURRENCESAFE_CONCURRENCE_STRONGESTSAFEQUEUE_H_

#include <memory>
#include <mutex>
#include <condition_variable>

template<typename T>
class StrongestSafeQueue{

  struct Node{
    std::shared_ptr<T> data;
    std::unique_ptr<struct Node> next;
  };
  //using Node = struct Node;

 public:

  StrongestSafeQueue():head(new Node),tail(head.get()){};   // 构造时，就创建虚拟节点
  ~StrongestSafeQueue() = default;

  StrongestSafeQueue(const StrongestSafeQueue&) =delete;
  StrongestSafeQueue& operator=(const StrongestSafeQueue&) = delete;

  std::shared_ptr<T> try_pop();
  bool try_pop(T& data);

  void wait_pop(T& data);
  std::shared_ptr<T> wait_pop();

  void push(T data);
  bool empty();

 private:
  Node* get_tail(){ // 线程安全
    std::lock_guard<std::mutex> lg_tail(tail_mutex);
    return tail;
  }

  std::unique_ptr<Node> pop_head(){ // 非线程安全，必须在加锁前提下调用，即获得 head_mutex 锁
    std::unique_ptr<Node> old_head = std::move(head);
    head = std::move(old_head->next);
    return old_head;
  }

  std::unique_lock<std::mutex> wait_for_data(){ // 返回互斥锁，并且保证至少有一个数据，否则阻塞
    std::unique_lock<std::mutex> head_lock(head_mutex);
    cond_have.wait(head_lock,[&](){
      return head.get() != get_tail();  // false阻塞，代表队列为空，阻塞在此处等待有数据才接触阻塞，形象名为 wait_for_data
    }); // 队列不为空
    return std::move(head_lock);
  }

  std::unique_ptr<Node> wait_pop_head(){  // 线程安全且必然会得到队头节点，否则阻塞
    std::unique_lock<std::mutex> head_lock(wait_for_data());
    return pop_head();
  }

  std::unique_ptr<Node> wait_pop_head(T& data){ // 同上
    std::unique_lock<std::mutex> head_lock(wait_for_data());
    data = std::move(*head->data);
    return pop_head();
  }

  std::unique_ptr<Node> try_pop_head(){
    std::lock_guard<std::mutex> lg_head(head_mutex);
    if (head.get() == get_tail()){
      return std::unique_ptr<Node>();
    }
    return pop_head();
  }

  std::unique_ptr<Node> try_pop_head(T& data){
    std::lock_guard<std::mutex> lg_head(head_mutex);
    if (head.get() == get_tail()){
      return std::unique_ptr<Node>();
    }
    data = std::move(*head->data);
    return pop_head();
  }

 private:
  std::unique_ptr<Node> head; // 头指针，智能指针
  Node* tail; // 尾指针，裸指针

  std::mutex head_mutex;
  std::mutex tail_mutex;

  std::condition_variable cond_have;
};
template<typename T>
bool StrongestSafeQueue<T>::empty() {
  std::lock_guard<std::mutex> lg_head(head_mutex);
  return head.get() == get_tail();
}
template<typename T>
bool StrongestSafeQueue<T>::try_pop(T &data) {
  std::unique_ptr<Node> const old_head = try_pop_head(data);
  return old_head;
}

template<typename T>
std::shared_ptr<T> StrongestSafeQueue<T>::try_pop() {
  std::unique_ptr<Node> old_head = try_pop_head();
  return old_head ? old_head->data : std::shared_ptr<T>();
}

template<typename T>
void StrongestSafeQueue<T>::wait_pop(T &data) {
  std::unique_ptr<Node> const old_head = wait_pop_head(data);
}
template<typename T>
std::shared_ptr<T> StrongestSafeQueue<T>::wait_pop() {
  std::unique_ptr<Node> const old_head = wait_pop_head();
  return old_head->data;
}

template<typename T>
void StrongestSafeQueue<T>::push(T data) {
  std::shared_ptr<T> new_data = std::make_shared<T>(std::move(data)); // 构造 data
  std::unique_ptr<Node> new_node = std::make_unique<Node>(); // 构造 Node
  Node* new_tail = new_node.get();
  {
    std::lock_guard<std::mutex> lg_tail(tail_mutex);
    tail->data = new_data;
    tail->next = std::move(new_node);
    tail = new_tail;
  }
  cond_have.notify_one();
}



#endif //CONCURRENCESAFE_CONCURRENCE_STRONGESTSAFEQUEUE_H_
