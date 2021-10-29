// Link: https://leetcode.com/problems/search-in-rotated-sorted-array/

class Solution {
private:
    const int inf = 1e9;
    
public:
    int search(vector<int>& nums, int target) {
        int n = nums.size();
        
        const auto firstHalf = [&](int x) {
            return nums[0] <= x;
        };
        
        const auto secondHalf = [&](int x) {
            return x < nums[0];
        };
        
        int l = 0, r = n - 1, mid;
        while(l <= r) {
            mid = (l + r) / 2;
            
            int x = nums[mid];
            int num = 0;
            
            if((firstHalf(x) && firstHalf(target)) || (secondHalf(x) && secondHalf(target)))
                num = nums[mid];
            else if(firstHalf(x) && secondHalf(target))
                num = -inf;
            else if(secondHalf(x) && firstHalf(target))
                num = inf;
            else
                assert(false);
            
            if(num == target)
                return mid;
            
            if(num > target) r = mid - 1;
            else l = mid + 1;
        }
        return -1;
    }
};