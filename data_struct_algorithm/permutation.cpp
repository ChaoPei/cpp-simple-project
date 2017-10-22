/**
 * 递归求数的全排列
 */

#include <vector>
#include <iostream>
#include <iterator>
#include <memory>

using namespace std;

class Solution {
    
public:
    vector<vector<int>> result;

    vector<vector<int>> permute(vector<int>& nums) {
        if(nums.size() <= 1)
        {
            result.push_back(nums);
            return result;
        }
        
        dfs(0, nums);
        return result;
    
    }
    
    void dfs(int index, vector<int>& nums)
    {
        if(index == nums.size())
        {
            result.push_back(vector<int>(nums.begin(), nums.end()));
            return;
        }
        
        // 将nums[index] 与后面的每个数交换，然后递归遍历交换之后的序列
        for(int j = index; j < nums.size(); ++j)
        {
            std::swap(nums[j], nums[index]);
            dfs(index+1, nums);  
            std::swap(nums[j], nums[index]);
        }
    }
};

int main(int args, char* argv[])
{

    int i, j;
    int numbers[] = {1,2,3,4};
	vector<int> nums(numbers, numbers + sizeof(numbers) / sizeof(numbers[0]));
    shared_ptr<Solution> sp = make_shared<Solution>();
    auto result = sp->permute(nums);

	for(auto r: result) {
        copy(r.begin(), r.end(), ostream_iterator<int>(cout, " "));
        cout << endl;
    }

	system("pause");
	return 0;
}