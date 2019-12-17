class Fenwick {
	using val_t = int;
private:
	vector<val_t> nodes;
	int n;
public:
	Fenwick(int n) {
		this->n = n;
		nodes = vector<val_t>(n, 0);
	}
	Fenwick(const vector<val_t> &a) {
		this->n = a.size();
		nodes = vector<val_t>(n, 0);
		Loop(i, a.size()) add(i, a[i]);
	}
	void add(int k, val_t x) {
		++k;
		for (int id = k; id <= n; id += id & -id) {
			nodes[id - 1] += x;
		}
	}
	// note: sum of [s, t)
	val_t sumof(int s, int t) {
		val_t ret = 0;
		for (int id = t; id > 0; id -= id & -id) {
			ret += nodes[id - 1];
		}
		for (int id = s; id > 0; id -= id & -id) {
			ret -= nodes[id - 1];
		}
		return ret;
	}
};

class Fenwick2D {
	using val_t = int;
private:
	struct segval_t {
		int n = 0;
		map<int, int> encoder;
		Fenwick* st;
	};
	vector<segval_t> nodes;
	int n, N; // n is the original size, while N is the extended size
	int base;
	vi idl, idr;
	// [l, r), in bottom-up order
	void set_ids_in_range(int l, int r, vi &end_ids, vi &mid_ids) {
		end_ids.clear();
		mid_ids.clear();
		if (l >= r) return;
		r--;
		int id_top = N;
		while (id_top && ((l & id_top) == (r & id_top))) id_top >>= 1;
		int id_r = base + r;
		int id_l = base + l;
		int mode_l = 0;
		int mode_r = 1;
		for (int k = 1; k < id_top; k <<= 1) {
			int r0 = r & 1;
			int l0 = l & 1;
			if (r0 ^ mode_r) {
				int t = id_r - r0;
				end_ids.push_back(t);
				mode_r &= r0;
			}
			if (l0 ^ mode_l) {
				int t = id_l + (l0 ^ 1);
				end_ids.push_back(t);
				mode_l |= l0;
			}
			r >>= 1;
			l >>= 1;
			(--id_r) >>= 1;
			(--id_l) >>= 1;
			if (mode_r ^ 1) mid_ids.push_back(id_r);
			if (mode_l ^ 0)	mid_ids.push_back(id_l);
		}
		int id;
		if ((mode_l ^ 1) & mode_r) {
			if (id_top) end_ids.push_back(id = (--id_l) >> 1);
			else end_ids.push_back(id = id_l);
		}
		else if (mode_r ^ 0) end_ids.push_back(id = id_r);
		else if (mode_l ^ 1) end_ids.push_back(id = id_l);
		else id = id_l;
		while (id) {
			(--id) >>= 1;
			mid_ids.push_back(id);
		}
	}
	void add_core(const vi &end_ids, const vi &mid_ids, int s1, val_t x) {
		Foreach(id, end_ids) {
			nodes[id].st->add(nodes[id].encoder.lower_bound(s1)->snd, x);
		}
		Foreach(id, mid_ids) {
			nodes[id].st->add(nodes[id].encoder.lower_bound(s1)->snd, x);
		}
	}
	val_t sum_core(const vi &end_ids, const vi &mid_ids, int s1, int t1) {
		val_t ret = 0;
		Foreach(id, end_ids) {
			ret += nodes[id].st->sumof(nodes[id].encoder.lower_bound(s1)->snd, nodes[id].encoder.lower_bound(t1)->snd);
		}
		return ret;
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
	// efficient only if ids indicates points or small-constant-row horizontal rectangles
	Fenwick2D(int n, const vector<P> &id_pairs) {
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
			nodes[i].st = new Fenwick(nodes[i].n);
		}
	}
	void add(int s0, int s1, val_t x) {
		vi end_ids;
		vi mid_ids;
		set_ids_in_range(s0, s0 + 1, end_ids, mid_ids);
		add_core(end_ids, mid_ids, s1, x);
	}
	val_t sumof(int s0, int t0, int s1, int t1) {
		if (t0 - s0 <= 0 || t1 - s1 <= 0) return 0;
		vi end_ids;
		vi mid_ids;
		set_ids_in_range(s0, t0, end_ids, mid_ids);
		return sum_core(end_ids, mid_ids, s1, t1);
	}
};