#include "auto_util_header.hpp"

class SegTreeSum {
	using val_t = ll;
private:
	struct segval_t {
		bool enable;
		val_t upd, add, sum;
	};
	int n, N; // n is the original size, while N is the extended size
	int base;
	vector<segval_t> nodes;
	vi idl, idr, cover_size;
	void merge(int id) {
		nodes[id].sum = nodes[idl[id]].sum + nodes[idl[id]].add * cover_size[idl[id]]
			+ nodes[idr[id]].sum + nodes[idr[id]].add * cover_size[idr[id]];
	}
	void lazy(int id) {
		if (id >= base) return;
		if (nodes[id].enable) {
			val_t upd = nodes[id].upd + nodes[id].add;
			nodes[idl[id]] = { true, upd, 0, upd * cover_size[idl[id]] };
			nodes[idr[id]] = { true, upd, 0, upd * cover_size[idr[id]] };
			nodes[id] = { false, 0, 0, upd * cover_size[id] };
		}
		else {
			nodes[idl[id]].add += nodes[id].add;
			nodes[idr[id]].add += nodes[id].add;
			nodes[id].add = 0;
			merge(id);
		}
	}
	enum change_t {
		UPD, ADD
	};
	void change_rec(int s, int t, int l, int r, int id, val_t x, change_t op) {
		if (s == l && t == r) {
			if (op == UPD) nodes[id] = { true, x, 0, x * cover_size[id] };
			if (op == ADD) nodes[id].add += x;
		}
		else {
			lazy(id);
			int m = (l + r) >> 1;
			if (s < m && m < t) {
				change_rec(s, m, l, m, idl[id], x, op);
				change_rec(m, t, m, r, idr[id], x, op);
			}
			else if (s < m) {
				change_rec(s, t, l, m, idl[id], x, op);
			}
			else if (m < t) {
				change_rec(s, t, m, r, idr[id], x, op);
			}
			merge(id);
		}
	}
	val_t solve_rec(int s, int t, int l, int r, int id) {
		val_t v = 0;
		if (s == l && t == r) {
			v = nodes[id].sum;
		}
		else {
			lazy(id);
			int m = (l + r) >> 1;
			if (s < m && m < t) {
				val_t v0 = solve_rec(s, m, l, m, idl[id]);
				val_t v1 = solve_rec(m, t, m, r, idr[id]);
				v = v0 + v1;
			}
			else if (s < m) {
				v = solve_rec(s, t, l, m, idl[id]);
			}
			else if (m < t) {
				v = solve_rec(s, t, m, r, idr[id]);
			}
		}
		v += nodes[id].add * (t - s);
		return v;
	}
	void common_init() {
		idl.resize(base + N, -1);
		idr.resize(base + N, -1);
		Loop(i, base) {
			idl[i] = (i << 1) + 1;
			idr[i] = (i << 1) + 2;
		}
		cover_size.resize(base + N);
		Loop(i, n) {
			cover_size[base + i] = 1;
		}
		Loopr(i, base) {
			cover_size[i] = cover_size[idl[i]] + cover_size[idr[i]];
		}
	}
public:
	SegTreeSum(int n, val_t init = 0) {
		this->n = n;
		N = 1 << ceillog2(n);
		base = N - 1;
		nodes = vector<segval_t>(base + N, { false, 0, 0, 0 });
		common_init();
		Loop(i, n) {
			nodes[base + i] = { true, init, 0, init };
		}
		Loopr(i, base) {
			merge(i);
		}
	}
	SegTreeSum(const vector<val_t> &a) {
		this->n = int(a.size());
		N = 1 << ceillog2(n);
		base = N - 1;
		nodes = vector<segval_t>(base + N, { false, 0, 0, 0 });
		common_init();
		Loop(i, n) {
			nodes[base + i] = { true, a[i], 0, a[i] };
		}
		Loopr(i, base) {
			merge(i);
		}
	}
	void upd(int s, int t, val_t x) {
		if (s >= t) return;
		change_rec(s, t, 0, N, 0, x, UPD);
	}
	void add(int s, int t, val_t x) {
		if (s >= t) return;
		change_rec(s, t, 0, N, 0, x, ADD);
	}
	val_t sumof(int s, int t) {
		if (s >= t) return 0;
		return solve_rec(s, t, 0, N, 0);
	}
};