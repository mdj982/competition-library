#include "auto_util_header.hpp"

// ret.fst[i] = x <=> "left (x - 1) letters + s[i] + right (x - 1) letters" is a palindrome
// ret.snd[i] = x <=> "left (x - 1) letters + s[i] + s[i + 1] + right (x - 1) letters" is a palindrome
pair<vi, vi> Manacher(const string &s) {
	if (s.size() == 0) return { {},{} };
	string t((s.size() << 1) - 1, '$');
	Loop(i, s.size()) {
		t[i << 1] = s[i];
	}
	int n = t.size();
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
	ret.fst = vi(s.size());
	ret.snd = vi(s.size() - 1);
	Loop(i, ret.fst.size()) {
		ret.fst[i] = (x[i << 1] + 1) >> 1;
	}
	Loop(i, ret.snd.size()) {
		ret.snd[i] = x[(i << 1) + 1] >> 1;
	}
	return ret;
}