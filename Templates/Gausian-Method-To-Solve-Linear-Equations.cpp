/***
  ** Matrix type
  ** Equations : 
  **    A(0,0)X(0) + A(0,1)X(1) + ... = B(1), etc..
  **    A(n-1,0)X(1) + A(n-1,1)X(2) + ... = B(n), etc...
  ** mat vector<vector<T>>(n, vector<T>(n + 1))
  ** where mat[i][n] = Bi, mat[i][j] = A(i,j) for : j < n
***/
typedef vector<long double> vld;  // for solution(roots)

template<typename T> int fwdElim(vector<vector<T>> &mat);
template<typename T> vld backSub(vector<vector<T>> &mat);

template<typename T>
vld solveGauss(vector<vector<T>> &mat)
{
    int ind = fwdElim(mat);
    
    // Matrix is singular, i.e. |mat| = 0
    if(ind != -1)
    {
        if(mat[ind].back() == 0)
            cerr << "Infinitely many solutions" << endl;
        else
            cerr << "Inconsistent System" << endl;

        assert(false);
    }
    return backSub(mat);
}

// r.e.f. : Row echelon form
// Convert mat to r.e.f
template<typename T>
int fwdElim(vector<vector<T>> &mat)
{
    int n, i_max, v_max;
    n = mat.size();
    
    for(int k = 0 ; k < n ; k++) {
        i_max = k;
        v_max = mat[i_max][k];

        for(int i = k + 1 ; i < n ; i++) {
            if(abs(mat[i][k]) > v_max) {
                v_max = mat[i][k];
                i_max = i;
            }
        }

        if(mat[k][i_max] == 0)
            return k; // matrix is singular

        // swapping row of mat
        if(i_max != k) swap(mat[i_max], mat[k]);

        for(int i = k + 1 ; i < n ; i++) {
            
            long double div = 1.0 * mat[i][k] / mat[k][k];
            for(int j = k + 1 ; j <= n ; j++)
                mat[i][j] -= 1.0 * mat[k][j] * div;
            mat[i][k] = 0;
        }
    }

    return -1;  // succes flag
} 

// fxn to find roots
template<typename T>
vld backSub(vector<vector<T>> &mat)
{
    int n = mat.size();
    vld a(n);

    for(int i = n - 1 ; i >= 0 ; i--) {

        // rhs of equation
        a[i] = mat[i][n];
        for(int j = i + 1 ; j < n ; j++)
            a[i] -= 1.0 * mat[i][j] * a[j];

        a[i] /= (1.0 * mat[i][i]);
    }
    return a;
}