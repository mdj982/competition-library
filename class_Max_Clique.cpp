#include "auto_util_header.hpp"

class Max_Clique {
private:
	static int max_clique_rec(const vvi &mx, unordered_map<ll, int> &mp, ll mask) {
		if (mask != 0 && mp[mask] == 0) {
			ll x = mask & -mask;
			int id = int(log2(x));
			int r0 = max_clique_rec(mx, mp, mask ^ x);
			ll y = 0;
			for (int j = id + 1; j < mx[id].size(); ++j) {
				if (mask & (ll(mx[id][j]) << j)) y |= (1LL << j);
			}
			int r1 = max_clique_rec(mx, mp, y) + 1;
			mp[mask] = max(r0, r1);
		}
		return mp[mask];
	}
public:
	// O(n*2^(n/2))
	static int max_clique(const vvi &mx) {
		int n = int(mx.size());
		unordered_map<ll, int> mp;
		return max_clique_rec(mx, mp, (1LL << n) - 1);
	}
};