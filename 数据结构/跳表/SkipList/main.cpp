#include "SkipList.h"
#include <cassert>

void test_skiplist() {
	SkipList skiplist;

	// 测试插入和查找
	skiplist.insert(1, 10);
	skiplist.insert(2, 20);
	skiplist.insert(3, 30);
	assert(skiplist.search(1) != nullptr && skiplist.search(1)->value_ == 10);
	assert(skiplist.search(2) != nullptr && skiplist.search(2)->value_ == 20);
	assert(skiplist.search(3) != nullptr && skiplist.search(3)->value_ == 30);

	// 测试查找不存在的元素
	assert(skiplist.search(4) == nullptr);

	// 测试更新已有元素
	skiplist.insert(1, 100);
	assert(skiplist.search(1) != nullptr && skiplist.search(1)->value_ == 100);

	// 测试删除元素
	skiplist.remove(2);
	assert(skiplist.search(2) == nullptr);

	// 删除后仍然能找到其他元素
	assert(skiplist.search(1) != nullptr);
	assert(skiplist.search(3) != nullptr);

	// 测试删除所有元素
	skiplist.remove(1);
	skiplist.remove(3);
	assert(skiplist.search(1) == nullptr);
	assert(skiplist.search(3) == nullptr);

	// 测试删除不存在的元素不会崩溃
	skiplist.remove(100);

	std::cout << "All tests passed!" << std::endl;
}


int main() {

	test_skiplist();

	return 0;
}