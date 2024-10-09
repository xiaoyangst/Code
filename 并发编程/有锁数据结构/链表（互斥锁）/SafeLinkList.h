//
// Created by xy on 2024-10-06.
//

#ifndef CONCURRENCESAFE_CONCURRENCE_SAFELINKLIST_H_
#define CONCURRENCESAFE_CONCURRENCE_SAFELINKLIST_H_

#include <memory>
#include <mutex>

template <typename T>
class SafeLinkList{

  struct Node{
    std::shared_ptr<T> data;
    std::unique_ptr<Node> next;
    std::mutex mtx; // 每个节点拥有一个 锁

    Node():next(nullptr){} // 用以构造虚拟头节点
    explicit Node(T data):data(std::make_shared<T>(std::move(data))){}
  };

 private:
  Node head;

 public:
  SafeLinkList() = default;
  ~SafeLinkList() {
    remove_if([](T const&) {
      return true;
    });
  }


  SafeLinkList(const SafeLinkList<T>&) = delete;
  SafeLinkList& operator=(const SafeLinkList<T>&) = delete;

  void push_front(T data){
    std::unique_ptr<Node> new_node = std::make_unique<Node>(data);
    std::lock_guard<std::mutex> lg(head.mtx);
    new_node->next = std::move(head.next);
    head.next = std::move(new_node);
  }


  template<typename Function>
  void for_each(Function F){
    Node* current = &head;
    std::unique_lock<std::mutex> ul(head.mtx);  // 方便随时解锁
    while (Node* const next = current->next.get()){
      // 要操作 current 下一个 节点，自然要 加其锁
      std::unique_lock<std::mutex> ul_next(next->mtx);
      ul.unlock();  // 已经可以安全访问到 current 下一个节点
      F(*next->data);
      current = next;
      ul = std::move(ul_next);  // 保证 current 的访问是线程安全，因为之前的 ul 已经被 unlock
    }
  }

  template<typename Predicate>
  std::shared_ptr<T> find_first_if(Predicate P){
    Node* current = &head;
    std::unique_lock<std::mutex> ul(head.mtx);
    while (Node* const next = current->next.get()){
      std::unique_lock<std::mutex> ul_next(next->mtx);
      ul.unlock();
      if(P(*next->data)){
        return next->data;
      }
      current = next;
      ul = std::move(ul_next);  // 保证 current 的访问是线程安全，因为之前的 ul 已经被 unlock
    }
    return std::shared_ptr<T>();
  }

  template<typename Predicate>
  void remove_if(Predicate P){
    Node* current = &head;
    std::unique_lock<std::mutex> ul(head.mtx);
    while (Node* const next = current->next.get()) {
      std::unique_lock<std::mutex> ul_next(next->mtx);
      if (P(*next->data)) {
        std::unique_ptr<Node> old_next = std::move(current->next);
        current->next = std::move(next->next);
        ul_next.unlock();
      } else {
        ul.unlock();
        current = next;
        ul = std::move(ul_next);  // 保证 current 的访问是线程安全，因为之前的 ul 已经被 unlock
      }
    }
  }

};

#endif //CONCURRENCESAFE_CONCURRENCE_SAFELINKLIST_H_

/*
 Node head 和 Node* head 声明有何区别？

 Node head 表示一个具体的 Node 类型对象，而不是指针。
 它表示的是一个实际存在的 Node 结构体或类的实例，保存在栈上。

 Node* head 表示的是一个指向 Node 类型的指针。
 它存储的是一个内存地址，指向某个 Node 对象，而不是直接存储 Node 对象本身。
 需要通过 malloc 或 new 来初始化，否则不可以使用
 */

/*
 大家要注意，插入和删除加锁的顺序要保持一致，都是从头到尾，这样能防止死锁，也能保持互斥
 */

/*
 为什么 for_each 在获取 head 时候不需要加锁 ？
 因为 head 作为虚拟头结点，顶多会改变 next 指向，但这是不用被关注的，本身就是在遍历
 */