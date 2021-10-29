// Link: https://leetcode.com/problems/minimum-operations-to-make-array-equal/

class Solution {
public:
    int minOperations(int n) {
        // sum(2 * i - 1), for i = 1 to n
        // 2 * sum(i) - sum(1)
        // 2 * (n * (n + 1) / 2) - (n)
        // n * (n + 1) - n
        // n * n
        // sum = n * n
        // avg = sum / n = n
        
        int avg = n;
        
        // idea here is to make all the elements equal
        // to avg, so create mapping of mirror values
        // according to avg value, i.e. 1 3 5 7
        // avg is 4 => {1, 7}, {3, 5} mapped
        
        if(avg & 1) {
            // ans = sum(arr[i] - avg), for i = mid + 1 to n
            //     = 2 + 4 + ...
            int mid = n / 2;
            return 2 * mid * (mid + 1) / 2;
        }
        
        // ans = sum(arr[i] - avg), for i = mid + 1 to n
        //     = 1 + 3 + ...
        int mid = n / 2;
        return mid * mid;
    }
};