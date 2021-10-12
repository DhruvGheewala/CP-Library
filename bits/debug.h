
#ifndef __CUSTOM_DEBUG_HEADER_INCLUDED__

#define __CUSTOM_DEBUG_HEADER_INCLUDED__

#include <bits/stdc++.h>
using namespace std;

// show line number
#ifdef __INCLUDE_LINE_NUMBER__
#define cline cerr << "Line: " << __LINE__ << ", "
#else
#define cline cerr
#endif

// if debug is enabled
#ifdef DEBUG

#define endl '\n'
#define fundri cerr << "----------------------------" << endl

// use tourist macro
#ifdef __USE_TOURIST_DEBUG__
#define debug(...) cline, debug_out_tourist(#__VA_ARGS__, __VA_ARGS__)
#else
#define debug(...) cline, debug_out_separate(#__VA_ARGS__, __VA_ARGS__)
#endif

#endif

string to_string(string s) {
    return '"' + s + '"';
}

string to_string(char* s) {
    return to_string((string) s);
}

// string to_string(const string &s) {
//     return "";
// }

string to_string(const char* s) {
    return "";
}

string to_string(const char c) {
    string res(3, '\'');
    res[1] = c;
    return res;
}

string to_string(const bool b) {
    return (b ? "1" : "0");
}

string to_string(const _Bit_reference b) {
    return (b ? "1" : "0");
}

template<class T, class Container, class Compare>
string to_string(priority_queue<T, Container, Compare> pq);

template<typename A, typename B>
string to_string(pair<A, B> p);

string to_string(vector<bool> v) {
    bool first = true;
    string res = "{";
    int n = static_cast<int>(v.size());
    for(const auto &b: v) {
        if(!first) {
            res += ", ";
        }
        first = false;
        res += to_string(b);
    }
    res += "}";
    return res;
}

template<size_t N>
string to_string(bitset<N> v) {
    string res = "";
    for(size_t i = 0; i < N; i++)
        res += static_cast<char>('0' + v[i]);
    return res;
}

template<typename T>
string to_string(T v);

template<typename T>
string to_string(queue<T> q)
{
    // queue<T> q = _q;
    string res = "{";
    bool first = true;
    while(!q.empty()) {
        if(!first) {
            res += ", ";
        }
        first = false;
        res += to_string(q.front());
        q.pop();
    }
    res += "}";
    return res;
}

template<class T, class Container, class Compare>
string to_string(priority_queue<T, Container, Compare> pq)
{
    string res = "{";
    bool first = true;
    while(!pq.empty()) {
        if(!first) {
            res += ", ";
        }
        first = false;
        res += to_string(pq.top());
        pq.pop();
    }
    res += "}";
    return res;
}

template<typename T>
string to_string(stack<T> st)
{
    string res = "{";
    bool first = true;

    while(!st.empty()) {
        if(!first) {
            res += ", ";
        }
        first = false;
        res += to_string(st.top());
        st.pop();
    }
    res += "}";
    return res;
}

template<typename T>
string to_string(T v) {
    bool first = true;
    string res = "{";
    for(const auto &x: v) {
        if(!first) {
            res += ", ";
        }
        first = false;
        res += to_string(x);
    }
    res += "}";
    return res;
}

template<typename A, typename B>
string to_string(pair<A, B> p) {
    return "{" + to_string(p.first) + ", " + to_string(p.second) + "}";
}

// ============================================ \\

template<typename H>
void show(string name, H arg, bool last = false)
{

    cerr << name;
    if(to_string(arg) != "") {
        cerr << " = " << to_string(arg);
    }

    if(last) cerr << endl;
    else cerr << ", ";
}

// std macro for debuging
template<typename T>
void debug_out_separate(string name, T arg) {show(name, arg, true);}
// template<typename T>
// void debug_out_separate() {cerr << endl;}

template<typename H, typename... T>
void debug_out_separate(string names, H arg, T ...args)
{
    int ind = names.find(',');
    show(names.substr(0, ind), arg);
    debug_out_separate(names.substr(ind + 2), args...);
}

// ============================================ \\

// tourist's macro for debuging
template<typename T>
void debug_out_tourist() {cerr << endl;}

template<typename H, typename... T>
void debug_out_tourist(H arg, T ...args)
{
    cerr << to_string(arg) << ", ";
    debug_out_tourist(args...);
}

#endif