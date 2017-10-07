/**
 * 无向图的最小生成树Prim算法：从选定的一个根节点开始，每次选择往树上添加一条最短路径，所以很像Dijkstra
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
    vector<int> known;                      // 节点是否已经遍历过了的标记
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
        // 初始化
        int n, m, v1, v2, value;
        cin >> n >> m;
        graphs.assign(n+1, list<pair<int, int>>());
        dist.assign(n+1, INF);
        pre_vex.assign(n+1, -1);
        known.assign(n+1, 0);
        
        // 生成邻接表
        while(m--) {
            cin >> v1 >> v2 >> value;
            graphs[v1].push_back(make_pair(v2, value));
            graphs[v2].push_back(make_pair(v1, value));
        }
        cout << "complete create graphs" << endl;
    }

    
    void bfs() {
        int start;
        cin >> start;

        dist[start] = 0;
        known[start] = 1;

        while(true) {
            cout << "start: " << start << "  update: ";
            // 更新所有的邻接点 
            for(auto it = graphs[start].cbegin(); it != graphs[start].cend(); it++) {
                // 如果节点还未被遍历过
                if(!known[(*it).first]) {
                    // prime算法和dijkstra算法只有更新距离是不一样的，其余原理类似
                    // int curValue = dist[start] + (*it).second;
                    int curValue = (*it).second;
                    if(curValue < dist[(*it).first]) {
                        cout << (*it).first << " ";
                        dist[(*it).first] = curValue;
                        pre_vex[(*it).first] = start;
                    }
                }   
            }
            cout << "\n";

            // 从所有unknown中选出距离最小的点
            int minDist = INF;
            int minVex = 0;
            for(int i = 1; i < known.size(); ++i) {
                if(dist[i] < minDist && !known[i] && i != start) {
                    minDist = dist[i];
                    minVex = i;
                }
            }
            cout << "minDist: " << minDist << " minVex: " << minVex << "\n";

            // 如果找不到节点了，说明遍历结束了，否则以这个最小距离的点为起点，继续遍历
            if(0 == minVex) {
                break;
            } else {
                start = minVex;
                known[start] = 1;
            }
        }
        
        std::copy(++dist.begin(), dist.end(), ostream_iterator<int>(cout, " "));
        std::cout << std::endl;
        std::copy(++pre_vex.begin(), pre_vex.end(), ostream_iterator<int>(cout, " "));
    }
};


int main() {

    // 7 12 1 2 2 1 4 1 2 4 3 2 5 10 3 1 4 3 6 5 4 3 2 4 6 8 4 7 4 4 5 7 5 7 6 7 6 1 1
    Dijkstra dijkstra;
    dijkstra.createGraph();
    dijkstra.bfs();

    getchar();
    return 0;
}