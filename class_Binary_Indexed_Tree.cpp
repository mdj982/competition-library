class Binary_Indexed_Tree {
private:
	vll bits;
	int n;
public:
	Binary_Indexed_Tree(vll a) {
		n = a.size();
		bits = vll(n, 0);
		Loop(i, a.size()) add(i, a[i]);
	}
	void add(int k, ll x) {
		++k;
		for (int index = k; index <= n; index += index & -index) {
			bits[index - 1] += x;
		}
	}
	// note: sum of [s, t)
	ll sum(int s, int t) {
		ll ret = 0;
		for (int index = t; index > 0; index -= index & -index) {
			ret += bits[index - 1];
		}
		for (int index = s; index > 0; index -= index & -index) {
			ret -= bits[index - 1];
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
	Binary_Indexed_Tree bit(vll(cnt, 0));
	ll ret = 0;
	Loop(i, n) {
		ret += bit.sum(b[i] + 1, cnt);
		bit.add(b[i], 1);
	}
	return ret;
}