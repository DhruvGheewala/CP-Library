class SegmentTree {
private:
	int n;
	vector<int> tree;

	int merge(int lhs, int rhs) {
		return lhs + rhs;
	}

public:
	SegmentTree() {}

	SegmentTree(const vector<int> &a) {
		n = a.size();
		tree.assign(2 * n + 1, 0);

		// a[i] == tree[i + n]
		for(int i = n, j = 0; j < n; i++, j++)
			tree[i] = a[j];

		for(int i = n - 1; i > 0; i--)
			tree[i] = merge(tree[2 * i], tree[2 * i + 1]);
	}

	void update(int idx, int val) {
		idx += n;
		tree[idx] = val; // update a[idx - n]

		while(idx > 0) {
			int left = idx;
			int right = idx;

			if(idx % 2 == 0) right++;
			else left--;

			idx >>= 1;
			// left is left child of idx
			// right is right child of idx
			tree[idx] = merge(tree[left], tree[right]);
		}
	}

	int sum(int L, int R) {
		int result = 0;
		int l = L + n, r = R + n;
		while(l <= r) {
			// At the end of this iteration
			// We have the answer of range: [L, l] && [r, R]

			if(l % 2 == 1) {
				// l is in right subtree
				// of it's parent
				// so include it
				// and goto uncle

				// Otherwise, We have to include
				// whole segement so goto parent
				result += tree[l];
				l++;
			}

			if(r % 2 == 0) {
				// r is in the left subtree
				// of it's parent
				// so include it
				// and goto uncle

				// Otherwise, We have to include
				// whole segement so goto parent
				result += tree[r];
				r--;
			}

			l >>= 1;
			r >>= 1;
		}
		return result;
	}
};