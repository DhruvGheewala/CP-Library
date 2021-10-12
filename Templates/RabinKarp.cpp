/****
  **  obj.answer() will give currently found answer.(intially it is zero)
  **  obj.find() will find always new occurance of pat ot return -1
  **  if obj.find() returns -1 and then you again call it, then it will start again(reset)
  **  base_char and no_of_chars are used for compression(it doesn't affect if you make it constant)
*****/

const int mod = 1e9 + 9;
int mul(int a, int b) {return ((a % mod) * (b % mod)) % mod;}
int sum(int a, int b) {return ((a % mod) + (b % mod) + mod) % mod;}
int sub(int a, int b) {return sum(a, -b);}

class RabinKarp {
private:

    int no_of_chars, base_char;
    int lastFind, txtHash, patHash, res;
    int n, m;
    string txt, pat;
    vector<int> power;
    bool flag;

    int charToInt(char a) {return a - base_char;}

    int hash(const string &a)
    {
        int res = 0;
        int n = a.size();

        for(int i = 0 ; i < n ; i++) {
            res = mul(res, no_of_chars);
            res = sum(res, charToInt(a[i]));
        }
        return res;
    }

    void powers(int n)
    {
        power = vector<int>(n + 1);
        power[0] = 1;
        for(int i = 1 ; i <= n ; i++)
            power[i] = mul(power[i - 1], no_of_chars);
    }

public:

    RabinKarp(const string &_txt, const string &_pat)
    {
        txt = _txt;
        pat = _pat;

        n = txt.size();
        m = pat.size();
        res = -1;
        flag = (n >= m);


        int mini = min(*min_element(txt.begin(), txt.end()), *min_element(pat.begin(), pat.end()));
        int maxi = max(*max_element(txt.begin(), txt.end()), *max_element(pat.begin(), pat.end()));
        
        base_char = mini - 1;
        no_of_chars = maxi - mini + 2;
        
        lastFind = 0;
        patHash = hash(pat);
        txtHash = hash(txt.substr(0, m));
        powers(m);
    }

    int find()
    {
        if(!flag) return res = -1;

        int r = lastFind + m - 1;
        for( ; r < n ; lastFind++) {

            if(txtHash == patHash && txt.substr(lastFind, m) == pat) {
                txtHash = sub(txtHash, mul(power[m - 1], charToInt(txt[lastFind])));
                txtHash = mul(txtHash, no_of_chars);
                txtHash = sum(txtHash, charToInt(txt[r + 1]));
                return res = lastFind++;
            }
            
            r++;
            if(r >= n) break;
            
            txtHash = sub(txtHash, mul(power[m - 1], charToInt(txt[lastFind])));
            txtHash = mul(txtHash, no_of_chars);
            txtHash = sum(txtHash, charToInt(txt[r]));
        }
        
        lastFind = 0;
        patHash = hash(pat);
        txtHash = hash(txt.substr(0, m));

        return res = -1;
    }

    int answer() {return res;}
};