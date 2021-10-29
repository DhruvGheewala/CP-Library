int n, k;
vector<int> a;

// partition fxn of original quicksort
int partition_original(int l, int r)
{
    int piv = l;
    int pivot = a[piv];

    while(l < r)
    {
        while(l < n && a[l] <= pivot) l++;
        while(r >= 0 && a[r] > pivot) r--;
        if(l < r) swap(a[l], a[r]);
    }
    swap(a[piv], a[r]);
    return r;
}

// randomization
int partition(int l, int r)
{
    int piv = rnd(min(l + 1, r), r);
    swap(a[l], a[piv]);
    return partition_original(l, r);
}

int QuickSort(int l = 0, int r = n - 1)
{
    int ans;
    
    if(l < r) {
        int m = partition(l, r);
        QuickSort(l, m - 1);
        QuickSort(m + 1, r);
    }
    
    return ans;
}