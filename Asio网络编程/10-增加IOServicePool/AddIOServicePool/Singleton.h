#pragma once
#include <memory>
#include <mutex>

template <typename T>
class Singleton {
protected:
	Singleton() = default;
	~Singleton() = default;

	Singleton(const Singleton<T>&) = delete;
	Singleton& operator=(const Singleton<T>&) = delete;

	static std::shared_ptr<T> instance_;
	static std::once_flag once_;
public:
	static std::shared_ptr<T> getInstance() {
		std::call_once(once_, []() {
			instance_ = std::make_shared<T>();
			});
		return instance_;
	}
};

template <typename T>
std::shared_ptr<T> Singleton<T>::instance_ = nullptr;

template <typename T>
std::once_flag Singleton<T>::once_;