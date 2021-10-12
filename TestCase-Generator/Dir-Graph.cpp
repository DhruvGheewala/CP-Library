/***
 // INPUT-FORMAT
 **** t
 **** t lines{n m
 **** m lines{u, v, w}}
***/

const int MINW = 1;
const int MINN = 5;
const int MAXN = 10;
const int MAXM = 100;
const int MAXW = 100;

// testcases
int t = rnd(1, 100);
// t = 1;
cout << t << endl;

while(t--)
{
    // no. vertices
    int n = rnd(MINN, MAXN);
    // no. edges
    int m = rnd(n - 1, min(n * (n - 1) / 2, MAXM));
    
    
    cout << n << " " << m << endl;
    
    // if there are loops too then change number limit of and size of choice and last loop too
    vector<bool> choice(n * (n - 1), 0);
    for(int i = 0 ; i < m ; i++)
        choice[i] = 1;
    shuffle(all(choice), rng);
    
    int k = 0;
    for(int i = 0 ; i < n ; i++)
    {
        for(int j = 0 ; j < n ; j++)
        {
            if(i == j)
                continue;
            if(m == 0)
                break;
    
            if(choice[k])
            {
                ll w = rnd(MINW, MAXW);
                m--;
                cout << i + 1 << " " << j + 1 << " " << w << endl;
            }
            k++;
        }
        if(m == 0)
            break;
    }
}