int max_subarray_sum(const vector<int> &a)
{
    int n, sum;
    n = a.size();
    sum = 0;
    vector<int> dp(n, 0);

    // one-way
    for(int i = 0 ; i < n ; i++)
    {
        sum += a[i];
        if(sum > 0)
        {
        	dp[i] = sum;
        	continue;
        }
        sum = 0;
        dp[i] = a[i];  // this can be changed according 
                       // to min size of subarray(here, it is 1)
    }

    // another-way, uncomment '0', iff min size of subarray is >= 0
    
    dp[0] = max({
        // 0,
        a[0]
    });
    for(int i = 1; i < n; i++) {
        dp[i] = max({
            // 0,
            a[i],
            dp[i - 1] + a[i]
        });
    }

    return *max_element(dp.begin(), dp.end());
}