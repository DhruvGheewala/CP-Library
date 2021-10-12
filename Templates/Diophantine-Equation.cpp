class DiophantineEquationSolver {
private:
    void shiftSolution(int &x, int &y, int a, int b, int cnt)
    {
        x += cnt * b;
        y -= cnt * a;
    }

    // a * x + b * y = gcd(a, b)
    int gcd(int a, int &x, int b, int &y)
    {
        if(b == 0) {
            x = 1;
            y = 0;
            return a;
        }
        
        int xg, yg;
        int g = gcd(b, xg, a % b, yg);
        x = yg;
        y = xg - yg * (a / b);
        return g;
    }

    bool findAnySolution(int a, int &x, int b, int &y, int c, int &g)
    {
        g = gcd(abs(a), x, abs(b), y);
        if(c % g != 0) return false;

        x *= (c / g);
        y *= (c / g);

        if(a < 0) x *= -1;
        if(b < 0) y *= -1;
        return true;
    }

public:
    // a * x + b * y = c
    bool findAnySolution(int a, int &x, int b, int &y, int c) {int g;return findAnySolution(a, x, b, y, c, g);}

    /**
      * case isn't handled for a == 0 || b == 0
      * returns {count of solution, vector<pii>}

      * vector<pii> contains solutions for corner cases like, 
      * [0] -> minx
      * [1] -> maxx
      * [2] -> miny
      * [3] -> maxy
    **/
    pair<int, vector<pii>> findAllSolutions(int a, int b, int c, 
        int minx = -inf, int maxx = inf, int miny = -inf, int maxy = inf) {
        vector<pii> sols(4);
        pair<int, vector<pii>> notFound = {0, vector<pii>()};

        int x, y, g;
        if(!findAnySolution(a, x, b, y, c, g))
            return notFound;

        a /= g;
        b /= g;

        int sign_a = a > 0 ? 1 : -1;
        int sign_b = b > 0 ? 1 : -1;

        // minx
        shiftSolution(x, y, a, b, (minx - x) / b);
        if(x < minx) shiftSolution(x, y, a, b, sign_b);
        if(x > maxx) return notFound;
        int lx1 = x;
        sols[0] = {x, y};
        notFound.second.push_back({x, y});

        // maxx
        shiftSolution(x, y, a, b, (maxx - x) / b);
        if(x > maxx) shiftSolution(x, y, a, b, -sign_b);
        int rx1 = x;
        sols[1] = {x, y};
        notFound.second.push_back({x, y});
            
        // miny
        shiftSolution(x, y, a, b, (y - miny) / a);
        if(y < miny) shiftSolution(x, y, a, b, -sign_a);
        if(y > maxy) return notFound;
        int lx2 = x;
        sols[2] = {x, y};

        // maxy
        shiftSolution(x, y, a, b, (y - maxy) / a);
        if(y > maxy) shiftSolution(x, y, a, b, sign_a);
        int rx2 = x;
        sols[3] = {x, y};
    
        // finding a range...
        if(lx2 > rx2) swap(lx2, rx2);
        int lx = max(lx1, lx2);
        int rx = min(rx1, rx2);

        return {
            max((rx - lx) / abs(b) + 1, int(0)),  // range
            sols  // solutions of corner cases
        };
    }    
} diophantine;