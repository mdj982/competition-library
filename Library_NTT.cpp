#include "auto_util_header.hpp"
#include "Library_mod.cpp"

namespace number_theoretic_transform {

	// when MOD - 1 = 2^m * a,
	// min_omega = root^a (try 3,5,7,... to get root)
	// min_omega_depth = m
	// mod_half = (MOD + 1) / 2

	modll min_omega;
	int min_omega_depth;
	modll mod_half;

	void make_base(int mode) {
		switch (mode) {
			/*
		case 0:
			MOD = 167772161;
			min_omega = 17;
			min_omega_depth = 25;
			mod_half = 83886081;
			break;
		case 1:
			MOD = 469762049;
			min_omega = 30;
			min_omega_depth = 26;
			mod_half = 234881025;
			break;
		case 2:
			MOD = 1224736769;
			min_omega = 149;
			min_omega_depth = 24;
			mod_half = 612368385;
			break;
			*/
		default:
			MOD = 924844033;
			min_omega = 3597;
			min_omega_depth = 21;
			mod_half = 462422017;
		}
	}

	vector<modll> omegas, iomegas;

	inline int bit_reverse(int x, int digit) {
		int ret = digit ? x & 1 : 0;
		Loop(i, digit - 1) { ret <<= 1; x >>= 1; ret |= x & 1; }
		return ret;
	}

	inline void make_omegas(int n) {
		if (omegas.size() != n) {
			omegas.resize(n);
			modll omega = pow(min_omega, (1 << min_omega_depth) / n);
			Loop(i, n) {
				if (i == 0) omegas[i] = 1;
				else omegas[i] = omegas[i - 1] * omega;
			}
		}
	}

	inline void make_iomegas(int n) {
		if (iomegas.size() != n) {
			iomegas.resize(n);
			modll iomega = modll(1) / pow(min_omega, (1 << min_omega_depth) / n);
			Loop(i, n) {
				if (i == 0) iomegas[i] = 1;
				else iomegas[i] = iomegas[i - 1] * iomega;
			}
		}
	}

	// a.size() should be 2^digit
	vector<modll> NTT(const vector<modll> a, int mode = 0) {
		int n = int(a.size());
		int digit = int(rndf(log2(n)));
		vector<modll> ret = a;
		make_omegas(n);
		Loop(i, n) {
			int j = bit_reverse(i, digit);
			if (j > i) std::swap(ret[i], ret[j]);
		}
		Loop(i, digit) {
			int j = 0, m = 1 << i, mw = (digit - i - 1);
			Loop(group_id, n >> (i + 1)) {
				Loop(k, m) {
					modll x = ret[j] + omegas[k << mw] * ret[j + m];
					modll y = ret[j] - omegas[k << mw] * ret[j + m];
					ret[j] = x; ret[j + m] = y;
					++j;
				}
				j += m;
			}
		}
		return ret;
	}

	// f.size() should be 2^digit
	vector<modll> INTT(const vector<modll>& f, int mode = 0) {
		int n = int(f.size());
		int digit = int(rndf(log2(n)));
		vector<modll> ret = f;
		make_iomegas(n);
		Loopr(i, digit) {
			int j = 0, m = 1 << i, mw = (digit - i - 1);
			Loop(group_id, n >> (i + 1)) {
				Loop(k, m) {
					modll q = (ret[j] + ret[j + m]) * mod_half;
					modll r = (ret[j] - ret[j + m]) * mod_half * iomegas[k << mw];
					ret[j] = q; ret[j + m] = r;
					++j;
				}
				j += m;
			}
		}
		Loop(i, n) {
			int j = bit_reverse(i, digit);
			if (j > i) std::swap(ret[i], ret[j]);
		}
		return ret;
	}

	// a.size() = b.size() should be 2^digit
	vector<modll> mul_convolution(const vector<modll> &a, const vector<modll> &b) {
		int n = int(a.size());
		vector<modll> ret;
		make_base(0);
		// Garner's algorithm is unsupported yet 
		vector<modll> g = NTT(a), h = NTT(b);
		Loop(i, n) g[i] *= h[i];
		ret = INTT(g);
		return ret;
	}

	int legal_size_of(int n) {
		int ret = 1 << (int)log2(n);
		if (ret < n) ret <<= 1;
		return ret;
	}
}

using namespace number_theoretic_transform;