// we can have atmost k(<= n) stock at a time
int sell_stock_problem(const vector<int> &profits, int k)
{
    int n = profits.size();
    k = min(n, k);

    vector<vector<int>> dp(2, vector<int>(k + 1, -inf));
    dp[0][0] = dp[1][0] = 0;

    for(int i = 1 ; i < n ; i++)
    {
        dp[1][0] = max(dp[0][0], dp[0][1] + profits[i]);
        dp[1][k] = max(dp[0][k], dp[0][k - 1] - profits[i]);
        
        for(int j = k - 1 ; j >= 1 ; j--)
            dp[1][j] = max({dp[0][j], dp[0][j - 1] - profits[i], dp[0][j + 1] + profits[i]});

        for(int j = 0 ; j <= k ; j++)
            dp[0][j] = dp[1][j];
    }

    // or return *max_element(dp[1].begin(), dp[1].end());
    return dp[1][0];
    // but this is good, bcoz atlast, it is better to have no stocks
}

// dp[0][] is best values for prev days
// dp[1][] is best values for current days
// dp[][i] is best values with i stocks