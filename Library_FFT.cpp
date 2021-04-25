#include "auto_util_header.hpp"

namespace Fourier_transform {

	vector<cdouble> omegas, iomegas;

	inline int bit_reverse(int x, int digit) {
		int ret = digit ? x & 1 : 0;
		Loop(i, digit - 1) { ret <<= 1; x >>= 1; ret |= x & 1; }
		return ret;
	}

	inline void make_omegas(int n) {
		if (omegas.size() != n) {
			omegas.resize(n);
			Loop(i, n) omegas[i] = exp(cdouble({ 0, 2 * M_PI * i / n }));
		}
	}

	inline void make_iomegas(int n) {
		if (iomegas.size() != n) {
			iomegas.resize(n);
			Loop(i, n) iomegas[i] = exp(cdouble({ 0, -2 * M_PI * i / n }));
		}
	}

	// a.size() should be 2^digit
	vector<cdouble> FFT(const vector<cdouble> a) {
		int n = int(a.size());
		int digit = int(rndf(log2(n)));
		vector<cdouble> ret = a;
		make_omegas(n);
		Loop(i, n) {
			int j = bit_reverse(i, digit);
			if (j > i) swap(ret[i], ret[j]);
		}
		Loop(i, digit) {
			int j = 0, m = 1 << i, mw = (digit - i - 1);
			Loop(group_id, n >> (i + 1)) {
				Loop(k, m) {
					cdouble x = ret[j] + omegas[k << mw] * ret[j + m];
					cdouble y = ret[j] - omegas[k << mw] * ret[j + m];
					ret[j] = x; ret[j + m] = y;
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
		vector<cdouble> ret = f;
		make_iomegas(n);
		Loopr(i, digit) {
			int j = 0, m = 1 << i, mw = (digit - i - 1);
			Loop(group_id, n >> (i + 1)) {
				Loop(k, m) {
					cdouble q = (ret[j] + ret[j + m]) * 0.5;
					cdouble r = (ret[j] - ret[j + m]) * 0.5 * iomegas[k << mw];
					ret[j] = q; ret[j + m] = r;
					++j;
				}
				j += m;
			}
		}
		Loop(i, n) {
			int j = bit_reverse(i, digit);
			if (j > i) swap(ret[i], ret[j]);
		}
		return ret;
	}

	// a.size() = b.size() should be 2^digit
	vector<cdouble> mul_convolution(const vector<cdouble> &a, const vector<cdouble> &b) {
		int n = int(a.size());
		vector<cdouble> ret;
		vector<cdouble> g = FFT(a), h = FFT(b);
		Loop(i, n) g[i] *= h[i];
		ret = IFFT(g);
		return ret;
	}

	int legal_size_of(int n) {
		int ret = 1 << (int)log2(n);
		if (ret < n) ret <<= 1;
		return ret;
	}
}

using namespace Fourier_transform;