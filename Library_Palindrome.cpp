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