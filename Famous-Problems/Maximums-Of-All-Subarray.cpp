/****
  ** Time Complexity : O(n)
  ** Space Complexity : O(n) // this can be optimized to O(k) by improving insertValue fxn
****/

deque<pair<int, int>> dq;
void insertValue(int val, int x)
{
	// if we change dq.pop_back().first < val to dq.pop_back().first > val
	// then we will get minimums of all subarrays

	while(!dq.empty() && dq.back().first < val)
		dq.pop_back();
	dq.push_back({val, x});
}

int findAllSubarrayMaximum(const vector<int> &a, vector<int> &ans, int k)
{
	int n = a.size();
	ans.resize(n - k + 1);
	dq = deque<pair<int, int>>();

	for(int i = 0 ; i < k ; i++)
		insertValue(a[i], i);

	int fir = 1;
	ans[0] = dq.front().first;
	for(int i = k ; i < n ; i++)
	{
		insertValue(a[i], i);
		while(dq.front().second <= i - k)
			dq.pop_front();
		ans[fir] = dq.front().first;
		fir++;
	}
	return n - k + 1;
}