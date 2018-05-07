namespace mod_op {

	const ll MOD = (ll)1e9 + 7;

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
		inline modll operator+(const modll &x) { return (val + x.val) % MOD; }
		inline modll operator-(const modll &x) { return (val - x.val) % MOD; }
		inline modll operator*(const modll &x) { return (val * x.val) % MOD; }
		inline modll operator/(const modll &x) { return (val * inv(x.val)) % MOD; }
		inline modll& operator+=(const modll &x) { val = (val + x.val) % MOD; return *this; }
		inline modll& operator-=(const modll &x) { val = (val - x.val) % MOD; return *this; }
		inline modll& operator*=(const modll &x) { val = (val * x.val) % MOD; return *this; }
		inline modll& operator/=(const modll &x) { val = (val * inv(x.val)) % MOD; return *this; }
		inline bool operator==(const modll &x) { return val == x.val; }
		inline bool operator!=(const modll &x) { return val != x.val; }
		friend inline istream& operator >> (istream &is, modll& x) { is >> x.val; return is; }
		friend inline ostream& operator << (ostream &os, modll& x) { os << x.val; return os; }
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

	vector<vector<modll>> Stirling_nums2;
	vector<vector<modll>> Stirling_nums2_sum;

	void make_Stirling_nums2(int n) {
		for (int i = (int)Stirling_nums2.size(); i <= n; ++i) {
			Stirling_nums2.push_back(vector<modll>(i + 1));
			Stirling_nums2_sum.push_back(vector<modll>(i + 1, 0));
			Loop(j, i + 1) {
				if (j == 0) Stirling_nums2[i][j] = 0;
				else if (j == 1) Stirling_nums2[i][j] = 1;
				else if (j == i) Stirling_nums2[i][j] = 1;
				else Stirling_nums2[i][j] = Stirling_nums2[i - 1][j - 1] + Stirling_nums2[i - 1][j] * modll(j);
				if (j > 0) Stirling_nums2_sum[i][j] = Stirling_nums2_sum[i][j - 1] + Stirling_nums2[i][j];
			}
		}
	}

	modll get_Stirling_num2(ll n, ll r) {
		if (n >= r && r >= 0) {
			make_Stirling_nums2((int)n);
			return Stirling_nums2[(int)n][(int)r];
		}
		else return 0;
	}

	modll get_Stirling_num2_sum(ll n, ll r) {
		if (n >= r && r >= 0) {
			make_Stirling_nums2((int)n);
			return Stirling_nums2_sum[(int)n][(int)r];
		}
		else return 0;
	}

	vector<vector<modll>> partition_nums;
	vector<vector<modll>> partition_nums_sum;

	void make_partition_nums(int n) {
		for (int i = (int)partition_nums.size(); i <= n; ++i) {
			partition_nums.push_back(vector<modll>(i + 1));
			partition_nums_sum.push_back(vector<modll>(i + 1, 0));
			Loop(j, i + 1) {
				if (j == 0) partition_nums[i][j] = 0;
				else if (j == 1) partition_nums[i][j] = 1;
				else if (j == i) partition_nums[i][j] = 1;
				else partition_nums[i][j] = partition_nums[i - 1][j - 1] + (i >= j * 2 ? partition_nums[i - j][j] : 0);
				if (j > 0) partition_nums_sum[i][j] = partition_nums_sum[i][j - 1] + partition_nums[i][j];
			}
		}
	}
	modll get_partition_num(ll n, ll r) {
		if (n >= r && n >= 0) {
			make_partition_nums((int)n);
			return partition_nums[(int)n][(int)r];
		}
		else return 0;
	}

	modll get_partition_num_sum(ll n, ll r) {
		if (n >= r && r >= 0) {
			make_partition_nums((int)n);
			return partition_nums_sum[(int)n][(int)r];
		}
		else return 0;
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

// the number of methods of dividing n factors into r groups
modll grouping(ll n, ll r, bool distinct_n, bool distinct_r, bool enable_empty_r) {
	int mode = (distinct_n ? 0b100 : 0) + (distinct_r ? 0b010 : 0) + (enable_empty_r ? 0b001 : 0);
	if (n <= 0 || r <= 0) return 0;
	switch (mode) {
	case 0b000:
		return get_partition_num(n, r);
	case 0b001:
		return get_partition_num_sum(n, r);
	case 0b010:
		return combination(n - 1, r - 1);
	case 0b011:
		return combination(n + r - 1, r - 1);
	case 0b100:
		return get_Stirling_num2(n, r);
	case 0b101:
		return get_Stirling_num2_sum(n, r);
	case 0b110:
		return get_Stirling_num2(n, r) * get_fact(r);
	case 0b111:
		return pow(modll(r), n);
	default:
		return 0;
	}
}