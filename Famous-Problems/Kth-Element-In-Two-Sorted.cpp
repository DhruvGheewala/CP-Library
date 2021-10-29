const int inf = 1e9;

// 0-based kth element
int kthElement(vector<int> &a, vector<int> &b, int k) {
    int n = a.size();
    int m = b.size();
    
    if(n > m) {
        swap(a, b);
        swap(n, m);
    }

    int l = 1, r = n;
    while(l <= r) {
        // cut1 + cut2 = k + 1
        // cut2 = k - cut1 + 1
        int cut1 = (l + r) / 2;
        int cut2 = k - cut1 + 1;

        // k + 1 = cut1 + cut2

        int left1 = (cut1 - 1 >= 0) ? a[cut1 - 1] : -inf;
        int left2 = (cut2 - 1 >= 0) ? b[cut2 - 1] : -inf;
        int right1 = (cut1 < n) ? a[cut1] : inf;
        int right2 = (cut2 < m) ? b[cut2] : inf;

        if(left1 <= right2 && left2 <= right1) {
            return max(left1, left2);
        }

        if(left1 > right2) {
            r = cut1 - 1;
        } else {
            l = cut1 + 1;
        }
    }
    return b[k];
}