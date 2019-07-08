class BIT {
private:
	vll nodes;
	int n;
public:
	BIT(vll a) {
		n = a.size();
		nodes = vll(n, 0);
		Loop(i, a.size()) add(i, a[i]);
	}
	void add(int k, ll x) {
		++k;
		for (int id = k; id <= n; id += id & -id) {
			nodes[id - 1] += x;
		}
	}
	// note: sum of [s, t)
	ll sum(int s, int t) {
		ll ret = 0;
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
	BIT bit(vll(cnt, 0));
	ll ret = 0;
	Loop(i, n) {
		ret += bit.sum(b[i] + 1, cnt);
		bit.add(b[i], 1);
	}
	return ret;
}