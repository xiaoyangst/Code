#include <list>
#include <unordered_map>
#include <optional>
#include <string>
#include <stdexcept>

using namespace std;

class LRUCache {
public:
    explicit LRUCache(int capacity) : capacity_(capacity) {
        if (capacity_ <= 0) {
            throw invalid_argument("Capacity must be greater than 0");
        }
    }

    optional<string> get(const string& key) {
        auto it = findUMap_.find(key);
        if (it != findUMap_.end()) {
            // �������ʹ��λ��
            dataList_.splice(dataList_.begin(), dataList_, it->second);
            return it->second->second;
        }
        return nullopt;
    }

    void put(const string& key, const string& val) {
        auto it = findUMap_.find(key);
        if (it != findUMap_.end()) {
            // �������ݼ�λ��
            it->second->second = val;
            dataList_.splice(dataList_.begin(), dataList_, it->second);
            return;
        }

        // ��̭�߼�
        if (dataList_.size() >= capacity_) {
            auto& last = dataList_.back();
            findUMap_.erase(last.first);
            dataList_.pop_back();
        }

        // ����������
        dataList_.emplace_front(key, val);
        findUMap_[key] = dataList_.begin();
    }

private:
    int capacity_;
    list<pair<string, string>> dataList_;
    unordered_map<string, list<pair<string, string>>::iterator> findUMap_;
};
