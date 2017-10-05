/**
 * 有向加权图的最短路径算法，贪心算法的典型实例
 * 
 * 和DAG_BFS类似，每次将邻接点加入队列中去，不同之处在于，需要保存点是否已经被遍历的信息：
 * 因为在BFS的过程中，有的顶点需要调整，则根据BFS的规则，调整过的顶点需要入队，而如果将其入队，
 * 也就是说再次放入队列，又需要遍历这个顶点的队友邻接顶点，解决办法有两种：
 *    * 保存顶点遍历信息，不使用队列，而是每次从已经遍历过的顶点中选择。（不适用与具有负权值的图，
 *      因为如果有负值，可能会使当前节点的路径实际可以取到更小，但是因为只遍历了一次，所以没法找到最小值）
 *    * 忽略known设定，每次更新都将顶点入队，重新遍历。问题是如果存在负值回路（一个环形，整个环形的权值为负值），
 *      则可能会导致无限循环。设置停止条件，遍历了最多V（顶点个数）次之后停止。（适用于负权值的图）
 * 
 * 算法步骤：
 * 分阶段进行，第一阶段选择一个起点，然后更新与这个所有点的距离信息，
 * 接着从其余所有点中找到距离最小的一个点，然后更新所有点的信息，
 * 当所有点都被选择了一遍之后，算法结束。
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
        }
        cout << "complete create graphs" << endl;
    }

    void updateAdj(int v) {

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
                    cout << (*it).first << " ";
                    // 并且当前的距离小于已知的距离，则更新节点
                    int curValue = dist[start] + (*it).second;
                    if(curValue < dist[(*it).first]) {
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
                if(dist[i] < minDist && !known[i]) {
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

    // 7 12 1 2 2 1 4 1 2 4 3 2 5 10 3 1 4 3 6 5 4 3 2 4 6 8 4 7 4 4 5 2 5 7 6 7 6 1 1
    Dijkstra dijkstra;
    dijkstra.createGraph();
    dijkstra.bfs();

    getchar();
    return 0;
}