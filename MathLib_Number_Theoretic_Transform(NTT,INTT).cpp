namespace mod_op {

	ll MOD = 924844033;

	class modll {
	private:
		ll val;
		inline ll modify(ll x) { ll ret = x % MOD; if (ret < 0) ret += MOD; return ret; }
		inline ll inv(ll x) {
			if (x == 0) return 1 / x;
			else if (x == 1) return 1;
			else return modify(inv(MOD % x) * modify(-MOD / x));
		}
	public:
		modll(ll init = 0) { val = modify(init); return; }
		modll(const modll& another) { val = another.val; return; }
		inline modll& operator=(const modll &another) { val = another.val; return *this; }
		inline modll operator+(const modll &x) { return modify(val + x.val); }
		inline modll operator-(const modll &x) { return modify(val - x.val); }
		inline modll operator*(const modll &x) { return modify(val * x.val); }
		inline modll operator/(const modll &x) { return modify(val * inv(x.val)); }
		inline modll& operator+=(const modll &x) { val = modify(val + x.val); return *this; }
		inline modll& operator-=(const modll &x) { val = modify(val - x.val); return *this; }
		inline modll& operator*=(const modll &x) { val = modify(val * x.val); return *this; }
		inline modll& operator/=(const modll &x) { val = modify(val * inv(x.val)); return *this; }
		inline bool operator==(const modll &x) { return val == x.val; }
		inline bool operator!=(const modll &x) { return val != x.val; }
		friend inline istream& operator >> (istream &is, modll& x) { is >> x.val; return is; }
		friend inline ostream& operator << (ostream &os, const modll& x) { os << x.val; return os; }
		ll get_val() { return val; }
	};

	modll pow(modll n, ll p) {
		modll ret;
		if (p == 0) ret = 1;
		else if (p == 1) ret = n;
		else {
			ret = pow(n, p / 2);
			ret *= ret;
			if (p % 2 == 1) ret *= n;
		}
		return ret;
	}

	vector<modll> facts;

	inline void make_facts(int n) {
		if (facts.empty()) facts.push_back(modll(1));
		for (int i = (int)facts.size(); i <= n; ++i) facts.push_back(modll(facts.back() * (ll)i));
		return;
	}

	vector<modll> ifacts;
	vector<modll> invs;

	inline void make_invs(int n) {
		if (invs.empty()) {
			invs.push_back(modll(0));
			invs.push_back(modll(1));
		}
		for (int i = (int)invs.size(); i <= n; ++i) {
			// because 0 = MOD = kq + r, 1/k = -q/r
			invs.push_back(invs[(int)MOD % i] * ((int)MOD - (int)MOD / i));
		}
		return;
	}

	inline void make_ifacts(int n) {
		make_invs(n);
		if (ifacts.empty()) ifacts.push_back(modll(1));
		for (int i = (int)ifacts.size(); i <= n; ++i) ifacts.push_back(modll(ifacts.back() * invs[i]));
		return;
	}

	//nCr
	modll combination(ll n, ll r) {
		if (n >= r && r >= 0) {
			modll ret;
			make_facts((int)n);
			make_ifacts((int)n);
			ret = facts[(unsigned)n] * ifacts[(unsigned)r] * ifacts[(unsigned)(n - r)];
			return ret;
		}
		else return 0;
	}

	modll get_fact(ll n) {
		make_facts((int)n);
		return facts[(int)n];
	}

	modll get_ifact(ll n) {
		make_ifacts((int)n);
		return ifacts[(int)n];
	}

}

using namespace mod_op;
typedef vector<modll> vmodll;
typedef vector<vector<modll>> vvmodll;

namespace number_theoretic_transform {

	// when MOD - 1 = 2^m * a,
	// min_omega = root^a (try 3,5,7,... to get root)
	// min_omega_depth = m
	// mod_half = (MOD + 1) / 2

	modll min_omega;
	int min_omega_depth;
	modll mod_half;

