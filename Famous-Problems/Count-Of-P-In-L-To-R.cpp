/****
  ** Time Complexity : O(log(r))
  ** Space Complexity : O(1)
****/

// Count occurrences of p in the factorization of every element from the given range: [l, r]
int factorCount(int l, int r, int p) {
    int a, b, ans = 0;
    l = max(l, int(1));

    for(int power = p; power <= r; power *= p) {
        b = r / power;
        a = (l - 1) / power;
        ans += b - a;
    }
    return ans;
}