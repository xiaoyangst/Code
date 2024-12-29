#include <iostream>
#include <mutex>
#include <atomic>

struct ControlBlock
{
	std::atomic<int> refCount_;	// 引用计数
	ControlBlock() : refCount_(1){}
};

template <typename T>
class SharedPtr
{
private:
	void release() {	// 释放当前资源
		if (controlBlock_) {
			if (controlBlock_->refCount_.fetch_sub(1) == 1) {
				delete ptr_;
				delete controlBlock_;
			}
		}
		ptr_ = nullptr;
		controlBlock_ = nullptr;
	}
public:
	SharedPtr() : ptr_(nullptr),controlBlock_(nullptr) {

	}
	explicit SharedPtr(T* ptr):ptr_(ptr){
		if (ptr) {	// 有效指针才有必要创建控制块
			controlBlock_ = new ControlBlock();
		}
		else {
			controlBlock_ = nullptr;
		}
	}
	~SharedPtr() {
		release();
	}
	// 拷贝构造函数
	SharedPtr(const SharedPtr& other){
		std::lock_guard<std::mutex> lg_mtx(mtx_);
		ptr_ = other.ptr_;
		controlBlock_ = other.controlBlock_;
		if (controlBlock_) {
			++controlBlock_->refCount_;
		}
		std::cout << "拷贝构造函数" << std::endl;
	}
	// 拷贝赋值运算符函数
	SharedPtr& operator=(const SharedPtr& other) {
		if (&other != this) {	// 确保不是自身赋值自身
			std::scoped_lock lock(mtx_, other.mtx_);
			release();	// 释放一下当前 智能指针对象管理的资源
			ptr_ = other.ptr_;
			controlBlock_ = other.controlBlock_;
			if (controlBlock_) {
				++controlBlock_->refCount_;	// 记得增加引用计数
			}
			std::cout << "拷贝赋值运算符函数" << std::endl;
		}
		return *this;
	}
	// 移动构造函数
	SharedPtr(SharedPtr&& other) noexcept{
		std::lock_guard<std::mutex> lg_mtx(mtx_);
		ptr_ = other.ptr_;
		controlBlock_ = other.controlBlock_;
		other.controlBlock_ = nullptr;
		other.ptr_ = nullptr;
		std::cout << "移动构造函数" << std::endl;
	}
	// 移动赋值运算符函数
	SharedPtr& operator=(SharedPtr&& other) noexcept {
		if (&other != this) {
			std::scoped_lock lock(mtx_, other.mtx_);
			release();
			ptr_ = other.ptr_;
			controlBlock_ = other.controlBlock_;
			++controlBlock_->refCount_;	// 记得增加引用计数
			other.ptr_ = nullptr;
			other.controlBlock_ = nullptr;
			std::cout << "移动赋值运算符函数" << std::endl;
		}
		return *this;
	}
	// 获取裸指针
	T* get() {
		std::lock_guard<std::mutex> lg_mtx(mtx_);
		return ptr_;
	}
	// 获取引用计数
	int use_count() {
		std::lock_guard<std::mutex> lg_mtx(mtx_);
		return controlBlock_ ? controlBlock_->refCount_.load() : 0 ;
	}
	// 箭头操作符
	T* operator->() {
		std::lock_guard<std::mutex> lg_mtx(mtx_);
		return ptr_;
	}
	// 解引用操作符
	T& operator*() {
		std::lock_guard<std::mutex> lg_mtx(mtx_);
		return *ptr_;
	}
	// 重置
	void reset(T* ptr = nullptr) {
		std::lock_guard<std::mutex> lg_mtx(mtx_);
		release();
		// 下面就类似于调用构造函数
		ptr_ = ptr;
		if (ptr) {	
			controlBlock_ = new ControlBlock();

		}
		else {
			controlBlock_ = nullptr;
		}
	}
private:
	T* ptr_;					// 指向实际对象的指针
	ControlBlock* controlBlock_;
	mutable std::mutex mtx_;
};


void testSharedPtr() {
	// 创建一个SharedPtr对象，管理一个int类型的对象
	SharedPtr<int> sp1(new int(10));
	std::cout << "sp1 use_count: " << sp1.use_count() << std::endl; // 输出引用计数

	// 拷贝构造SharedPtr对象
	SharedPtr<int> sp2(sp1);
	std::cout << "sp1 use_count: " << sp1.use_count() << std::endl; // 输出引用计数
	std::cout << "sp2 use_count: " << sp2.use_count() << std::endl; // 输出引用计数

	// 移动构造SharedPtr对象
	SharedPtr<int> sp3(std::move(sp1));
	std::cout << "sp1 use_count: " << sp1.use_count() << std::endl; // 输出引用计数
	std::cout << "sp3 use_count: " << sp3.use_count() << std::endl; // 输出引用计数

	// 拷贝赋值运算符
	SharedPtr<int> sp4;
	sp4 = sp2;
	std::cout << "sp2 use_count: " << sp2.use_count() << std::endl; // 输出引用计数
	std::cout << "sp4 use_count: " << sp4.use_count() << std::endl; // 输出引用计数

	// 移动赋值运算符
	SharedPtr<int> sp5;
	sp5 = std::move(sp2);
	std::cout << "sp2 use_count: " << sp2.use_count() << std::endl; // 输出引用计数
	std::cout << "sp5 use_count: " << sp5.use_count() << std::endl; // 输出引用计数

	// 重置SharedPtr对象
	sp5.reset(new int(20));
	std::cout << "sp5 use_count: " << sp5.use_count() << std::endl; // 输出引用计数
	std::cout << "*sp5: " << *sp5 << std::endl; // 输出对象值
}

int main() {
	testSharedPtr();
	return 0;
}


/*
delete nullptr 有没有什么问题？

移动的资源是对象的内部资源，那对象本身该如何处置？
*/