namespace mod_op {

	const ll MOD = ;// (ll)1e9 + 7;

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

	//log_a(b), if x does not exist, return -1
	ll disc_log(modll a, modll b) {
		ll ret = -1;
		ll m = ceilsqrt(MOD);
		unordered_map<ll, ll> mp;
		modll x = 1;
		Loop(i, m) {
			mp[x.get_val()] = i;
			x *= a;
		}
		x = modll(1) / pow(a, m);
		modll k = b;
		Loop(i, m) {
			if (mp.find(k.get_val()) == mp.end()) k *= x;
			else {
				ret = i * m + mp[k.get_val()];
				break;
			}
		}
		return ret;
	}
}

using namespace mod_op;
typedef vector<modll> vmodll;
typedef vector<vector<modll>> vvmodll;