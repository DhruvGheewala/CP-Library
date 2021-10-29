class MaxHeap {
private:
	const int inf = INT_MAX;
	
	int n;
	vector<int> heap;

	void heapify(int i) {
		int left = 2 * i, right = 2 * i + 1;

		int max_index = i;
		if(left < n && heap[left] > heap[max_index]) {
			max_index = left;
		}

		if(right < n && heap[right] > heap[max_index]) {
			max_index = right;
		}

		// already a perfect heap
		if(max_index == i) return;
		swap(heap[i], heap[max_index]);
		heapify(max_index);
	}

public:

	// O(n)
	MaxHeap(vector<int> &arr) {
		heap = arr;
		heap.insert(heap.begin(), inf); // 1-based
		n = heap.size();
		
		for(int i = n / 2 - 1; i >= 1; i--)
			heapify(i);
	}

	// O(logn)
	void push(int val) {
		heap.push_back(val);
		n++;

		int index = n - 1;
		int par = index / 2;
		while(par >= 1 && heap[par] < heap[index]) {
			swap(heap[par], heap[index]);
			index = par;
			par = index / 2;
		}
	}

	int size() {return n - 1;}
	bool empty() {return size() <= 0;}

	// O(1)
	int top() {return heap[1];}

	// O(logn)
	int pop() {
		int ret = top();
		n--;
		swap(heap[1], heap[n]);
		heap.pop_back();
		heapify(1);
		return ret;
	}
};