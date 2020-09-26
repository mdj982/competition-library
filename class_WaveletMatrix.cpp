#include "auto_util_header.hpp"

// not strictly a succinct data structure
class SuccinctBitVector {
private:
	int n, m;
	vector<int> L0;
	vector<int> L1;
	vector<bitset<64>> B; // divided blocks
public:
	// s should be composed of '0' or '1'
	SuccinctBitVector(const string &s) {
		this->n = int(s.length());
		this->m = ceildiv(n, 64);
		this->L0.resize(m + 1, 0);
		this->L1.resize(m + 1, 0);
		this->B.resize(m, 0);
		int p = 0;
		Loop(i, m) {
			Loop(j, 64) {
				if (p == n) break;
				B[i][j] = s[p++] - '0';
			}
			int c = int(B[i].count());
			L0[i + 1] = L0[i] + 64 - c;
			L1[i + 1] = L1[i] + c;
		}
	}
	// return s[p] - '0'
	int access(int p) {
		int q = p >> 6;
		int r = p & 0b111111;
		return B[q][r];
	}
	// return the number of '0' in s[0, p)
	int rank0(int p) {
		if (p <= 0) return 0;
		if (p > n) p = n;
		p--;
		int q = p >> 6;
		int r = p & 0b111111;
		return L0[q] + r + 1 - int((B[q] << (63 - r)).count());
	}
	// return the number of '1' in s[0, p)
	int rank1(int p) {
		if (p <= 0) return 0;
		if (p > n) p = n;
		p--;
		int q = p >> 6;
		int r = p & 0b111111;
		return L1[q] + int((B[q] << (63 - r)).count());
	}
	// return the position of (0-indexed) k-th '0', return -1 if not exists
	int select0(int k) {
		int q = int(std::lower_bound(L0.begin(), L0.end(), k + 1) - L0.begin()) - 1;
		if (q < m) {
			int t = k - L0[q];
			B[q].flip();
			int r = int(B[q]._Find_first());
			Loop(i, t) {
				r = int(B[q]._Find_next(r));
			}
			B[q].flip();
			return (q << 6) + r;
		}
		else {
			return -1;
		}
	}
	// return the position of (0-indexed) k-th '1', return -1 if not exists
	int select1(int k) {
		int q = int(std::lower_bound(L1.begin(), L1.end(), k + 1) - L1.begin()) - 1;
		if (q < m) {
			int t = k - L1[q];
			int r = int(B[q]._Find_first());
			Loop(i, t) {
				r = int(B[q]._Find_next(r));
			}
			return (q << 6) + r;
		}
		else {
			return -1;
		}
	}
};

class WaveletMatrix {
private:
	int n, m;
	vector<SuccinctBitVector*> sbv;
	map<ll, int> offset;
public:
	// Required: a[i] >= 0
	WaveletMatrix(const vector<ll> &a) {
		this->n = int(a.size());
		this->m = floorlog2(*max_element(a.begin(), a.end())) + 1;
		this->sbv.resize(this->m);
		struct mval_t {
			int p, b;
			ll x;
			bool operator<(const mval_t &another) const {
				return b != another.b ? b < another.b : p < another.p;
			}
		};
		vector<mval_t> v(this->n);
		Loop(i, this->n) {
			v[i].p = i;
			v[i].x = a[i];
		}
		Loopr(j, this->m) {
			Loop(i, this->n) {
				v[i].b = (v[i].x >> j) & 1;
			}
			string s(this->n, '0');
			Loop(i, n) {
				s[i] += v[i].b;
			}
			sbv[j] = new SuccinctBitVector(s);
			std::sort(v.begin(), v.end());
			Loop(i, this->n) {
				v[i].p = i;
			}
		}
		Loop(i, this->n) {
			if (offset.count(v[i].x)) continue;
			else offset[v[i].x] = i;
		}
	}
	// return the number of x in a[0, p)
	int rank(ll x, int p) {
		if (p <= 0) return 0;
		if (p > n) p = n;
		if (!this->offset.count(x)) return 0;
		int l = this->offset[x];
		int r = p;
		Loopr(j, this->m) {
			if ((x >> j) & 1) {
				r = sbv[j]->rank0(n) + sbv[j]->rank1(r);
			}
			else {
				r = sbv[j]->rank0(r);
			}
		}
		return r - l;
	}
	// return (0-indexed) k-th minimum number in a[l, r)
	ll kth_min(int l, int r, int k) {
		if (k < 0 || k >= r - l) return LLONG_MAX;
		ll x = 0;
		Loopr(j, this->m) {
			x <<= 1;
			int cnt0 = sbv[j]->rank0(r) - sbv[j]->rank0(l);
			if (k < cnt0) {
				l = sbv[j]->rank0(l);
				r = sbv[j]->rank0(r);
			}
			else {
				l = sbv[j]->rank0(n) + sbv[j]->rank1(l);
				r = sbv[j]->rank0(n) + sbv[j]->rank1(r);
				k = k - cnt0;
				x++;
			}
		}
		return x;
	}
};