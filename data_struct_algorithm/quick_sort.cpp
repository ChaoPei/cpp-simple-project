/**
 * 快速排序
 */

#include <random>
#include <iostream>
#include <exception>
#include <vector>
#include <iterator>

using namespace std;

// #define _DEBUG

/**
 * 返回一个[min, max]之间的随机正整数值
 */
int range_random_int(int min, int max) {
    random_device rd;
    int random_int = rd();
    return abs(random_int) % (max - min + 1) + min;
}

int partition(vector<int> &nums, int start, int end) {
    if(nums.empty() || start < 0 || end >= nums.size() || start > end) {
        throw new runtime_error("invalid input !\n");
    }

    // 选择列兵并将其置于末尾
    int idx = range_random_int(start, end); 

#ifdef _DEBUG
    cout << "start: " << start << " end: " << end << endl;
    cout << "choose idx: " << idx << " num: " << nums.at(idx) << endl;
#endif
    
    swap(nums[idx], nums[end]);

    int small = start - 1;
    for(int i = start; i < end; ++i) {
        if(nums[i] < nums[end]) {
            small++;
            if(small != i) {
                // 说明此时small所指的位置，元素的值大于列兵的值
                swap(nums[small], nums[i]);
            }
        }
    }
    small++;
    swap(nums[small], nums[end]);
    
#ifdef _DEBUG
    copy(nums.begin(), nums.end(), ostream_iterator<int>(cout, " "));
    cout << endl;
#endif

    return small;
}

void quick_sort(vector<int> &nums, int start, int end) {
    
    if(nums.empty() || start < 0 || end >= nums.size() || start > end) {
        throw new runtime_error("invalid input !\n");
    } 
    if(start == end) {
        return;
    }
    

    int mid = partition(nums, start, end);
    if(mid > start) {
        quick_sort(nums, start, mid - 1);
    }

    if(mid < end) {
        quick_sort(nums, mid + 1, end);
    }
}

void quick_sort(vector<int> &nums) {
    if(nums.size() <= 1) {
        return;
    }

    quick_sort(nums, 0, nums.size() - 1);
}


int main() {
    vector<int> nums{2,6,1,3,-2,67,12,43,98,21,2,-1,-10};
    
    cout << "origin nums: \n";
    copy(nums.begin(), nums.end(), ostream_iterator<int>(cout, " "));
    cout << endl;
    
    quick_sort(nums);
    
    cout << "sorted nums: \n";
    copy(nums.begin(), nums.end(), ostream_iterator<int>(cout, " "));
    cout << endl;
    

    return 0;   
}
