#include "auto_util_header.hpp"

// ret[i] := the max length of common prefix between s[i, n) and s
vi Z_algorithm(const string &s) {
	int n = s.length();
	vi ret(n);
	ret[0] = n;
	for (int i = 1, j = 0; i < n;) {
		while (i + j < n && s[j] == s[i + j]) ++j;
		ret[i] = j;
		if (j == 0) ++i;
		else {
			int k = 1;
			while (i + k < n && k + ret[k] < j) {
				ret[i + k] = ret[k], ++k;
			}
			i += k; j -= k;
		}
	}
	return ret;
}

// ret[i] := the max length of common prefix between s[i, n) and t
vi Z_algorithm(const string &s, const string &t) {
	int n = s.length();
	int m = t.length();
	vi ret(n);
	vi a = Z_algorithm(t + s);
	Loop(i, n) {
		ret[i] = std::min(m, a[m + i]);
	}
	return ret;
}