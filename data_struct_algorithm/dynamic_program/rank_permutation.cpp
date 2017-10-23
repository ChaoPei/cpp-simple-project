/**
 * n个人排名，允许并列名次，共有多少种排名结果？
 * 
 * 一般有两个维度的动态规划，肯定是两层循环，需要考虑两个因素
 * dp[i][j]表示有个j个人，i种名次的情形，那么显然有以下递推关系成立：
 *  - dp[i][i] = i!
 *  - dp[1][j] = 1
 *  - dp[i][j] = i * dp[i][j-1] + i * dp[i-1][j-1](j > i > 1)
 *      - 当j-1个人排完名次之后加入一个人，有两种情况：
 *      - 他和其中一个人或几个人并列，则可能是已经排序的i种名次的一种
 *      - 他自己一个名次，则前面是i-1中名次，而他这个可能i种的一种
 */

#include <vector>
#include <iostream>
#include <iterator>

using namespace std;


vector<int> calc_factorial(int n) {
    vector<int> factorial(n + 1);
    for(int i = 1; i <= n; ++i) {
        factorial[i] = i == 1 ? 1 : factorial[i-1] * i;
    }

    return factorial;
}

void dp_solution(int n, const vector<int> &factorial) {
    vector<int> ans(n + 1, 0);
    vector<vector<int>> dp(n + 1, vector<int>(n + 1, 0));
    for(int i = 1; i <= n; ++i) {       // i种名次
        for(int j = i; j < n; ++j) {    // 人次
            if(i == 1) {
                dp[i][j] = 1;
            } else if (j == i) {
                dp[i][j] = factorial[i];
            } else {
                dp[i][j] = i * dp[i][j-1] + i * dp[i-1][j-1];
            }
        }
    }

    for(int i = 1; i <= n; ++i) {
        for(int j = 1; j <= n; ++j) {
            ans[i] += dp[j][i];
        }
    }
    copy(ans.begin() + 1, ans.end(), ostream_iterator<int>(cout, "\n"));
    cout << endl;
}

int main() {
    int n = 13;
    auto fac = calc_factorial(n);
    copy(fac.begin() + 1, fac.end(), ostream_iterator<int>(cout, " "));
    cout << endl;
    dp_solution(n, fac);

    return 0;
}