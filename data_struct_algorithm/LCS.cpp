/**
 * 最长公共子串（Longest Common Substring）
 * 
 * 动态规划的思想：假设两个字符串分别为s和t，s[i]和t[j]分别表示其第i和第j个字符(字符顺序从0开始)，
 * 再令L[i, j]表示以s[i]和s[j]为结尾的相同子串的最大长度。应该不难递推出L[i, j]和L[i+1,j+1]之间
 * 的关系，因为两者其实只差s[i+1]和t[j+1]这一对字符。若s[i+1]和t[j+1]不同，那么L[i+1, j+1]自然
 * 应该是0，因为任何以它们为结尾的子串都不可能完全相同；而如果s[i+1]和t[j+1]相同，那么就只要在以
 * s[i]和t[j]结尾的最长相同子串之后分别添上这两个字符即可，这样就可以让长度增加一位。
 * 
 * 最长公共子串的递推公式：
 *  - L[i+1, j+1] = (s[i] == t[j] ? L[i,j] + 1 : 0)
 *
 * 最长公共子序列的递推公式：
 *  - 如果str1[m] == str2[n]，则L[m,n] = L[m - 1, n -1] + 1
 *  - 如果str1[m] != str2[n]，则L[m,n] = max{L[m,n - 1]，L[m - 1, n]}
 */

#include <vector>
#include <iostream>
#include <string>

#define _DEBUG

using namespace std;

int longest_common_substring(const string &s1, const string &s2) {
    size_t sz1 = s1.size();
    size_t sz2 = s2.size();

    if(sz1 == 0 || sz2 ==0) {
        return 0;
    }

    // 记录L[i, j]
    vector<vector<int>> table(sz1, vector<int>(sz2));

    // 初始化table[0, j]，s2的每个字符和s1[0]进行比较
    for(int j = 0; j < sz2; ++j) {
        table[0][j] = s2[j] == s1[0] ? 1 : 0;
    }

    // 推导求解
    for(int i = 1; i < sz1; ++i) {
        // 初始化table[i, 0]
        table[i][0] = s1[i] == s2[0] ? 1 : 0;
        for(int j = 1; j < sz2; ++j) {
            if(s1[i] == s2[j]) {
                table[i][j] = table[i - 1][j - 1] + 1;  
            }
            // else，则默认为0
        }
    }
    
    // 确定最大值和起始位置
    int start1 = -1;
    int start2 = -1;
    int  longest = 0;
    for(int i = 0; i < sz1; ++i) {
        for(int j = 0; j < sz2; ++j) {
            if(table[i][j] > longest) {
                longest = table[i][j];
                start1 = i - longest + 1;
                start2 = j - longest + 1;
            }
        }
    }

#ifdef _DEBUG
    cout<< "(first, second, longest) = (" 
    << start1 << ", " << start2 << ", " << longest
    << ")" << endl;
#endif

    return longest;
}

/**
 * path用来记录两个字符串对应位置的字符是否相同
 * path[i, j]表示s1中第i个字符：s1[i - 1]，和s2中第j个字符：s2[j - 1]之间的关系
 */
int longest_common_subsequence(const string &s1, const string &s2, vector<vector<int>> &path) {
    size_t sz1 = s1.size();
    size_t sz2 = s2.size();

    // 和最长子串不一样，子序列不要求连续，所以table中的长度一定是递增的
    vector<vector<int>> table(sz1, vector<int>(sz2));
    
    for(int j = 0; j < sz2; ++j) {
        table[0][j] = s2[j] == s1[0] ? 1 : 0;
    }

    for(int i = 1; i < sz1; ++i) {
        table[i][0] = s1[i] == s2[0] ? 1 : 0;
        for(int j = 1; j < sz2; ++j) {
            if(s1[i - 1] == s2[j - 1]) {
                // 如果s1第i个字符和s2第j个字符相等
                table[i][j] = table[i - 1][j - 1] + 1;
                path[i][j] = 0;
            }
            else {
                // 如果不想等，则根据递推公式取前面子序列的最大值
                table[i][j] = max(table[i-1][j], table[j][i-1]);

                // 记录状态
                if(table[i - 1][j] >= table[i][j - 1]){
                    path[i][j] = 1;
                } else {
                    path[i][j] = 2;
                }
            }
        }
    }

#ifdef _DEBUG
    // TODO: 根据path打印子序列
#endif

    return table[sz1 - 1][sz2 - 1];
}

int main() {
    string s1 = "abcdefghijk";
    string s2 = "efgh";
    string s3 = "aefgij";

    int lcs1 = longest_common_substring(s1, s2);

    vector<vector<int>> path(s1.size(), vector<int>(s2.size(), -1));
    int lcs2 = longest_common_subsequence(s1, s3, path);

    cout << lcs1 << endl;
    cout << lcs2 << endl;

    return 0;
}