/*
    ### Code Starts From here ###

        a.size() = k
        a[i].size() = n
        m = n * k = total elements
        
        Time Complexities:
            Sorting: O(m * logm)
            Finding the interval: O(m)
            Overall: O(m * logn)

        Space Complexity: O(m)
*/

pair<int, int> SIKL(const vector<vector<int>> &a)
{
    pair<int, int> range = {-inf, inf};
    int k = a.size();
    assert(k >= 1);
    int n = a[0].size();
    int m = n * k;

    int cnt = 0;
    vector<pair<int, int>> list(m);
    for(int i = 0 ; i < k ; i++) {
        for(int j = 0 ; j < n ; j++) {
            list[cnt] = {a[i][j], i};
            cnt++;
        }
    }
    sort(list.begin(), list.end());

    vector<int> freq(k, 0);
    int x, left, right, save, curRange, minRange = inf;
    for(cnt = save = left = 0 ; left < m ; ++left) {
        if(left >= 1) {
            // Removing (left - 1)th entry
            x = list[left - 1].second;
            if(freq[x] == 1) cnt--;
            freq[x]--;
        }

        for(right = save ; right < m ; ++right) {
            if(cnt == k) {
                // Atleast 1 element of each list is available in range: [left, right)
                curRange = list[right - 1].first - list[left].first;
                if(curRange < minRange) {
                    minRange = curRange;
                    range = {list[left].first, list[right - 1].first};
                }
                break;
            }

            x = list[right].second;
            if(freq[x] == 0) cnt++;
            freq[x]++;
        }
        save = right;
    }
    return range;
}