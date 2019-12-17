class Fenwick {
	using val_t = ll;
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

// solve the number of pair(i, j) such that a[i] > a[j] (i < j) 
ll solve_inversion_number(const vll &a) {
	int n = a.size();
	map<ll, int> mp;
	Loop(i, n) mp[a[i]] = 1;
	int cnt = 0;
	Loopitr(itr, mp) itr->second = cnt++;
	vi b(n);
	Loop(i, n) b[i] = mp[a[i]];
	Fenwick fnk(vll(cnt, 0));
	ll ret = 0;
	Loop(i, n) {
		ret += fnk.sumof(b[i] + 1, cnt);
		fnk.add(b[i], 1);
	}
	return ret;
}