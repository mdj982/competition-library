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
	// return the number of '0' in s[0, p]
	int rank0(int p) {
		int q = p >> 6;
		int r = p & 0b111111;
		return L0[q] + r + 1 - int((B[q] << (63 - r)).count());
	}
	// return the number of '1' in s[0, p]
	int rank1(int p) {
		int q = p >> 6;
		int r = p & 0b111111;
		return L1[q] + int((B[q] << (63 - r)).count());
	}
	// return the position of k-th '0', return -1 if not exists
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
	// return the position of k-th '1', return -1 if not exists
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