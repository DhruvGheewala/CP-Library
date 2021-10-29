// Link: https://leetcode.com/problems/permutation-sequence/

class Solution {
public:
    string getPermutation(int n, int k) {
        int factorial = 1;
        vector<int> values;
        for(int i = 1; i <= n; i++) {
            values.push_back(i);
            factorial *= i;
        }
        
        k = k - 1;
        string res = "";
        while(!values.empty()) {
            factorial /= values.size();
            int l = 0, r = factorial - 1;
            for(int i = 0; i < values.size(); i++) {
                if(l <= k && k <= r) {
                    res += to_string(values[i]);
                    values.erase(values.begin() + i);
                    k = k % factorial;
                    break;
                }
                
                l = r + 1;
                r = l + factorial - 1;
            }
        }
        return res;
    }
};