	void make_base(int mode) {
		switch (mode) {
			/*
		case 0:
			MOD = 167772161;
			min_omega = 17;
			min_omega_depth = 25;
			mod_half = 83886081;
			break;
		case 1:
			MOD = 469762049;
			min_omega = 30;
			min_omega_depth = 26;
			mod_half = 234881025;
			break;
		case 2:
			MOD = 1224736769;
			min_omega = 149;
			min_omega_depth = 24;
			mod_half = 612368385;
			break;
			*/
		default:
			MOD = 924844033;
			min_omega = 3597;
			min_omega_depth = 21;
			mod_half = 462422017;
		}
	}

	vector<modll> omegas, iomegas;

	inline int bit_reverse(int x, int digit) {
		int ret = digit ? x & 1 : 0;
		Loop(i, digit - 1) { ret <<= 1; x >>= 1; ret |= x & 1; }
		return ret;
	}

	inline void make_omegas(int n) {
		if (omegas.size() != n) {
			omegas.resize(n);
			modll omega = pow(min_omega, (1 << min_omega_depth) / n);
			Loop(i, n) {
				if (i == 0) omegas[i] = 1;
				else omegas[i] = omegas[i - 1] * omega;
			}
		}
	}

	inline void make_iomegas(int n) {
		if (iomegas.size() != n) {
			iomegas.resize(n);
			modll iomega = modll(1) / pow(min_omega, (1 << min_omega_depth) / n);
			Loop(i, n) {
				if (i == 0) iomegas[i] = 1;
				else iomegas[i] = iomegas[i - 1] * iomega;
			}
		}
	}

	// a.size() should be 2^digit
	vector<modll> NTT(const vector<modll> a, int mode = 0) {
		int n = int(a.size());
		int digit = int(rndf(log2(n)));
		vector<modll> ret = a;
		make_omegas(n);
		Loop(i, n) {
			int j = bit_reverse(i, digit);
			if (j > i) swap(ret[i], ret[j]);
		}
		Loop(i, digit) {
			int j = 0, m = 1 << i, mw = (digit - i - 1);
			Loop(group_id, n >> (i + 1)) {
				Loop(k, m) {
					modll x = ret[j] + omegas[k << mw] * ret[j + m];
					modll y = ret[j] - omegas[k << mw] * ret[j + m];
					ret[j] = x; ret[j + m] = y;
					++j;
				}
				j += m;
			}
		}
		return ret;
	}

	// f.size() should be 2^digit
	vector<modll> INTT(const vector<modll>& f, int mode = 0) {
		int n = int(f.size());
		int digit = int(rndf(log2(n)));
		vector<modll> ret = f;
		make_iomegas(n);
		Loopr(i, digit) {
			int j = 0, m = 1 << i, mw = (digit - i - 1);
			Loop(group_id, n >> (i + 1)) {
				Loop(k, m) {
					modll q = (ret[j] + ret[j + m]) * mod_half;
					modll r = (ret[j] - ret[j + m]) * mod_half * iomegas[k << mw];
					ret[j] = q; ret[j + m] = r;
					++j;
				}
				j += m;
			}
		}
		Loop(i, n) {
			int j = bit_reverse(i, digit);
			if (j > i) swap(ret[i], ret[j]);
		}
		return ret;
	}

	// a.size() = b.size() should be 2^digit
	vector<modll> mul_convolution(const vector<modll> &a, const vector<modll> &b) {
		int n = int(a.size());
		vector<modll> ret;
		make_base(0);
		// Garner's algorithm is unsupported yet 
		vector<modll> g = NTT(a), h = NTT(b);
		Loop(i, n) g[i] *= h[i];
		ret = INTT(g);
		return ret;
	}

	int legal_size_of(int n) {
		int ret = 1 << (int)log2(n);
		if (ret < n) ret <<= 1;
		return ret;
	}
}

using namespace number_theoretic_transform;