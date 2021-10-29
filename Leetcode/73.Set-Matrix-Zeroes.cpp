// Link: https://leetcode.com/problems/set-matrix-zeroes/

class Solution {
public:
    void setZeroes(vector<vector<int>>& matrix) {
        int n = matrix.size();
        int m = matrix[0].size();
        
        int store = matrix[0][0];
        bool row0 = false, col0 = false;
        for(int i = 0; i < n; i++) {
            if(matrix[i][0] == 0) col0 = true;
            for(int j = 0; j < m; j++) {
                if(matrix[i][j] == 0) {
                    matrix[i][0] = 0;
                    matrix[0][j] = 0;
                    row0 = (i == 0);
                }
            }
        }
        matrix[0][0] = store;
        
        for(int i = n - 1; i >= 0; i--) {
            for(int j = m - 1; j >= 0; j--) {
                if(matrix[i][0] == 0 || matrix[0][j] == 0)
                    matrix[i][j] = 0;
                else if(i == 0 && row0)
                    matrix[i][j] = 0;
                else if(j == 0 && col0)
                    matrix[i][j] = 0;
            }
        }
    }
};