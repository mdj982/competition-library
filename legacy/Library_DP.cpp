// digit DP

// for [0, x], ret[i] = { the fixed prefix digits, the following number of digits }
vector<pair<string, int>> digit_prefix_division(string s) {
	vector<pair<string, int>> ret;
	int n = s.length();
	ret.push_back({ "0", 0 });
	if (s != "0") {
		Loop(i, n - 1) {
			Loop1(j, 9) {
				ret.push_back({ "", i });
				ret.back().first += ('0' + j);
			}
		}
		Loop(i, n) {
			Loop(j, s[i] - '0') {
				if (i + j == 0) continue;
				ret.push_back({ s.substr(0, i), n - i - 1 });
				ret.back().first += ('0' + j);
			}
		}
		ret.push_back({ s, 0 });
	}
	return ret;
}


// bitmask DP

vi decode(int x) {
	vi ret;
	int p = 0;
	while (x > 0) {
		if (x & 1) ret.push_back(p);
		x >>= 1;
		p += 1;
	}
	return ret;
}

int encode(vi a) {
	int ret = 0;
	Loop(i, a.size()) {
		ret += (1 << a[i]);
	}
	return ret;
}
