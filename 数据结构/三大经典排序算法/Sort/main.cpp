#include <iostream>
#include <string>
#include <algorithm>
#include <vector>


// 堆排序
// 从下标 0 开始，尽管网上有的下标从 1 开始（公式好推导）

int getLeft(int i) {
    return 2 * i + 1;
}

int getRight(int i) {
    return 2 * i + 2;
}

// 下沉
void siftDown(std::vector<int> &data, int len, int cur) {
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

void heapSort(std::vector<int> &data) {
    // 先把数组堆化（大顶堆）,得到一个 当前节点 大于等于 左右节点
    // 排除叶子节点，从后往上取出节点，每个节点进行一次 siftDown
    for (int i = (data.size() / 2) - 1; i >= 0; i--) {
        siftDown(data, data.size(), i);
    }
    // 再进行堆排序
    // 依次 把堆顶元素和堆底元素进行交换
    // 这时候的堆底元素往后将不参与排序，堆顶元素已经破坏堆，需要将堆顶元素进行 siftDown
    int len = data.size() - 1;
    for (int i = len; i > 0; --i) {
        std::swap(data[0], data[i]);
        siftDown(data, i, 0);
    }
}


// 归并排序

void merge(std::vector<int> &data, int left, int mid, int right) {
    std::vector<int> tmp(right - left + 1);

    // 将无序的两个数组 从小到大依次加入到 tmp 中
    // 你可能疑惑哪里来的两个无序数组？[left,mid] [mid+1,right]

    // 先把左数组的起始下标，右数组的起始下标 记录下来
    int left_index = left, right_index = mid + 1, index = 0;

    // 开始比较，谁小就依次保存它
    while (left_index <= mid && right_index <= right) {
        data[left_index] <= data[right_index] ? tmp[index++] = data[left_index++] : tmp[index++] = data[right_index++];
    }
    // 可能某一个数组还有元素没有被处理完毕
    while (left_index <= mid) {
        tmp[index++] = data[left_index++];
    }
    while (right_index <= right) {
        tmp[index++] = data[right_index++];
    }
    // 把排序后的结果保存到 data 数组中去
    int k = 0;
    for (int i = left; i <= right;) {
        data[i++] = tmp[k++];
    }
}

void mergeSort(std::vector<int> &data, int left, int right) {
    if (left >= right) return;  // 只有一个元素，就停止继续递归
    int mid = left - ((left - right) / 2);
    mergeSort(data, left, mid);
    mergeSort(data, mid + 1, right);
    merge(data, left, mid, right);
}

void mergeSort(std::vector<int> &data) {
    if (data.empty()) return;
    mergeSort(data, 0, data.size() - 1);
}


// 快速排序

int medianThree(std::vector<int> &data, int left, int mid, int right) {
    int left_val = data[left], mid_val = data[mid], right_val = data[right];
    if ((left_val <= mid_val && mid_val <= right_val)
        || (right_val <= mid_val && mid_val <= left_val)) {
        return mid;
    }
    if ((mid_val <= left_val && left_val <= right_val)
        || (right_val <= left_val && left_val <= mid_val)) {
        return left;
    }
    return right;
}

int partition(std::vector<int> &data, int left, int right) {
    int mid = left - ((left - right) / 2);
    int median = medianThree(data, left, mid, right);  // 得到一个合理的 base
    std::swap(data[median], data[left]);
    int base = data[left];
    int src_left = left, src_right = right;

    // 先右后左
    while (left < right) {
        while (left < right && data[right] >= base) {    // 找到一个右边可用于交换的元素
            right--;
        }
        while (left < right && data[left] <= base) {     // 找到一个左边可用于交换的元素
            left++;
        }
        std::swap(data[left], data[right]);        // 既然已经找到符合交换的两个元素，那就交换吧，来继续满足 左小右大 原则
    }
    std::swap(data[left], data[src_left]);
    return left;
}

/*
int partition(std::vector<int> &data, int left, int right) {
    int base = data[left];
    int src_left = left, src_right = right;

    // 先右后左
    while (left < right) {
        while (left < right && data[right] >= base) {    // 找到一个右边可用于交换的元素
            right--;
        }
        while (left < right && data[left] <= base) {     // 找到一个左边可用于交换的元素
            left++;
        }
        std::swap(data[left], data[right]);        // 既然已经找到符合交换的两个元素，那就交换吧，来继续满足 左小右大 原则
    }
    std::swap(data[left], data[src_left]);
    return left;
}
*/

void QuickSort(std::vector<int> &data, int left, int right) {
    if (left >= right) return;
    int mid = partition(data, left, right);
    QuickSort(data, left, mid - 1);
    QuickSort(data, mid + 1, right);
}

void QuickSort(std::vector<int> &data) {
    if (data.empty()) return;
    QuickSort(data, 0, data.size() - 1);
}

template<typename T>
void Print(const T &data) {
    for (auto num: data) {
        std::cout << num << " ";
    }
    std::cout << std::endl;
}

int main() {

    std::vector<int> data = {4, 3, 7, 1, 2, 10, 5, 9, 6, 8};
    heapSort(data);

    Print(data);

    return 0;
}
