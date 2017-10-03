/**
 * 考虑有向无权图的最短路径算法，选定一个起点s之后，遍历得到s的邻接顶点，然后将其入队，再遍历队列中的顶点，知道队列为空
 * 时间复杂度为O(E+V)
 * 典型的广度优先搜索算法，用一个队列保存当前已经入队的顶点
 */

#include <iostream>
#include <queue>
#include <vector>
#include <list>
#include <iterator>

using namespace std;

#define INF 99999

class DAG {

private:
    vector<list<int>> graphs;   // 图的邻接表
    vector<int> dist;           // 图中每个顶点到起点的距离
    vector<int> path;
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
        graphs.assign(n+1, list<int>());
        dist.assign(n+1, INF);
        path.assign(n+1, 0);
        // 生成邻接表和计算顶点的入度
        while(m--) {
            cin >> v1 >> v2;
            graphs[v1].push_back(v2);
        }
    }
    
    void bfs() {
        int start;
        cin >> start;
        queue<int> q;
        q.push(start);
        dist[start] = 0;
        path[start] = start;
        int v;
        while(!q.empty()) {
            // 取出一个入度为0的顶点
            v = q.front();
            q.pop();

            // 将其所指向的顶点的入度减1，然后判断将入度为0的加入stack
            for(auto it = graphs[v].begin(); it != graphs[v].end(); it++) {
                if(dist[*it] == INF) {
                    dist[*it] = dist[v] + 1;
                    path[*it] = v;
                    q.push(*it);
                }
            }
        }

        std::copy(dist.begin(), dist.end(), ostream_iterator<int>(cout, " "));
        std::cout << std::endl;
        std::copy(path.begin(), path.end(), ostream_iterator<int>(cout, " "));
    }
};


int main() {

    DAG gbfs;
    gbfs.createGraph();
    gbfs.bfs();

    getchar();
    return 0;
}