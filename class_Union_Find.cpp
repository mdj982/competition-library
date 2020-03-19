class Union_Find {
private:
	vi p, r; // parent, rank
	vll w; // weight
	ll(*unite_rule)(ll, ll);
	// w is the size of connected component in default
	static ll default_unite_rule(ll u, ll v) {
		return u + v;
	}
public:
	Union_Find(int N, const vll w = {}, ll(*unite_rule)(ll, ll) = nullptr) {
		this->p.resize(N);
		this->r.resize(N);
		this->w.resize(N);
		Loop(i, N) {
			this->p[i] = i;
			this->r[i] = 0;
			this->w[i] = w.size() ? w[i] : 1;
		}
		this->unite_rule = (unite_rule != nullptr ? unite_rule : default_unite_rule);
	}
	int find(int x) {
		if (p[x] == x) return x;
		else return p[x] = find(p[x]);
	}
	void unite(int x, int y) {
		x = find(x);
		y = find(y);
		if (x == y) return;
		if (r[x] == r[y]) r[x]++;
		if (r[x] < r[y]) {
			p[x] = y;
			w[y] = unite_rule(w[x], w[y]);
		}
		else {
			p[y] = x;
			w[x] = unite_rule(w[x], w[y]);
		}
	}
	bool is_same(int x, int y) {
		return find(x) == find(y);
	}
	int get_w(int x) {
		return w[find(x)];
	}
	void change_w(int x, ll v) {
		w[find(x)] = v;
	}
};

/*
ll unite_rule_min(ll u, ll v) {
	return min(u, v);
}
*/

