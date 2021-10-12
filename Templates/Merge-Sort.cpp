// Variations
// 1. https://leetcode.com/problems/count-of-smaller-numbers-after-self
// 2. https://leetcode.com/problems/reverse-pairs/
// 3. 

vector<int> store;
void combine(int l, int m, int r, vector<int> &arr) {
	int i = l, j = m + 1, k = l;
	while(i <= m && j <= r) {
		if(arr[i] <= arr[j]) {
			store[k] = arr[i];
			i++, k++;
		} else {
			store[k] = arr[j];
			j++, k++;
		}
	}

	while(i <= m) {
		store[k] = arr[i];
		i++, k++;
	}

	while(j <= r) {
		store[k] = arr[j];
		j++, k++;
	}

	for(int i = l; i <= r; i++)
		arr[i] = store[i];
}

void mergeSortRec(int l, int r, vector<int> &arr) {
	if(l < r) {
		int m = (l + r) / 2;
		mergeSortRec(l, m, arr);
		mergeSortRec(m + 1, r, arr);
		combine(l, m, r, arr);
	}
}

void mergeSort(vector<int> &arr) {
	int n = arr.size();
	store.resize(n);
	mergeSortRec(0, n - 1, arr);
}