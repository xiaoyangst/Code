#ifndef INTERVIEW_PRIORITYQUEUE_H
#define INTERVIEW_PRIORITYQUEUE_H

#include <vector>
#include <stdexcept>


template<typename T>
class PriorityQueue {
public:
    PriorityQueue() = default;

    void push(const T &element) {
        data_.push_back(element);
        siftUp(data_, data_.size(), data_.size() - 1);
    }

    void push(const T &&element) {
        data_.push_back(element);
        siftUp(data_, data_.size(), data_.size() - 1);
    }

    void pop() {
        if (data_.empty()) {
            throw std::runtime_error("top from empty queue");
        }
        // 把堆顶和堆底元素交换，再把堆底元素 pop 出去
        // 然后对 堆顶元素 siftDown，保证堆合理性
        std::swap(data_[0], data_[data_.size() - 1]);
        data_.pop_back();
        siftDown(data_, data_.size(), 0);
    }

    const T &top() {
        if (data_.empty()) {
            throw std::runtime_error("top from empty queue");
        }
        return data_[0];
    }

    bool empty() {
        return data_.empty();
    }

private:
    void siftDown(std::vector<T> &data, int len, int cur) {    // 刪除元素，需要下沉
        while (true) {
            // 判断当前节点 当前节点左节点 当前节点右节点 谁最大
            int max_index = cur;
            int left_index = getLeft(cur);
            int right_index = getRight(cur);
            if (left_index < len && data[max_index] < data[left_index]) {
                max_index = left_index;
            }
            if (right_index < len && data[max_index] < data[right_index]) {
                max_index = right_index;
            }
            // 此刻 max_index 指向 三个节点中的最大节点

            if (max_index != cur) { // 只要 max_index != cur，就表明 max_index 已经被更改，那就意味着 还有堆化的必要
                std::swap(data[max_index], data[cur]);
                cur = max_index;
            } else {
                break;
            }
        }
    }

    void siftUp(std::vector<T> &data, int len, int cur) {     //插入元素，需要上浮
        while (cur > 0) {
            int parent = getParent(cur);
            if (data[cur] <= data[parent]) break;
            std::swap(data[cur],data[parent]);
            cur = parent;
        }
    }

    int getLeft(int index) {
        return 2 * index + 1;
    }

    int getRight(int index) {
        return 2 * index + 2;
    }

    int getParent(int index) {
        return (index - 1) / 2;
    }

private:
    std::vector<T> data_;
};


/*
#include <vector>
#include <stdexcept>

template<typename T, typename Compare = std::less<T>>
class PriorityQueue {
public:
    PriorityQueue() = default;

    void push(const T &element) {
        data_.push_back(element);
        siftUp(data_, data_.size(), data_.size() - 1);
    }

    void push(T &&element) {
        data_.push_back(std::move(element));
        siftUp(data_, data_.size(), data_.size() - 1);
    }

    void pop() {
        if (data_.empty()) {
            throw std::runtime_error("pop from empty queue");
        }
        std::swap(data_[0], data_[data_.size() - 1]);
        data_.pop_back();
        if (!data_.empty()) {
            siftDown(data_, data_.size(), 0);
        }
    }

    const T &top() const {
        if (data_.empty()) {
            throw std::runtime_error("top from empty queue");
        }
        return data_[0];
    }

    [[nodiscard]] bool empty() const {
        return data_.empty();
    }

private:
    void siftDown(std::vector<T> &data, int len, int cur) {
        while (true) {
            int best_index = cur;
            int left_index = getLeft(cur);
            int right_index = getRight(cur);

            if (left_index < len && comp(data[best_index], data[left_index])) {
                best_index = left_index;
            }
            if (right_index < len && comp(data[best_index], data[right_index])) {
                best_index = right_index;
            }

            if (best_index != cur) {
                std::swap(data[best_index], data[cur]);
                cur = best_index;
            } else {
                break;
            }
        }
    }

    void siftUp(std::vector<T> &data, int len, int cur) {
        while (cur > 0) {
            int parent_index = getParent(cur);
            if (comp(data[parent_index], data[cur])) {
                std::swap(data[parent_index], data[cur]);
                cur = parent_index;
            } else {
                break;
            }
        }
    }

    [[nodiscard]] int getLeft(int index) const { return 2 * index + 1; }

    [[nodiscard]] int getRight(int index) const { return 2 * index + 2; }

    [[nodiscard]] int getParent(int index) const { return (index - 1) / 2; }

private:
    std::vector<T> data_;
    Compare comp;
};
*/

#endif //INTERVIEW_PRIORITYQUEUE_H
