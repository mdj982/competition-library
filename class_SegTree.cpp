template <class seg_t>
class SegTree {
private:
	void local_update(int index, seg_t(*update_rule)(seg_t, seg_t, seg_t)) {
		int l = left_of(index), r = index * 2 + 2;
		segs[index] = update_rule(segs[index], segs[l], segs[r]);
	}
	void get_index_of_range_rec(int s, int t, int l, int r, int index, vi *v) {
		if (s == l && t == r) v->push_back(index);
		else {
			int mid = (l + r) / 2;
			if (s < mid && mid < t) {
				get_index_of_range_rec(s, mid, l, mid, left_of(index), v);
				get_index_of_range_rec(mid, t, mid, r, right_of(index), v);
			}
			else if (s < mid) {
				get_index_of_range_rec(s, t, l, mid, left_of(index), v);
			}
			else if (mid < t) {
				get_index_of_range_rec(s, t, mid, r, right_of(index), v);
			}
		}
	}
public:
	int n;
	vector<seg_t> segs;
	enum direction_t { NONE, LEFT, RIGHT };
	vector<direction_t> froms;
	SegTree(vector<seg_t> a, seg_t init, seg_t(*update_rule)(seg_t, seg_t, seg_t)) {
		n = (int)pow(2, ceil(log2(a.size())));
		segs = vector<seg_t>(n * 2 - 1, init);
		froms = vector<direction_t>(n * 2 - 1, NONE);
		Loop(i, a.size()) segs[n - 1 + i] = a[i];
		Loopr(i, n - 1) local_update(i, update_rule);
	}
	int left_of(int index) {
		int ret = index * 2 + 1;
		if (ret < n * 2 - 1) return ret;
		else return -1;
	}
	int right_of(int index) {
		int ret = index * 2 + 2;
		if (ret < n * 2 - 1) return ret;
		else return -1;
	}
	void update(int k, seg_t x, seg_t(*update_rule)(seg_t, seg_t, seg_t), bool by_index) {
		vi indices = get_index_above(k, by_index);
		segs[indices[0]] = x;
		Loop1(i, indices.size() - 1) local_update(indices[i], update_rule);
	}
	// note: range [s, t)
	vi get_index_of_range(int s, int t) {
		vi ret = {};
		get_index_of_range_rec(s, t, 0, n, 0, &ret);
		return ret;
	}
	// note: indices.size() should not be 0, indices should not be intersected
	seg_t independent_combine(vi indices, seg_t(*update_rule)(seg_t, seg_t, seg_t)) {
		seg_t ret = segs[indices.back()];
		Loopr(i, indices.size() - 1) {
			ret = update_rule(segs[indices[i]], segs[indices[i]], ret);
		}
		return ret;
	}
	vi get_index_above(int k, bool by_index) {
		int index = by_index ? k : k + n - 1;
		vi ret = { index };
		while (index > 0) {
			index = (index - 1) / 2;
			ret.push_back(index);
		}
		return ret;
	}
	vi get_index_under(int k, bool by_index) {
		int index = by_index ? k : k + n - 1;
		vi ret = { index };
		while (index < n * 2 - 1) {
			if (froms[index] == NONE) break;
			index = (froms[index] == LEFT ? left_of(index) : right_of(index));
			ret.push_back(index);
		}
		return ret;
	}
	seg_t get_val(int k, bool by_index = false) {
		int index = by_index ? k : k + n - 1;
		return segs[index];
	}
	/*
	void debug_output() {
		int index = 0;
		int cnt = 0;
		while (index < n * 2 - 1) {
			Loop(i, 1 << cnt) {
				cout << segs[index] << " ";
				index++;
			}
			cout << endl;
			cnt++;
		}
	}
	*/
};