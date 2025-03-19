#include <gtest/gtest.h>
#include "utils/SafeQueue.h"  // 需要确保 SafeQueue 的头文件路径正确
#include <thread>
#include <vector>

TEST(SafeQueueTest, PushAndPopSingleThread) {
	SafeQueue<int> queue;
	int value;
	queue.push(42);
	ASSERT_TRUE(queue.pop(value));
	EXPECT_EQ(value, 42);
}

TEST(SafeQueueTest, PopBlocksUntilPush) {
	SafeQueue<int> queue;
	int value;
	std::thread producer([&]() {
	  std::this_thread::sleep_for(std::chrono::milliseconds(100));
	  queue.push(99);
	});

	ASSERT_TRUE(queue.pop(value));
	EXPECT_EQ(value, 99);
	producer.join();
}

TEST(SafeQueueTest, MultipleProducersAndConsumers) {
	SafeQueue<int> queue;
	const int num_threads = 5;
	std::vector<std::thread> producers, consumers;
	std::vector<int> results;
	std::mutex result_mutex;

	for (int i = 0; i < num_threads; ++i) {
		producers.emplace_back([&, i]() {
		  queue.push(i);
		});
	}

	for (int i = 0; i < num_threads; ++i) {
		consumers.emplace_back([&]() {
		  int value;
		  if (queue.pop(value)) {
			  std::lock_guard<std::mutex> lock(result_mutex);
			  results.push_back(value);
		  }
		});
	}

	for (auto &t : producers) t.join();
	for (auto &t : consumers) t.join();

	ASSERT_EQ(results.size(), num_threads);
	std::sort(results.begin(), results.end());
	for (int i = 0; i < num_threads; ++i) {
		EXPECT_EQ(results[i], i);
	}
}

TEST(SafeQueueTest, StopFunctionality) {
	SafeQueue<int> queue;
	int value;
	std::thread consumer([&]() {
	  EXPECT_FALSE(queue.pop(value));  // 应该返回 false
	});

	std::this_thread::sleep_for(std::chrono::milliseconds(100));
	queue.stop();
	consumer.join();
}
