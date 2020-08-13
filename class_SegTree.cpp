#include "auto_util_header.hpp"

class SegTree {
	using val_t = ll;
private:
	struct segval_t {
		bool enable;
		val_t upd, add, min, max, sum;
	};
	int n, N; // n is the original size, while N is the extended size
	int base;
	vector<segval_t> nodes;
	vi idl, idr, cover_size;
	void merge(int id) {
		nodes[id].min = min(nodes[idl[id]].min + nodes[idl[id]].add,
			nodes[idr[id]].min + nodes[idr[id]].add);
		nodes[id].max = max(nodes[idl[id]].max + nodes[idl[id]].add,
			nodes[idr[id]].max + nodes[idr[id]].add);
		nodes[id].sum = nodes[idl[id]].sum + nodes[idl[id]].add * cover_size[idl[id]]
			+ nodes[idr[id]].sum + nodes[idr[id]].add * cover_size[idr[id]];
	}
	void lazy(int id) {
		if (id >= base) return;
		if (nodes[id].enable) {
			val_t upd = nodes[id].upd + nodes[id].add;
			nodes[idl[id]] = { true, upd, 0, upd, upd, upd * cover_size[idl[id]] };
			nodes[idr[id]] = { true, upd, 0, upd, upd, upd * cover_size[idr[id]] };
			nodes[id] = { false, 0, 0, upd, upd, upd * cover_size[id] };
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
			if (op == UPD) nodes[id] = { true, x, 0, x, x, x * cover_size[id] };
			else if (op == ADD) nodes[id].add += x;
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
	enum solve_t {
		MIN, MAX, SUM
	};
	val_t solve_rec(int s, int t, int l, int r, int id, solve_t op) {
		val_t v = 0;
		if (s == l && t == r) {
			if (op == MIN) v = nodes[id].min;
			else if (op == MAX) v = nodes[id].max;
			else if (op == SUM) v = nodes[id].sum;
		}
		else {
			lazy(id);
			int m = (l + r) >> 1;
			if (s < m && m < t) {
				val_t v0 = solve_rec(s, m, l, m, idl[id], op);
				val_t v1 = solve_rec(m, t, m, r, idr[id], op);
				if (op == MIN) v = min(v0, v1);
				else if (op == MAX) v = max(v0, v1);
				else if (op == SUM) v = v0 + v1;
			}
			else if (s < m) {
				v = solve_rec(s, t, l, m, idl[id], op);
			}
			else if (m < t) {
				v = solve_rec(s, t, m, r, idr[id], op);
			}
		}
		if (op == MIN) v += nodes[id].add;
		else if (op == MAX) v += nodes[id].add;
		else if (op == SUM) v += nodes[id].add * (t - s);
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
	SegTree(int n, val_t init = 0) {
		this->n = n;
		N = 1 << ceillog2(n);
		base = N - 1;
		nodes = vector<segval_t>(base + N, { false, 0, 0, LLONG_MAX, LLONG_MIN, 0 });
		common_init();
		upd(0, n, init);
	}
	SegTree(const vector<val_t> &a) {
		this->n = int(a.size());
		N = 1 << ceillog2(n);
		base = N - 1;
		nodes = vector<segval_t>(base + N, { false, 0, 0, LLONG_MAX, LLONG_MIN, 0 });
		common_init();
		Loop(i, n) {
			nodes[base + i] = { true, a[i], 0, a[i], a[i], a[i] };
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
	val_t minof(int s, int t) {
		if (s >= t) return LLONG_MAX;
		return solve_rec(s, t, 0, N, 0, MIN);
	}
	val_t maxof(int s, int t) {
		if (s >= t) return LLONG_MIN;
		return solve_rec(s, t, 0, N, 0, MAX);
	}
	val_t sumof(int s, int t) {
		if (s >= t) return 0;
		return solve_rec(s, t, 0, N, 0, SUM);
	}
};