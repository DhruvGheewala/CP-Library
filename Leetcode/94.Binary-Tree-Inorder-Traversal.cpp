// Link: https://leetcode.com/problems/binary-tree-inorder-traversal/

/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode() : val(0), left(nullptr), right(nullptr) {}
 *     TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
 *     TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
 * };
 */
class Solution {
public:
    vector<int> inorderTraversal(TreeNode* root) {
        // There are n - 1 edges and we will create the threaded binary tree
        // w/o changing the structure and each edge will be visisted at most thrice
        // so complexity will be O(n)
        
        vector<int> result;
        TreeNode *ptr = root;
        while(ptr) {
            // Find inorder predecesor for each left nodes
            // bcz predecesor is rightmost node in left child
            if(ptr -> left) {
                TreeNode *rightMost = ptr -> left;
                while(rightMost -> right && rightMost -> right != ptr) {
                    rightMost = rightMost -> right;
                }
                
                // create thread
                if(rightMost -> right == nullptr) {
                    rightMost -> right = ptr;
                    ptr = ptr -> left;
                }
                // remove the thread, In this condition
                // edges will be traversed thrice
                else {
                    // we visited the whole left subtree
                    // of rightMost -> right
                    // so processs it and goto right
                    rightMost -> right = nullptr;
                    result.push_back(ptr -> val);
                    ptr = ptr -> right;
                }
            }
            // we visited left, root, now goto right
            else {
                result.push_back(ptr -> val);
                ptr = ptr -> right;
            }
        }
        return result;
    }
};