#include <cassert>
#include <algorithm> // 确保包含此头文件

#include "Trie.h"


void testTrie() {
	Trie trie;

	// 插入单词
	trie.insert("apple");
	trie.insert("app");
	trie.insert("application");
	trie.insert("bat");
	trie.insert("batman");
	trie.insert("batmobile");

	// 测试搜索功能
	assert(trie.search("apple") == 1);
	assert(trie.search("app") == 1);
	assert(trie.search("application") == 1);
	assert(trie.search("bat") == 1);
	assert(trie.search("batman") == 1);
	assert(trie.search("batmobile") == 1);
	assert(trie.search("banana") == 0); // 不存在

	// 测试前缀匹配
	auto prefixResults = trie.prefix("app");
	assert(prefixResults.size() == 3);
	assert(find(prefixResults.begin(), prefixResults.end(), "app") != prefixResults.end());
	assert(find(prefixResults.begin(), prefixResults.end(), "apple") != prefixResults.end());
	assert(find(prefixResults.begin(), prefixResults.end(), "application") != prefixResults.end());

	prefixResults = trie.prefix("bat");
	assert(prefixResults.size() == 3);
	assert(find(prefixResults.begin(), prefixResults.end(), "bat") != prefixResults.end());
	assert(find(prefixResults.begin(), prefixResults.end(), "batman") != prefixResults.end());
	assert(find(prefixResults.begin(), prefixResults.end(), "batmobile") != prefixResults.end());

	prefixResults = trie.prefix("cat"); // 不存在
	assert(prefixResults.empty());

	// 测试删除功能
	trie.remove("apple");
	assert(trie.search("apple") == 0);
	assert(trie.search("app") == 1); // "app" 仍然存在

	trie.remove("app");
	assert(trie.search("app") == 0);

	trie.remove("batman");
	assert(trie.search("batman") == 0);
	assert(trie.search("bat") == 1); // "bat" 仍然存在

	// 测试前序遍历（输出所有单词）
	std::cout << "Trie Contents after deletion:" << std::endl;

	trie.preOrder();

	std::cout << "All tests passed successfully!" << std::endl;
}

int main()
{
	testTrie();
	return 0;
}

