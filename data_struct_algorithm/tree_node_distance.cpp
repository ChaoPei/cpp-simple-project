/**
 * 有一棵二叉树，树上每个点标有权值，权值各不相同，请设计一个算法算出权值最大的叶节点到权值最小的叶节点的距离。
 * 二叉树每条边的距离为1，一个节点经过多少条边到达另一个节点为这两个节点之间的距离。
 * 给定二叉树的根节点root，请返回所求距离。
 */

/**
 * 思路：因为找的是叶子节点，所以首先先序遍历，找到最大和最小权值的叶子节点，记录该节点到root的路径
 * 比较两组路径，合并父节点相同的路径，求出距离
 */

#include <vector>
#include <memory>
#include <iostream>


#define _DEBUG

using namespace std;

struct TreeNode {
    int val;
    struct TreeNode* left;
    struct TreeNode* right;

    TreeNode(int data) : val(data), left(nullptr), right(nullptr) {}
};

class Solution {

private:
    vector<TreeNode*> max_leaf_path;
    vector<TreeNode*> min_leaf_path;

public:
    // 利用DFS寻找最大和最小的叶子节点
    void dfs(TreeNode* root, int &max, int &min, vector<TreeNode*> path) {

        // 到达叶子节点后，比较当前叶子节点和已知的叶子节点值的最大值或者最小值。
        if(root->left == nullptr && root->right == nullptr) {
            if(root->val > max) {
                max = root->val;
                max_leaf_path = path;
            }

            if(root->val < min) {
                min = root->val;
                min_leaf_path = path;
            }
            return;
        }

        if(root->left) {
            path.push_back(root->left);
            dfs(root->left, max, min, path);  
            // 回溯
            path.pop_back();    // 删除当前节点
        }

        if(root->right) {
            path.push_back(root->right);
            dfs(root->right, max, min, path);
            path.pop_back();    // 回溯
        }
    }

    int calc_distrance(TreeNode* root) {
        if(!root) {
            return 0;
        }
        int max_val = root->val;
        int min_val = root->val;
        
        vector<TreeNode*> path(1, root);

        dfs(root, max_val, min_val, path);

#ifdef _DEBUG
        for(auto node : max_leaf_path) {
            cout << node->val << ' ';
        }
        cout << '\n';

        for(auto node : min_leaf_path) {
            cout << node->val << ' ';
        }
        cout << endl;
#endif

        // 寻找root到最大和最小路径上的公共父节点
        int dist = max_leaf_path.size() + min_leaf_path.size();
        for(int i = 0; i < max_leaf_path.size() && i < min_leaf_path.size(); ++i) {
            if(max_leaf_path[i] != min_leaf_path[i]) {
                break;
            }
            dist -= 2;
        }
        return dist;
    }
};

int main() {

    TreeNode node1(3);
    TreeNode node2(2);
    TreeNode node3(4);
    TreeNode node4(1);
    TreeNode node5(5);
    TreeNode node6(6);
    node1.left = &node2;
    node1.right = &node3;
    node2.left = &node4;
    node2.right = &node5;
    node3.right = &node6;

    shared_ptr<Solution> sp = make_shared<Solution>();
    int dis = sp->calc_distrance(&node1);
    cout << dis << endl;

    return 0;
}