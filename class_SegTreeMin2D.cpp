class SegTreeMin2D {
	using val_t = ll;
private:
	struct segval_t {
		int n = 0;
		map<int, int> encoder;
		SegTreeMin* st;
	};
	vector<segval_t> nodes;
	int n, N; // n is the original size, while N is the extended size
	int base;
	vi idl, idr;
	enum change_t {
		UPD, ADD
	};
	void change_rec(int s0, int t0, int s1, int t1, int l, int r, int id, val_t x, change_t op) {
		if (op == UPD) nodes[id].st->upd(nodes[id].encoder.lower_bound(s1)->snd, nodes[id].encoder.lower_bound(t1)->snd, x);
		if (op == ADD) nodes[id].st->add(nodes[id].encoder.lower_bound(s1)->snd, nodes[id].encoder.lower_bound(t1)->snd, x);
		if (s0 == l && t0 == r) {
			return;
		}
		else {
			int m = (l + r) >> 1;
			if (s0 < m && m < t0) {
				change_rec(s0, m, s1, t1, l, m, idl[id], x, op);
				change_rec(m, t0, s1, t1, m, r, idr[id], x, op);
			}
			else if (s0 < m) {
				change_rec(s0, t0, s1, t1, l, m, idl[id], x, op);
			}
			else if (m < t0) {
				change_rec(s0, t0, s1, t1, m, r, idr[id], x, op);
			}
			return;
		}
	}
	val_t solve_rec(int s0, int t0, int s1, int t1, int l, int r, int id) {
		val_t v = 0;
		if (s0 == l && t0 == r) {
			v = nodes[id].st->minof(nodes[id].encoder.lower_bound(s1)->snd, nodes[id].encoder.lower_bound(t1)->snd);
		}
		else {
			int m = (l + r) >> 1;
			if (s0 < m && m < t0) {
				val_t v0 = solve_rec(s0, m, s1, t1, l, m, idl[id]);
				val_t v1 = solve_rec(m, t0, s1, t1, m, r, idr[id]);
				v = min(v0, v1);
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
	// n: the row size 
	// efficient only if ids indicate points or small-constant-row horizontal rectangles
	SegTreeMin2D(int n, const vector<P> &id_pairs) {
		this->n = n;
		N = 1 << ceillog2(n);
		base = N - 1;
		common_init();
		nodes.resize(base + N);
		set<P> id_pair_set;
		Foreach(id_pair, id_pairs) {
			id_pair_set.insert(id_pair);
		}
		Foreach(id_pair, id_pair_set) {
			nodes[base + id_pair.fst].encoder[id_pair.snd] = nodes[base + id_pair.fst].n++;
		}
		Loopr(i, base) {
			set<int> ids;
			Foreach(itr, nodes[idl[i]].encoder) {
				ids.insert(itr.fst);
			}
			Foreach(itr, nodes[idr[i]].encoder) {
				ids.insert(itr.fst);
			}
			Foreach(id, ids) {
				nodes[i].encoder[id] = nodes[i].n++;
			}
		}
		Loop(i, base + N) {
			nodes[i].encoder[INT_MIN] = -1;
			nodes[i].encoder[INT_MAX] = nodes[i].n;
			nodes[i].st = new SegTreeMin(nodes[i].n);
		}
	}
	void upd(int s0, int t0, int s1, int t1, val_t x) {
		if (t0 - s0 <= 0 || t1 - s1 <= 0) return;
		change_rec(s0, t0, s1, t1, 0, N, 0, x, UPD);
	}
	void add(int s0, int t0, int s1, int t1, val_t x) {
		if (t0 - s0 <= 0 || t1 - s1 <= 0) return;
		change_rec(s0, t0, s1, t1, 0, N, 0, x, ADD);
	}
	val_t minof(int s0, int t0, int s1, int t1) {
		if (t0 - s0 <= 0 || t1 - s1 <= 0) return LLONG_MAX;
		else return solve_rec(s0, t0, s1, t1, 0, N, 0);
	}
};