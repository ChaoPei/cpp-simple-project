/**
 * 拓扑排序C++实现
 * 
 * 思路：利用一个stack（或者queue）存放所有入度为0的顶点，然后从stack（或者queue）中取出一个顶点，赋予其拓扑的序号，
 * 并将其的出度所有顶点的入度减1，然后判断这些顶点如果入度变为了0，则将其加入stack（或者queue）中。
 * 
 * 图用邻接表表示
 */

#include <iostream>
#include <stack>
#include <vector>
#include <list>
#include <iterator>

using namespace std;

class TopoSort {

private:
    vector<list<int>> graphs;
    vector<int> inDegree;
    stack<int> stk;

public:

    /**
     * 创建图，生成图的邻接表，保存顶点的入度信息
     * 
     * 输入：
     * n m     // n是顶点个数，m是边的个数
     * v1 v2   // v1是边的起点，v2是边的终点
     * v1 v2   
     * ...
     */
    void createGraph()
    {
        int n, m, v1, v2;
        cin >> n >> m;
        graphs.assign(n, list<int>());
        inDegree.assign(n, 0);
        // 生成邻接表和计算顶点的入度
        while(m--) {
            cin >> v1 >> v2;
            v1--;v2--;
            graphs[v1].push_back(v2);
            inDegree[v2]++;
        }
        // 将顶点入度为0的加入stack中
        for(int i = 0; i < n; i++) {
            if(inDegree[i] == 0) {
                stk.push(i);
            }
        }
    }
    
    void topoSort() {
        vector<int> result;
        int v;
        while(!stk.empty()) {
            // 取出一个入度为0的顶点
            v = stk.top();
            stk.pop();
            result.push_back(v + 1);

            // 将其所指向的顶点的入度减1，然后判断将入度为0的加入stack
            for(auto it = graphs[v].begin(); it != graphs[v].end(); it++) {
                if(--inDegree[*it] == 0) {
                    stk.push(*it);
                }
            }
        }
        
        // 存在环路
        if(result.size() != inDegree.size()) {
            cout << "图中存在环路" << endl;
            std::copy(result.begin(), result.end(), ostream_iterator<int>(cout, " "));
            return;
        }

        std::copy(result.begin(), result.end(), ostream_iterator<int>(cout, " "));
    }
};


int main() {

    TopoSort ts;
    ts.createGraph();
    ts.topoSort();

    getchar();
    return 0;
}