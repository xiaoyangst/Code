#ifndef CONCURRENCESAFE_CONCURRENCE_STRONGESTSAFEQUEUE_H_
#define CONCURRENCESAFE_CONCURRENCE_STRONGESTSAFEQUEUE_H_

#include <memory>
#include <mutex>

template<typename T>
class StrongestSafeQueue{

  struct Node{  // 定义节点
    std::shared_ptr<T> data;
    std::unique_ptr<struct Node> next;
  };
  using Node = struct Node;

 public:
  StrongestSafeQueue():head(new Node),tail(head.get()){};   // 构造时，就创建虚拟节点
  ~StrongestSafeQueue() = default;

  StrongestSafeQueue(const StrongestSafeQueue&) =delete;
  StrongestSafeQueue& operator=(const StrongestSafeQueue&) = delete;

  std::shared_ptr<T> try_pop();
  void push(T data);

 private:
  Node* get_tail(){
    std::lock_guard<std::mutex> lg_tail(tail_mutex);
    return tail;
  }

  std::unique_ptr<Node> pop_head(){
    std::lock_guard<std::mutex> lg_head(head_mutex);
    if (head.get() == get_tail()){  // 只有虚拟节点，代表队列为空
      return nullptr;
    }
    std::unique_ptr<Node> old_head = std::move(head);
    head = std::move(old_head->next);
    return old_head;
  }

 private:
  std::unique_ptr<Node> head; // 头指针，智能指针
  Node* tail; // 尾指针，裸指针

  std::mutex head_mutex;
  std::mutex tail_mutex;
};


template<typename T>
void StrongestSafeQueue<T>::push(T data) {
  std::shared_ptr<T> new_data = std::make_shared<T>(std::move(data)); // 构造 data
  std::unique_ptr<Node> new_node = std::make_unique<Node>(); // 构造 Node
  Node* new_tail = new_node.get();
  std::lock_guard<std::mutex> lg_tail(tail_mutex);
  tail->data = new_data;
  tail->next = std::move(new_node);
  tail = new_tail;
}

template<typename T>
std::shared_ptr<T> StrongestSafeQueue<T>::try_pop() {
  std::unique_ptr<Node> old_head = pop_head();
  return old_head ? old_head->data : std::shared_ptr<T>();
}

#endif //CONCURRENCESAFE_CONCURRENCE_STRONGESTSAFEQUEUE_H_
