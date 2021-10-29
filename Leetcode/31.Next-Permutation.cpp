// Link: https://leetcode.com/problems/next-permutation/

class Solution {
public:
    void nextPermutation(vector<int>& nums) {
        int n = nums.size();
        for(int i = n - 2; i >= 0; i--) {
            if(nums[i] < nums[i + 1]) {
                // choosen suffix will be [i + 1, n - 1]
                // which will be the last permutation, i.e. reverse sorted
                
                // Task: find next permutation of suffix [i, n - 1]
                
                // replace the value of i by smallest greater number
                int pivot = i + 1;
                for(int j = i + 1; j < n; j++) {
                    if(nums[j] > nums[i])
                        pivot = j;
                }
                swap(nums[i], nums[pivot]);
                
                // reversing the suffix [i + 1, n - 1]
                // bcoz, we increase the value of nums[i]
                // so, We dont have to check for suffix [i + 1, n - 1]
                // the suffix [i, n - 1] will be anyways greater then previous
                // so better to take first permutation of suffix [i + 1, n - 1]
                // which is sorted order
                for(int l = i + 1, r = n - 1; l < r; l++, r--)
                    swap(nums[l], nums[r]);
                return;
            }
        }
        
        // reverse sorted then initialize it to first permutation
        for(int l = 0, r = n - 1; l < r; l++, r--)
            swap(nums[l], nums[r]);
    }
};