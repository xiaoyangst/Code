#include <iostream>
#include <mutex>
#include <atomic>

struct ControlBlock
{
	std::atomic<int> refCount_;	// ���ü���
	ControlBlock() : refCount_(1){}
};

template <typename T>
class SharedPtr
{
private:
	void release() {	// �ͷŵ�ǰ��Դ
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
		if (ptr) {	// ��Чָ����б�Ҫ�������ƿ�
			controlBlock_ = new ControlBlock();
		}
		else {
			controlBlock_ = nullptr;
		}
	}
	~SharedPtr() {
		release();
	}
	// �������캯��
	SharedPtr(const SharedPtr& other){
		std::lock_guard<std::mutex> lg_mtx(mtx_);
		ptr_ = other.ptr_;
		controlBlock_ = other.controlBlock_;
		if (controlBlock_) {
			++controlBlock_->refCount_;
		}
		std::cout << "�������캯��" << std::endl;
	}
	// ������ֵ���������
	SharedPtr& operator=(const SharedPtr& other) {
		if (&other != this) {	// ȷ����������ֵ����
			std::scoped_lock lock(mtx_, other.mtx_);
			release();	// �ͷ�һ�µ�ǰ ����ָ�����������Դ
			ptr_ = other.ptr_;
			controlBlock_ = other.controlBlock_;
			if (controlBlock_) {
				++controlBlock_->refCount_;	// �ǵ��������ü���
			}
			std::cout << "������ֵ���������" << std::endl;
		}
		return *this;
	}
	// �ƶ����캯��
	SharedPtr(SharedPtr&& other) noexcept{
		std::lock_guard<std::mutex> lg_mtx(mtx_);
		ptr_ = other.ptr_;
		controlBlock_ = other.controlBlock_;
		other.controlBlock_ = nullptr;
		other.ptr_ = nullptr;
		std::cout << "�ƶ����캯��" << std::endl;
	}
	// �ƶ���ֵ���������
	SharedPtr& operator=(SharedPtr&& other) noexcept {
		if (&other != this) {
			std::scoped_lock lock(mtx_, other.mtx_);
			release();
			ptr_ = other.ptr_;
			controlBlock_ = other.controlBlock_;
			++controlBlock_->refCount_;	// �ǵ��������ü���
			other.ptr_ = nullptr;
			other.controlBlock_ = nullptr;
			std::cout << "�ƶ���ֵ���������" << std::endl;
		}
		return *this;
	}
	// ��ȡ��ָ��
	T* get() {
		std::lock_guard<std::mutex> lg_mtx(mtx_);
		return ptr_;
	}
	// ��ȡ���ü���
	int use_count() {
		std::lock_guard<std::mutex> lg_mtx(mtx_);
		return controlBlock_ ? controlBlock_->refCount_.load() : 0 ;
	}
	// ��ͷ������
	T* operator->() {
		std::lock_guard<std::mutex> lg_mtx(mtx_);
		return ptr_;
	}
	// �����ò�����
	T& operator*() {
		std::lock_guard<std::mutex> lg_mtx(mtx_);
		return *ptr_;
	}
	// ����
	void reset(T* ptr = nullptr) {
		std::lock_guard<std::mutex> lg_mtx(mtx_);
		release();
		// ����������ڵ��ù��캯��
		ptr_ = ptr;
		if (ptr) {	
			controlBlock_ = new ControlBlock();

		}
		else {
			controlBlock_ = nullptr;
		}
	}
private:
	T* ptr_;					// ָ��ʵ�ʶ����ָ��
	ControlBlock* controlBlock_;
	mutable std::mutex mtx_;
};


void testSharedPtr() {
	// ����һ��SharedPtr���󣬹���һ��int���͵Ķ���
	SharedPtr<int> sp1(new int(10));
	std::cout << "sp1 use_count: " << sp1.use_count() << std::endl; // ������ü���

	// ��������SharedPtr����
	SharedPtr<int> sp2(sp1);
	std::cout << "sp1 use_count: " << sp1.use_count() << std::endl; // ������ü���
	std::cout << "sp2 use_count: " << sp2.use_count() << std::endl; // ������ü���

	// �ƶ�����SharedPtr����
	SharedPtr<int> sp3(std::move(sp1));
	std::cout << "sp1 use_count: " << sp1.use_count() << std::endl; // ������ü���
	std::cout << "sp3 use_count: " << sp3.use_count() << std::endl; // ������ü���

	// ������ֵ�����
	SharedPtr<int> sp4;
	sp4 = sp2;
	std::cout << "sp2 use_count: " << sp2.use_count() << std::endl; // ������ü���
	std::cout << "sp4 use_count: " << sp4.use_count() << std::endl; // ������ü���

	// �ƶ���ֵ�����
	SharedPtr<int> sp5;
	sp5 = std::move(sp2);
	std::cout << "sp2 use_count: " << sp2.use_count() << std::endl; // ������ü���
	std::cout << "sp5 use_count: " << sp5.use_count() << std::endl; // ������ü���

	// ����SharedPtr����
	sp5.reset(new int(20));
	std::cout << "sp5 use_count: " << sp5.use_count() << std::endl; // ������ü���
	std::cout << "*sp5: " << *sp5 << std::endl; // �������ֵ
}

int main() {
	testSharedPtr();
	return 0;
}


/*
delete nullptr ��û��ʲô���⣿

�ƶ�����Դ�Ƕ�����ڲ���Դ���Ƕ��������δ��ã�
*/