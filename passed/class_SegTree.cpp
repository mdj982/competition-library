template<class T>
class SegTree {
protected:
	int n, N; // n is the original size, while N is the extended size
	int base;
	vector<T> nodes;
	int left_of(int id) {
		if (id >= base) return -1;
		else return id * 2 + 1;
	}
	int right_of(int id) {
		if (id >= base) return -1;
		else return id * 2 + 2;
	}
	int parent_of(int id) {
		if (id == 0) return -1;
		else return (id - 1) >> 1;
	}
	// initially, (s, t, 0, N, 0, x, EXCEPTION);
	void rec(int s, int t, int l, int r, int id, int x, int v) {
		// lazy propagation process of v
		if (s == l && t == r) {
			// leaf process
		}
		else {
			int m = (l + r) / 2;
			int id_l = left_of(id);
			int id_r = right_of(id);
			if (s < m && m < t) {
				rec(s, m, l, m, id_l, x, v);
				rec(m, t, m, r, id_r, x, v);
			}
			else if (s < m) {
				rec(s, t, l, m, id_l, x, v);
				// opposite (right side) process
			}
			else if (m < t) {
				// opposite (left side) process
				rec(s, t, m, r, id_r, x, v);
			}
			// merge process
		}
	}
public:
	SegTree(int n, T init) {
		this->n = n;
		N = (int)pow(2, ceil(log2(n)));
		base = N - 1;
		nodes = vector<T>(base + N, init);
	}
	int cover_size(int id) {
		int cnt = 1;
		while (left_of(id) != -1) {
			id = left_of(id);
			cnt *= 2;
		}
		int l = id - base;
		int r = min(l + cnt, n);
		return max(0, r - l);
	}
};