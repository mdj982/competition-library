#include "auto_util_header.hpp"

class SegTreeSum2D {
	using val_t = int;
private:
	struct segval_t {
		vll codes;
		Fenwick* fwk;
	};
	vector<segval_t> nodes;
	int n, N; // n is the original size, while N is the extended size
	int base;
	vi idl, idr;
	enum change_t {
		UPD, ADD
	};
	void change_rec(int s0, int s1, int l, int r, int id, val_t x, change_t op) {
		int s1_code = int(lower_bound(nodes[id].codes.begin(), nodes[id].codes.end(), s0 + s1 * ll(this->N)) - nodes[id].codes.begin());
		if (op == UPD) nodes[id].st->upd(s1_code, s1_code + 1, x);
		if (op == ADD) nodes[id].st->add(s1_code, s1_code + 1, x);
		if (s0 == l && s0 + 1 == r) {
			return;
		}
		else {
			int m = (l + r) >> 1;
			if (s0 < m) {
				change_rec(s0, s1, l, m, idl[id], x, op);
			}
			else {
				change_rec(s0, s1, m, r, idr[id], x, op);
			}
			return;
		}
	}
	val_t solve_rec(int s0, int t0, int s1, int t1, int l, int r, int id) {
		val_t v = 0;
		if (s0 == l && t0 == r) {
			int s1_code = int(lower_bound(nodes[id].codes.begin(), nodes[id].codes.end(), s0 + s1 * ll(this->N)) - nodes[id].codes.begin());
			int t1_code = int(lower_bound(nodes[id].codes.begin(), nodes[id].codes.end(), t0 + (t1 - 1) * ll(this->N)) - nodes[id].codes.begin());
			v = nodes[id].fwk->sumof(s1_code, t1_code);
		}
		else {
			int m = (l + r) >> 1;
			if (s0 < m && m < t0) {
				val_t v0 = solve_rec(s0, m, s1, t1, l, m, idl[id]);
				val_t v1 = solve_rec(m, t0, s1, t1, m, r, idr[id]);
				v = v0 + v1;
			}
			else if (s0 < m) {
				v = solve_rec(s0, t0, s1, t1, l, m, idl[id]);
			}
			else if (m < t0) {
				v = solve_rec(s0, t0, s1, t1, m, r, idr[id]);
			}
		}
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
	// n: the row size, id_pairs: required to be distinct 
	// efficient only if ids indicate points
	SegTreeSum2D(int n, const vector<P> &id_pairs, const vector<val_t> &a) {
		struct pt_t {
			int row_id;
			ll code;
			val_t val;
			bool operator<(const pt_t &another) const {
				return code < another.code;
			}
		};
		this->n = n;
		N = 1 << ceillog2(n);
		base = N - 1;
		common_init();
		nodes.resize(base + N);
		vector<pt_t> pts(id_pairs.size());
		Loop(i, id_pairs.size()) {
			pts[i] = { id_pairs[i].fst, id_pairs[i].fst + id_pairs[i].snd * ll(N), a[i] };
		}
		sort(pts.begin(), pts.end());
		vector<vector<val_t>> as(base + N);
		Foreach(pt, pts) {
			nodes[base + pt.row_id].codes.push_back(pt.code);
			as[base + pt.row_id].push_back(pt.val);
		}
		Loop(i, N) {
			if (nodes[base + i].codes.size() == 0) {
				nodes[base + i].codes.push_back(i);
				as[base + i].push_back(0);
			}
		}
		Loopr(i, base) {
			int pl = 0, pr = 0;
			auto &codes = nodes[i].codes;
			auto &codes_l = nodes[idl[i]].codes;
			auto &codes_r = nodes[idr[i]].codes;
			while (pl < codes_l.size() && pr < codes_r.size()) {
				if (codes_l[pl] <= codes_r[pr]) {
					codes.push_back(codes_l[pl]);
					as[i].push_back(as[idl[i]][pl]);
					pl++;
				}
				else {
					codes.push_back(codes_r[pr]);
					as[i].push_back(as[idr[i]][pr]);
					pr++;
				}
			}
			while (pl < codes_l.size()) {
				codes.push_back(codes_l[pl]);
				as[i].push_back(as[idl[i]][pl]);
				pl++;
			}
			while (pr < codes_r.size()) {
				codes.push_back(codes_r[pr]);
				as[i].push_back(as[idr[i]][pr]);
				pr++;
			}
		}
		Loop(i, base + N) {
			nodes[i].fwk = new Fenwick(as[i]);
		}
	}
	void add(int s0, int s1, val_t x) {
		change_rec(s0, s1, 0, N, 0, x, ADD);
	}
	val_t sumof(int s0, int t0, int s1, int t1) {
		if (t0 - s0 <= 0 || t1 - s1 <= 0) return 0;
		else return solve_rec(s0, t0, s1, t1, 0, N, 0);
	}
};