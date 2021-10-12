1.  Cayley's Formulas generalized Version is:

    - k \* n ^ (n - k - 1)
    - k : number of connected components in forest
    - n : number of vertices

2.  MST Can Be Obtained By :

    - Krushkal
    - Prim - faster than krushkal if graph is dense enough
    - Boruvka - faster randomized algorithm that works in linear time O(E) \* oldest minimum spanning tree algorithm
    - Reverse-Delete-Algorithm (almost reverse of krushkal)

3.  Diophantine Equation :

    - g = gcd(A, B)
    - Ax + By = C have infinite solutions, iff g | C (read : g devides C)
    - Here x or y Any/Both can be Negative
    - Ax' + By' = g
    - where, we can find x' and y' using extended euclidian algorithm
    - solution for our equation is,
    - (x, y) = ((x' \* C + i \* B) / g, (y' \* C - i \* A) / g)
    - i is integer, by changing it, we can get all solutions

4.  Chicken Mc-nugget Theorem:

    - [Brilliant](https://brilliant.org/wiki/)
    - postage-stamp-problem-chicken-mcnugget-theorem/
    - [Codechef - bezout's identity, but explanation to make x and y positive](https://discuss.codechef.com/t/givcandy-editorial/12628)

5.  Fibonacci numbers :

    - fib(2 \* k) = fib(k) \* fib(k) + fib(k - 1) \* fib(k - 1)
    - fib(2 \* k + 1) = fib(k) \* fib(k + 1) + fib(k - 1) \* fib(k)

    Code :

    ```cpp
    /***
      ** Time Complexity : O(logn * log(logn))
      ** Space Complexity : O(Recursion Stack + apprx. logn * log(logn))
    ****/

    map<int, int> cache;

    int fib_recursive(int n) {
        // base case
        if(n <= 3) return max(1, n);

        // ignoring overlapping sub-structure
        if(cache.count(n)) return cache[n];

        if(n & 1) {
            int a = fib_recursive(n / 2);
            int b = fib_recursive(n / 2 - 1);
            return a * a + b * b;
        }

        int a = fib_recursive((n - 1) / 2);
        int b = fib_recursive((n - 1) / 2 + 1);
        int c = fib_recursive((n - 1) / 2 - 1);
        return a * (b + c);
    }

    /* ================ OR ================== */

    void fib_euclid(int n, int &x, int &y) {
        if(n == 0) {
            x = 0;
            y = 1;
            return;
        }

        if(n & 1) {
            fib_euclid(n - 1, y, x);
            y += x;
            return;
        }

        int a, b;
        fib_euclid(n / 2, a, b);
        y = a * a + b * b;
        x = a * b + a * (b - a);
    }

    int fib_euclid(int n) {
        int x, y;
        fib_euclid(n, x, y);
        return x;
    }
    ```

6.  - ~x = -(x + 1)
    - ~x = 1's complement of x(or inverting all the bits of x)
    - -x = 2's complement of x(or inverting all the bits to the left of least significant set bit of x)
