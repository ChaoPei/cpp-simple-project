/**
 * 求数组中最小（大）的前K个数
 */

#include <iostream>
#include <random>
#include <vector>
#include <iterator>
#include <set>

using namespace std;


int partition(vector<int> &nums, int start, int end) {
    if(nums.empty() || start < 0 || end >= nums.size() || start > end) {
        return -1;
    }

    random_device rd;
    int idx = rd() % (end - start + 1) + start;
    swap(nums[idx], nums[end]);
    int small = start - 1;
    for(int i = start; i < end; ++i) {
        if(nums[i] < nums[end]) {
            small++;
            if(small < i) {
                swap(nums[small], nums[i]);
            }
        }
    }
    small++;
    swap(nums[small], nums[end]);
    return small;
}

// 利用partition分区实现寻找最小的k个数（会改变原数组）
vector<int> kth_least_number(vector<int> &nums, int k) {
    if(nums.size() <= k) {
        return nums;
    }

    int idx = partition(nums, 0, nums.size() - 1);
    while(idx != k) {
        if(idx < k) {
            idx = partition(nums, idx + 1, nums.size() - 1);
        } else {
            idx = partition(nums, 0, idx - 1);
        }
    }

    return vector<int>(nums.begin(), nums.begin() + k);
}

// 利用最小堆实现寻找最大的k个数（不改变原数组）
vector<int> kth_biggest_number(const vector<int> &nums, int k) {
    if(nums.size() <= k) {
        return nums;
    }

    using Iter = multiset<int>::iterator;
    multiset<int> result;
    result.insert(nums.begin(), nums.begin() + k);
    for(auto it = nums.begin() + k; it != nums.end(); ++it) {
        
        // set默认从大到小排序，获取最小值
        auto biggestIter = result.begin();
        // 判断更新堆
        if(*biggestIter < *it) {
            result.erase(biggestIter);
            result.insert(*it);
        }       
    }

    return vector<int>(result.begin(), result.end());
}

int main() {
    vector<int> nums = {4, 5, 1, 6, 2, 7, 3, 8};
    int k = 4;
    vector<int> result = kth_least_number(nums, k);
    copy(result.begin(), result.end(), ostream_iterator<int>(cout, " "));
    cout << endl;

    result = kth_biggest_number(nums, k);
    copy(result.begin(), result.end(), ostream_iterator<int>(cout, " "));
    cout << endl;

    return 0;
}