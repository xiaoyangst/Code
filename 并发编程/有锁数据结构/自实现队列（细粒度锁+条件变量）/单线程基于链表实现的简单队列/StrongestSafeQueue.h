//
// Created by xy on 2024-10-01.
//

#ifndef CONCURRENCESAFE_CONCURRENCE_STRONGESTSAFEQUEUE_H_
#define CONCURRENCESAFE_CONCURRENCE_STRONGESTSAFEQUEUE_H_

#include <memory>

template<typename T>
class StrongestSafeQueue{

  struct Node{  // 定义节点
    T data;
    std::unique_ptr<struct Node> next;
    explicit Node(T data_):data(std::move(data_)){}
  };
  using Node = struct Node;

 public:
  StrongestSafeQueue():head(nullptr),tail(nullptr){};	// 务必初始化
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
  std::unique_ptr<Node> new_data(std::make_unique<Node>(data));
  Node* const new_tail = new_data.get(); // 获取裸指针
  if (tail){  // 队列中至少一个元素
    tail->next = std::move(new_data);
  }else{  // 队列中没有元素
    head = std::move(new_data);
  }
  tail = new_tail;
}
template<typename T>
std::shared_ptr<T> StrongestSafeQueue<T>::try_pop() {
  if (!head){
    return std::shared_ptr<T>();
  }
  std::shared_ptr<T> const result(
      std::make_shared<T>(
          std::move(head->data)
          )
      );
  std::unique_ptr<Node> const old_head = std::move(head);
  head = std::move(old_head->next); // 更新头指针
  return result;
}

#endif //CONCURRENCESAFE_CONCURRENCE_STRONGESTSAFEQUEUE_H_
