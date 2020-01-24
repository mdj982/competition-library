namespace Subset_Convolution {

	// f.size() should be 2^digit, ret will assemble value from subsets
	vll Zeta_trans(const vll &f) {
		int n = int(f.size());
		int digit = ceillog2(n);
		vll ret = f;
		Loop(i, digit) {
			int x = 1 << i;
			Loop(j, n) {
				if (j & x) ret[j] += ret[j ^ x];
			}
		}
		return ret;
	}

	// g.size() should be 2^digit, ret will disassemble value to subsets
	vll Mobius_trans(const vll &g) {
		int n = int(g.size());
		int digit = ceillog2(n);
		vll ret = g;
		Loop(i, digit) {
			int x = 1 << i;
			Loop(j, n) {
				if (j & x) ret[j] -= ret[j ^ x];
			}
		}
		return ret;
	}

	// f.size() should be 2^digit, ret will assemble value from supersets
	vll Zeta_trans_rev(const vll &f) {
		int n = int(f.size());
		int digit = ceillog2(n);
		vll ret = f;
		Loop(i, digit) {
			int x = 1 << i;
			Loop(j, n) {
				if (!(j & x)) ret[j] += ret[j | x];
			}
		}
		return ret;
	}

	// g.size() should be 2^digit, ret will disassemble value to supersets
	vll Mobius_trans_rev(const vll &g) {
		int n = int(g.size());
		int digit = ceillog2(n);
		vll ret = g;
		Loop(i, digit) {
			int x = 1 << i;
			Loop(j, n) {
				if (!(j & x)) ret[j] -= ret[j | x];
			}
		}
		return ret;
	}

	int bit_count(int x) {
		int ret = 0;
		while (x != 0) {
			ret += x & 1;
			x >>= 1;
		}
		return ret;
	}

	vvll ranked_Zeta_trans(const vll &f) {
		int n = int(f.size());
		int digit = ceillog2(n);
		vvll ret(digit + 1, vll(n));
		vvll rf(digit + 1, vll(n)); // ranked f
		Loop(j, n) {
			rf[bit_count(j)][j] = f[j];
		}
		Loop(i, digit + 1) {
			ret[i] = Zeta_trans(rf[i]);
		}
		return ret;
	}

	vll ranked_Mobius_trans(const vvll &f) {
		int digit = int(f.size()) - 1;
		int n = 1 << digit;
		vll ret(n);
		vvll rf(digit + 1, vll(n));
		Loop(i, digit + 1) {
			rf[i] = Mobius_trans(f[i]);
		}
		Loop(j, n) {
			ret[j] = rf[bit_count(j)][j];
		}
		return ret;
	}

	// ret(z) = \sum_{x&y=0,x+y=z} f(x)g(y)
	vll subset_convolution(const vll &f, const vll &g) {
		int n = int(f.size());
		int digit = ceillog2(n);
		vll fz = Zeta_trans(f);
		vll gz = Zeta_trans(g);
		vll fzgz(n);
		Loop(j, n) {
			fzgz[j] += fz[j] * gz[j];
		}
		vll ret = Mobius_trans(fzgz);
		return ret;
	}

	// O*(4^digit) version
	vll naive_subset_convolution(const vll &f, const vll &g) {
		int n = int(f.size());
		int digit = ceillog2(n);
		vll ret(n);
		Loop(i, n) {
			Loop(j, n) {
				if (i & j) continue;
				else {
					ret[i | j] += f[i] * g[j];
				}
			}
		}
		return ret;
	}

}

using namespace Subset_Convolution;