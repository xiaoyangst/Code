#ifndef CONCURRENCESAFE_CONCURRENCE_STRONGESTSAFEQUEUE_H_
#define CONCURRENCESAFE_CONCURRENCE_STRONGESTSAFEQUEUE_H_

#include <memory>

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
  std::unique_ptr<Node> head; // 头指针，智能指针
  Node* tail; // 尾指针，裸指针
};

template<typename T>
void StrongestSafeQueue<T>::push(T data) {
  std::shared_ptr<T> new_data = std::make_shared<T>(std::move(data)); // 构造 data
  std::unique_ptr<Node> new_node(std::make_unique<Node>()); // 构造 Node
  tail->data = new_data;
  Node* const new_tail = new_node.get();
  tail->next = std::move(new_node);
  tail = new_tail;
}

template<typename T>
std::shared_ptr<T> StrongestSafeQueue<T>::try_pop() {
  if (head.get() == tail){  // 只有虚拟节点，代表队列为空
    return std::shared_ptr<T>();
  }
  std::shared_ptr<T> const result(head->data);  // data 成员已经是 shared_ptr 指针了
  std::unique_ptr<Node> const old_head = std::move(head);
  head = std::move(old_head->next); // 更新头指针
  return result;
}

#endif //CONCURRENCESAFE_CONCURRENCE_STRONGESTSAFEQUEUE_H_
