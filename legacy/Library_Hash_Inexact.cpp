using hash_t = unsigned long long;

// ret[i] = encode(s[0] ... s[i])
vector<hash_t> prefix_encode(const string &s) {
	int n = s.length();
	const hash_t mod = ll(1e9) + 7;
	vector<hash_t> ret(n);
	Loop(i, n) {
		if (i == 0) ret[i] = s[i];
		else ret[i] = ret[i - 1] * mod + s[i];
	}
	return ret;
}

// ret[i] = encode(s[0] ... s[i])
vector<hash_t> suffix_encode(const string &s) {
	int n = s.length();
	const hash_t mod = ll(1e9) + 7;
	vector<hash_t> ret(n);
	hash_t x = 1;
	Loopr(i, n) {
		if (i == n - 1) ret[i] = s[i];
		else ret[i] = s[i] * x + ret[i + 1];
		x *= mod;
	}
	return ret;
}

// ret[i] = encode(s[i] ... s[i + l - 1])
vector<hash_t> rolling_encode(const string &s, int l) {
	int n = s.length();
	const hash_t mod = ll(1e9) + 7;
	vector<hash_t> ret(max(0, n - l + 1));
	hash_t x = 1;
	Loop(i, l - 1) x *= mod;
	if (ret.size() > 0) {
		Loop(i, l) {
			ret[0] *= mod;
			ret[0] += s[i];
		}
		Loop1(i, ret.size() - 1) {
			ret[i] = (ret[i - 1] - s[i - 1] * x) * mod + s[l + i - 1];
		}
	}
	return ret;
}