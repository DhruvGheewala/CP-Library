// give the count of sub-segments with sum k
int countSegments(const vector<int> &a, int k)
{
	int n = a.size();
	
	map<int, int> mp;
	mp[0]++;
	
	int cnt, sum;
	cnt = sum = 0;

	for(int i = 0 ; i < n ; i++)
	{
		sum += a[i];
		if(mp.count(sum - k))
			cnt += mp[sum - k];
		mp[sum]++;
	}
	return cnt;
}