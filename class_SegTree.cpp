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
	void local_update(int index, T(*update_rule)(T, T, T)) {
		int l = left_of(index), r = right_of(index);
		nodes[index] = update_rule(nodes[index], nodes[l], nodes[r]);
	}
	// initially, (s, t, 0, N, 0, ...);
	void get_index_of_range_rec(int s, int t, int l, int r, int index, vi &v) {
		if (s == l && t == r) {
			// leaf process
		}
		else {
			int mid = (l + r) / 2;
			int index_l = left_of(index), index_r = right_of(index);
			if (s < mid && mid < t) {
				get_index_of_range_rec(s, mid, l, mid, index_l, v);
				get_index_of_range_rec(mid, t, mid, r, index_r, v);
			}
			else if (s < mid) {
				get_index_of_range_rec(s, t, l, mid, index_l, v);
				// opposite process
			}
			else if (mid < t) {
				get_index_of_range_rec(s, t, mid, r, index_r, v);
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
	// use either subst or update
	void subst(int index, T x, bool from_base = false) {
		if (from_base) index += base;
		nodes[index] = x;
	}
	// use either subst or update
	void update(int index, T x, T(*update_rule)(T, T, T), bool from_base = false) {
		if (from_base) index += base;
		nodes[index] = x;
		while (index > 0) {
			index = parent_of(index);
			local_update(index, update_rule);
		}
	}
	// note: range [s, t)
	vi get_index_of_range(int s, int t) {
		vi ret;
		get_index_of_range_rec(s, t, 0, N, 0, ret);
		return ret;
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
	// get values
	T get(int index, bool from_base = false) {
		if (from_base) index += base;
		return nodes[index];
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