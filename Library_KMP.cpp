#include "auto_util_header.hpp"

// the skip table used in KMP algorithm
// ret[i] := the longest value x < i such that s.substr(0, x) == s.substr(i - x, x)
vi solve_KMP_table(const string &s) {
	int n = int(s.size());
	vi ret(n + 1, -1);
	for (int i = 0, j = -1; i < n; ++i) {
		while (j >= 0 && s[i] != s[j]) j = ret[j];
		++j;
		ret[i + 1] = j;
	}
	return ret;
}

// ret[i] == 1 <=> s[i, s + t.length()) == t
// t_table = solve_KMP_table(t)
vi KMP_matching(const string &s, const string &t, const vi &t_table) {
	int n = int(s.size());
	int m = int(t.size());
	vi ret(n, 0);
	for (int i = 0, j = 0, k = 0; i < n;) {
		if (s[i] == t[j]) {
			++i; ++j;
			if (j == m) {
				ret[k] = 1;
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

// ret[i] == 1 <=> s[i, s + t.length()) == t
vi KMP_matching(const string &s, const string &t) {
	vi t_table = solve_KMP_table(t);
	return KMP_matching(s, t, t_table);
}