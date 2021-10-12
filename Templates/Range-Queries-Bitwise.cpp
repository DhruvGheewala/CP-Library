bool atpos(int x, int pos) {return ((x & (1LL << pos)) != 0);}
int bit_count = 32;

vector<vi> RangeAndUtil;
// will give a[l] & ... & a[r] in O(32), i.e. O(1)
int range_and(int l, int r)
{
    int res = 0;
    
    for(int j = 0 ; j < bit_count ; j++)
        res += (1LL << j) * (r - l + 1 == RangeAndUtil[r + 1][j] - RangeAndUtil[l][j]);
        
    return res;
}

void build_range_and(const vi &a, bool is64bit = 0)
{
    bit_count = (1 + is64bit) * 32;
    int n = a.size();
    RangeAndUtil.assign(n + 1, vi(bit_count, 0));
    
    for(int i = 0 ; i < n ; i++) {
        for(int j = 0 ; j < bit_count ; j++)
            RangeAndUtil[i + 1][j] = RangeAndUtil[i][j] + atpos(a[i], j);
    }
}

vector<vi> RangeOrUtil;
// will give a[l] | ... | a[r] in O(32), i.e. O(1)
int range_or(int l, int r)
{
    int res = 0;
    
    for(int j = 0 ; j < bit_count ; j++)
        res += (1LL << j) * (r - l + 1 != RangeOrUtil[r + 1][j] - RangeOrUtil[l][j]);
        
    return res;
}

void build_range_or(const vi &a, bool is64bit = 0)
{
    bit_count = (1 + is64bit) * 32;
    int n = a.size();
    RangeOrUtil.assign(n + 1, vi(bit_count, 0));
    
    for(int i = 0 ; i < n ; i++) {
        for(int j = 0 ; j < bit_count ; j++)
            RangeOrUtil[i + 1][j] = RangeOrUtil[i][j] + 1 - atpos(a[i], j);
    }
}

vi RangeXorUtil;
// will give a[l] ^ ... ^ a[r] in O(1)
int range_xor(int l, int r)
{
    return RangeXorUtil[r + 1] ^ RangeXorUtil[l];
}

void build_range_xor(const vi &a)
{
    int n = a.size();
    RangeXorUtil.resize(n + 1);
    RangeXorUtil[0] = 1;
    
    for(int i = 1 ; i <= n ; i++)
        RangeXorUtil[i] = a[i - 1] ^ RangeXorUtil[i - 1];
}

// will build all
void build_range(const vi &a)
{
    build_range_and(a);
    build_range_xor(a);
    build_range_or(a);
}