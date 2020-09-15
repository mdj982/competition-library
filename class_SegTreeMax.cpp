#include "auto_util_header.hpp"

#define ENABLE_UPD
class SegTreeMax {
	using val_t = ll;
	static const val_t VALMIN = LLONG_MIN;
	static const int IDXMAX = INT_MAX;
private:
	struct idxval_t {
		int idx;
		val_t val;
		bool operator<(const idxval_t &another) const {
			return val != another.val ? val < another.val : idx > another.idx;
		}
		idxval_t operator+(const val_t x) {
			return {this->idx, this->val + x};
		}
		idxval_t* operator+=(const val_t x) {
			this->val += x;
			return this;
		}
	};
	struct segval_t {
		bool enable;
		val_t upd, add;
		idxval_t max;
	};
	int n, N; // n is the original size, while N is the extended size
	int base;
	vector<segval_t> nodes;
	vi idl, idr;
	void merge(int id) {
		nodes[id].max = max(nodes[idl[id]].max + nodes[idl[id]].add,
			nodes[idr[id]].max + nodes[idr[id]].add);
	}
#ifdef ENABLE_UPD
	void lazy(int id, int l, int r) {
		if (id >= base) return;
		if (nodes[id].enable) {
			val_t upd = nodes[id].upd + nodes[id].add;
			nodes[idl[id]] = { true, upd, 0, {l, upd} };
			nodes[idr[id]] = { true, upd, 0, {(l + r) >> 1, upd} };
			nodes[id] = { false, 0, 0, {l, upd} };
		}
		else {
			nodes[idl[id]].add += nodes[id].add;
			nodes[idr[id]].add += nodes[id].add;
			nodes[id].add = 0;
			merge(id);
		}
	}
#endif
	enum change_t {
		UPD, ADD
	};
	void change_rec(int s, int t, int l, int r, int id, val_t x, change_t op) {
		if (s == l && t == r) {
			if (op == UPD) nodes[id] = { true, x, 0, {s, x} };
			else if (op == ADD) nodes[id].add += x;
		}
		else {
#ifdef ENABLE_UPD
			lazy(id, l, r);
#endif
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
	idxval_t solve_rec(int s, int t, int l, int r, int id) {
		idxval_t v;
		if (s == l && t == r) {
			v = nodes[id].max;
		}
		else {
#ifdef ENABLE_UPD
			lazy(id, l, r);
#endif
			int m = (l + r) >> 1;
			if (s < m && m < t) {
				idxval_t v0 = solve_rec(s, m, l, m, idl[id]);
				idxval_t v1 = solve_rec(m, t, m, r, idr[id]);
				v = std::max(v0, v1);
			}
			else if (s < m) {
				v = solve_rec(s, t, l, m, idl[id]);
			}
			else if (m < t) {
				v = solve_rec(s, t, m, r, idr[id]);
			}
		}
		v += nodes[id].add;
		return v;
	}
	void common_init() {
		idl.resize(base + N, -1);
		idr.resize(base + N, -1);
		Loop(i, base) {
			idl[i] = (i << 1) + 1;
			idr[i] = (i << 1) + 2;
		}
	}
public:
	SegTreeMax(int n, val_t init = LLONG_MIN) {
		this->n = n;
		this->N = 1 << ceillog2(n);
		this->base = N - 1;
		this->nodes = vector<segval_t>(base + N, { false, 0, 0, {IDXMAX, VALMIN} });
		common_init();
		Loop(i, n) {
			this->nodes[base + i] = { true, init, 0, {i, init} };
		}
		Loopr(i, base) {
			merge(i);
		}
	}
	SegTreeMax(const vector<val_t> &a) {
		this->n = int(a.size());
		this->N = 1 << ceillog2(n);
		this->base = N - 1;
		this->nodes = vector<segval_t>(base + N, { false, 0, 0, {IDXMAX, VALMIN} });
		common_init();
		Loop(i, n) {
			nodes[base + i] = { true, a[i], 0, {i, a[i]} };
		}
		Loopr(i, base) {
			merge(i);
		}
	}
#ifdef ENABLE_UPD
	void upd(int s, int t, val_t x) {
		if (s >= t) return;
		change_rec(s, t, 0, N, 0, x, UPD);
	}
#endif
	void add(int s, int t, val_t x) {
		if (s >= t) return;
		change_rec(s, t, 0, N, 0, x, ADD);
	}
	// the smallest argmax_i\in[s, t) a[i]
	int maxidx(int s, int t) {
		if (s >= t) return IDXMAX;
		return solve_rec(s, t, 0, N, 0).idx;
	}
	val_t maxval(int s, int t) {
		if (s >= t) return VALMIN;
		return solve_rec(s, t, 0, N, 0).val;
	}
	// the smallest argmax_i\in[s, t) a[i], and its value
	idxval_t maxidxval(int s, int t) {
		if (s >= t) return {IDXMAX, VALMIN};
		return solve_rec(s, t, 0, N, 0);
	}
};
#undef ENABLE_UPD