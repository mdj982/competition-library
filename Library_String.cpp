// ret.fst[i] = x <=> "left (x - 1) letters + s[i] + right (x - 1) letters" is a parlindrome
// ret.snd[i] = x <=> "left (x - 1) letters + s[i] + s[i + 1] + right (x - 1) letters" is a parlindrome
pair<vi, vi> Manacher(const string &s) {
	if (s.length() == 0) return { {},{} };
	string t = "";
	Loop(i, s.length()) {
		t += s[i];
		t += '$';
	}
	t.pop_back();
	int n = t.length();
	vi x(n);
	for (int i = 0, j = 0; i < n;) {
		while (inrange(i - j, n) && inrange(i + j, n) && t[i - j] == t[i + j]) {
			++j;
		}
		x[i] = j;
		int k = 1;
		while (inrange(i - k, n) && inrange(i + k, n) && k + x[i - k] < j) {
			x[i + k] = x[i - k];
			++k;
		}
		i += k; j -= k;
	}
	pair<vi, vi> ret;
	ret.fst = vi(s.length());
	ret.snd = vi(s.length() - 1);
	Loop(i, ret.fst.size()) {
		ret.fst[i] = (x[i << 1] + 1) >> 1;
	}
	Loop(i, ret.snd.size()) {
		ret.snd[i] = x[(i << 1) + 1] >> 1;
	}
	return ret;
}

// the skip table used in KMP algorithm
// ret[i] := the longest value x < i such that s.substr(0, x) == s.substr(i - x, x)
vi solve_KMP_table(const string &s) {
	int n = int(s.length());
	vi ret(n + 1, -1);
	for (int i = 0, j = -1; i < n; ++i) {
		while (j >= 0 && s[i] != s[j]) j = ret[j];
		++j;
		ret[i + 1] = j;
	}
	return ret;
}

// return the matched indices in ascending order
// t_table = solve_KMP_table(t)
vi KMP_matching(const string &s, const string &t, const vi &t_table) {
	int n = int(s.length());
	int m = int(t.length());
	vi ret;
	for (int i = 0, j = 0, k = 0; i < n;) {
		if (s[i] == t[j]) {
			++i; ++j;
			if (j == m) {
				ret.push_back(k);
				k += j - t_table[j];
				j = t_table[j];
			}
		}
		else if (j == 0) {
			++i; ++k;
		}
		else {
			k += j - t_table[j];
			j = t_table[j];
		}
	}
	return ret;
}

// return the matched indices in ascending order
vi KMP_matching(const string &s, const string &t) {
	vi t_table = solve_KMP_table(t);
	return KMP_matching(s, t, t_table);
}