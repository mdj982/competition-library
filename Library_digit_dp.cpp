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