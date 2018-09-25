inline int bit_reverse(int x, int digit) {
	int ret = digit ? x & 1 : 0;
	Loop(i, digit - 1) { ret <<= 1; x >>= 1; ret |= x & 1; }
	return ret;
}

// a.size() should be 2^digit
vector<cdouble> FFT(vector<cdouble>& a) {
	int n = int(a.size());
	int digit = int(rndf(log2(n)));
	vector<cdouble> ret(n);
	vector<cdouble> w(n >> 1);
	Loop(i, n >> 1) w[i] = exp(cdouble({ 0, 2 * PI * i / n }));
	Loop(i, n) ret[bit_reverse(i, digit)] = a[i];
	Loop(i, digit) {
		int j = 0, m = 1 << i, mw = (digit - i - 1);
		Loop(group_id, n >> (i + 1)) {
			Loop(k, m) {
				cdouble buf = w[k << mw] * ret[j + m];
				ret[j] = ret[j] + buf;
				ret[j + m] = ret[j] - buf * 2.0;
				++j;
			}
			j += m;
		}
	}
	return ret;
}

// f.size() should be 2^digit
vector<cdouble> IFFT(const vector<cdouble>& f) {
	int n = int(f.size());
	int digit = int(rndf(log2(n)));
	vector<cdouble> a = f;
	vector<cdouble> w(n >> 1);
	Loop(i, n >> 1) w[i] = exp(cdouble({ 0, 2 * PI * i / n }));
	Loopr(i, digit) {
		int j = 0, m = 1 << i, mw = (digit - i - 1);
		Loop(group_id, n >> (i + 1)) {
			Loop(k, m) {
				a[j] = (a[j] + a[j + m]) / 2.0;
				a[j + m] = (a[j] - a[j + m]) / w[k << mw];
				++j;
			}
			j += m;
		}
	}
	vector<cdouble> ret(n);
	Loop(i, n) ret[bit_reverse(i, digit)] = a[i];
	return ret;
}

int legal_size_of(int n) {
	int ret = 1 << (int)log2(n);
	if (ret < n) ret <<= 1;
	return ret;
}