/**
 * 求数的组合：所有可能组合，不考虑数的个数和顺序
 * 
 * 位图算法：
 * 假设原有元素 n 个，则最终组合结果是 2n−1 个。我们可以用位操作方法：
 * 假设元素原本有：a,b,c 三个，则 1 表示取该元素，0 表示不取。故取a则是001，
 * 取ab则是011。所以一共三位，每个位上有两个选择 0 和 1。而000没有意义，所以是2n−1个结果。
 */

#include <vector>
#include <string>
#include <iostream>

using namespace std;

void combination(vector<string> &strs) {        // 要求len < 31，否则int的bit不够用
    if(strs.empty()) {
        return;
    }

    int len = strs.size();
    int n = 1 << len;       // n = 2 ^ len
    for(int i = 1; i < n; ++i) {
        for(int j = 0; j < len; ++j) {      // 打印出bit为1的元素
            if(i & (1 << j)) {
                cout << strs.at(j);
            }
        }
        cout << endl;
    }
}

int main() {
    vector<string> strs{"a", "b", "c", "d"};
    combination(strs);

    getchar();
    return 0;
}