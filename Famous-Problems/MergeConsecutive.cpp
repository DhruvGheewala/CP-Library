template<typename U> using object = pair<vector<int>, vector<U>>;
// res[i] = {count, value}
template<typename U, typename T>
object<U> mergeConsecutive(const T &cont)
{
    int n = cont.size();
    object<U> res;

    int cnt, last;
    cnt = last = 0;
    for(int i = 0 ; i < n ; ) {
        while(i < n && cont[i] == cont[last]) {
            cnt++;
            i++;
        }

        res.first.emplace_back(cnt);
        res.second.emplace_back(cont[last]);
        cnt = 0;
        last = i;
    }

    return res;
}