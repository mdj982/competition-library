template<class T>
class SegTree {
protected:
	int n, N; // n is the original size, while N is the extended size
	int base;
	vector<T> nodes;
	int left_of(int index) {
		if (index >= base) return -1;
		else return index * 2 + 1;
	}
	int right_of(int index) {
		if (index >= base) return -1;
		else return index * 2 + 2;
	}
	int parent_of(int index) {
		if (index == 0) return -1;
		else return (index - 1) >> 1;
	}
	// initially, (s, t, 0, N, 0, ...);
	void rec(int s, int t, int l, int r, int index, int x, int v) {
		// lazy propagation process
		if (s == l && t == r) {
			// leaf process
		}
		else {
			int mid = (l + r) / 2;
			int index_l = left_of(index);
			int index_r = right_of(index);
			if (s < mid && mid < t) {
				rec(s, mid, l, mid, index_l, x, v);
				rec(mid, t, mid, r, index_r, x, v);
			}
			else if (s < mid) {
				rec(s, t, l, mid, index_l, x, v);
				// opposite process
			}
			else if (mid < t) {
				rec(s, t, mid, r, index_r, x, v);
				// opposite process
			}
			// merge process
		}
	}
public:
	SegTree(int n, T init) {
		SegTree::n = n;
		N = (int)pow(2, ceil(log2(n)));
		base = N - 1;
		nodes = vector<T>(base + N, init);
	}
	// inclusive
	vi get_index_above(int index, bool from_base = false) {
		if (from_base) index += base;
		vi ret;
		while (index >= 0) {
			ret.push_back(index);
			index = parent_of(index);
		}
		return ret;
	}
	int cover_size(int index) {
		int cnt = 1;
		while (left_of(index) != -1) {
			index = left_of(index);
			cnt *= 2;
		}
		int l = index - base;
		int r = min(l + cnt, n);
		return max(0, r - l);
	}
};