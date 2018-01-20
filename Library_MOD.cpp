#define MOD (ll)(1e9+7)

//solve x, y s.t. mx+ny=gcd(m,n)
class Extended_Euclid {
private:
	ll m, n;
	vvll mx_multiply(vvll mx1, vvll mx2) {
		vvll ret(mx1.size(), vll(mx2[0].size(), 0));
		Loop(i, mx1.size()) {
			Loop(j, mx1[0].size()) {
				Loop(k, mx2[0].size()) {
					ret[i][j] += mx1[i][k] * mx2[k][j];
				}
			}
		}
		return ret;
	}
public:
	ll x, y, gcd;
	Extended_Euclid(ll M, ll N) {
		m = M;
		n = N;
		vll r(100), k(100);
		bool swapflag = false, m_negflag = false, n_negflag = false;
		if (m < n) { swap(m, n); swapflag = true; }
		if (m < 0) { m *= -1; m_negflag = true; }
		if (n < 0) { n *= -1; n_negflag = true; }
		r[0] = m;
		r[1] = n;
		int h = 1;
		while (1) {
			k[h - 1] = r[h - 1] / r[h];
			r[h + 1] = r[h - 1] % r[h];
			if (r[h + 1] == 0) break;
			h++;
		}
		gcd = r[h];
		vvll mx1 = { { 0, 1 },{ 1, (-1) * k[h - 1] } };
		Loopr(i, h - 1) {
			vvll mx2 = { { 0, 1 },{ 1, (-1) * k[i] } };
			mx1 = mx_multiply(mx1, mx2);
		}
		x = mx1[0][0];
		y = mx1[0][1];
		if (n_negflag) { n *= -1; y *= -1; }
		if (m_negflag) { m *= -1; x *= -1; }
		if (swapflag) { swap(m, n); swap(x, y); }
	}
};

ll mod_mul(ll x, ll y) {
	return x * y % MOD;
}

//A[k] = k!
vll mod_factlist(ll n) {
	if (n < 0) return{};
	else {
		vll ret((int)n + 1);
		ret[0] = 1;
		Loop1(i, (int)n) ret[i] = mod_mul(ret[i - 1], i);
		return ret;
	}
}

//n^p O(log p)
ll mod_pow(ll n, ll p) {
  if (p == 0) return 1;
	else if (p == 1) return n;
	else {
		ll ans = mod_pow(n, p / 2);
		ans = mod_mul(ans, ans);
		if (p % 2 == 1) ans = mod_mul(ans, n);
		return ans;
	}
}

//0 ~ mod - 1
ll mod_modify(ll n) {
	ll ret;
	if (n > 0) ret = n - n / MOD * MOD;
	else ret = n - (n - MOD + 1) / MOD * MOD;
	return ret;
}

//x^-1
ll mod_inv(ll n) {
	Extended_Euclid ee(n, MOD*(-1));
	return mod_modify(ee.x);
}

//nCr
ll mod_combination(ll n, ll r) {
	vll facts = mod_factlist(n);
	return mod_mul(facts[n], mod_inv(mod_mul(facts[r], facts[n - r])));
}