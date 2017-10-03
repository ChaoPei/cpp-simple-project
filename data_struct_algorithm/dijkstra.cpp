/**
 * 有向加权图的最短路径算法，贪心算法的典型实例
 * 
 * 和DAG_BFS类似，每次将邻接点加入队列中去，不同之处在于，需要保存点是否已经被遍历的信息，
 */

#include <iostream>
#include <queue>
#include <vector>
#include <list>
#include <iterator>
#include <utility>

using namespace std;

#define INF 99999

class Dijkstra {

private:
    // 索引下标就是顶点序号
    vector<list<pair<int, int>>> graphs;    // 图的邻接表，list保存邻接的顶点，pair是顶点的坐标和路径的权值
    vector<int> dist;                       // 图中每个顶点到起点的距离
    vector<int> pre_vex;                    // 路径中每个点的前一顶点，初始化为-1
public:

    /**
     * 创建图，生成图的邻接表，保存顶点的入度信息
     * 
     * 输入：
     * n m          // n是顶点个数，m是边的个数
     * v1 v2 value  // v1是边的起点，v2是边的终点，value是权值
     * v1 v2 value  
     * ...
     */
    void createGraph()
    {
        int n, m, v1, v2, value;
        cin >> n >> m;
        graphs.assign(n+1, list<pair<int, int>>());
        dist.assign(n+1, INF);
        pre_vex.assign(n+1, -1);
        // 生成邻接表和计算顶点的入度
        while(m--) {
            cin >> v1 >> v2 >> value;
            graphs[v1].push_back(make_pair(v2, value));
        }
    }
    
    void bfs() {
        int start;
        cin >> start;
        queue<int> q;
        q.push(start);
        dist[start] = 0;
        // pre_vex[start] = start;
        int v;
        while(!q.empty()) {
            // 取出一个入度为0的顶点
            v = q.front();
            q.pop();

            // 将其所指向的顶点的入度减1，然后判断将入度为0的加入stack
            for(auto it = graphs[v].cbegin(); it != graphs[v].cend(); it++) {
                int curValue = dist[v] + (*it).second;
                // 注意此处的判断，根据当前遍历的顶点来对之前的顶点进行调整，这地方把之前遍历和没遍历过的放在一起处理了（没遍历过的距离是无穷大）
                if(dist[(*it).first] >= curValue) {      
                    dist[(*it).first] = curValue;
                    pre_vex[(*it).first] = v;
                    q.push((*it).first);
                }
            }
        }
        
        std::copy(++dist.begin(), dist.end(), ostream_iterator<int>(cout, " "));
        std::cout << std::endl;
        std::copy(++pre_vex.begin(), pre_vex.end(), ostream_iterator<int>(cout, " "));
    }
};


int main() {

    Dijkstra dijkstra;
    dijkstra.createGraph();
    dijkstra.bfs();

    getchar();
    return 0;
}