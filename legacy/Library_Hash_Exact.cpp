vll rolling_encode(const string &s, int l, char base, int kind) {
	int n = s.length();
	vll ret(max(0, n - l + 1));
	ll x = 1;
	Loop(i, l - 1) x *= kind;
	if (ret.size() > 0) {
		Loop(i, l) {
			ret[0] *= kind;
			ret[0] += s[i] - base;
		}
		Loop1(i, ret.size() - 1) {
			ret[i] = (ret[i - 1] - (s[i - 1] - base) * x) * kind + (s[l + i - 1] - base);
		}
	}
	return ret;
}

string decode(ll x, int l, char base, int kind) {
	vector<char> a(l, base);
	Loopr(i, l) {
		a[i] += x % kind;
		x /= kind;
	}
	string ret = "";
	Loop(i, l) ret += a[i];
	return ret;
}