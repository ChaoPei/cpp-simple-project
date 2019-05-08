/**
 * kmp字符串匹配算法
 * 
 * 算法分为两部分：先求子串（要匹配的字符串）的部分匹配值，next数组
 * 然后在匹配的过程中，用当前已匹配的字符串长度，减去第一个未匹配字符所对应的最小匹配值，得到字符串移动的长度
 *
 * "部分匹配值"就是"前缀"和"后缀"的最长的共有元素的长度。
 * "前缀"指除了最后一个字符以外，一个字符串的全部头部组合；
 * "后缀"指除了第一个字符以外，一个字符串的全部尾部组合。
 */

#include <vector>
#include <iostream>
#include <string>
#include <iterator>

#define _DEBUG

using namespace std;

vector<int> get_next(string &pattern) {
    vector<int> next(pattern.size());

    // 第一个字符的前缀和后缀都是空集。
    // 从第二个字符开始，依次计算每个字符对应的部分匹配值
    int k = 0;  // k标记当前，前缀和后缀集合中，共有的字符串的最大长度
    for(int i = 1; i < pattern.size(); ++i) {
        while(k > 0 && pattern[i] != pattern[k]) {  // 在前一个位置的k不为0，但是却不相等，那么减少k，重新寻找与P[q]相等的位置，让下面的if来增加k
            //while循环是整段代码的精髓所在
            k = next[k - 1];                        
        }
        if(pattern[i] == pattern[k]) {              //如果相等，那么最大相同前后缀长度加1
            //增加k的唯一方式
            k++;
        }
        next[i] = k;
    }
    return next;
}

void kmp(string &s, string &pattern) {
    vector<int> next = get_next(pattern);
    
    // i是字符在s中的位置，j是在pattern中的位置
    for(int i = 1, j = 0; i < s.size(); ++i) {
        
        // 如果j大于0，说明前缀已经有一部分匹配了
        while(j > 0 && s[i] != pattern[j]) {    
            j = next[j - 1];    // 如果不匹配，则根据部分匹配值来进行位置调整（原串不动，匹配串利用自动匹配值移动）
        }

        // 如果字符匹配，两个位置同时向后移动
        if(s[i] == pattern[j]) {
            j++;
        }
        // 完全匹配，返回在原字符串中开始匹配的位置
        if(j == pattern.size()) {
            cout << "find match: " << i - pattern.size() + 1 << endl;
            // 继续寻找
            j = next[j - 1];
        }
    }
}

int main() {
    string s("BBC ABCDAB ABCDABCDABDE");
    string pattern("ABCDABD");

    kmp(s, pattern);    // 15

    return 0;
}