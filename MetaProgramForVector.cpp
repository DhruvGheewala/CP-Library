template<int D, typename T>
struct vec : public vector<vec<D - 1, T>>
{
	static_assert(D >= 1, "Vector dimension must be greater than zero !!");
	template<typename... Args>
	vec(int n = 0, Args... args) : vector<vec<D - 1, T>>(n, vec<D - 1, T>(args...)){}
};

template<typename T>
struct vec<1, T> : public vector<T>
{
	vec(int n = 0, T val = T()) : vector<T>(n, val){}
};

/***
	// how to declare
	vec<2, int> a(10, 20, x); --> vector<vector<int>> a(10, vector<int>(20, x))
	    ^         ^   ^   ^
	dimension
	
	vec<4, int> a(10, 20, 30, 40); --> 4d vector, by default value will be zero if not given
***/