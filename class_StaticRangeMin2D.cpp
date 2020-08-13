#include "auto_util_header.hpp"

class StaticRangeMin2D {
private:
	int n, m;
	int log_n, log_m;
	vector<vvvll> table;
	vi floorlogs;
	void make_sprstable() {
		Loop1(ki, log_n - 1) {
			int di = 1 << ki;
			Loop(i, n - di) {
				Loop(j, m) {
					ll &val = table[ki][0][i][j];
					val = min(val, table[ki - 1][0][i][j]);
					val = min(val, table[ki - 1][0][i + (di >> 1)][j]);
				}
			}
		}
		Loop1(kj, log_m - 1) {
			int dj = 1 << kj;
			Loop(ki, log_n) {
				int di = 1 << ki;
				Loop(i, n - di) {
					Loop(j, m - dj) {
						ll &val = table[ki][kj][i][j];
						val = min(val, table[ki][kj - 1][i][j]);
						val = min(val, table[ki][kj - 1][i][j + (dj >> 1)]);
					}
				}
			}
		}
	}
	void make_floorlogs() {
		floorlogs.resize(min(n, m) + 1);
		int p = 0;
		Loop(i, floorlogs.size()) {
			if ((1 << (p + 1)) == i) ++p;
			floorlogs[i] = p;
		}
	}
public:
	StaticRangeMin2D(const vvll &a) {
		this->n = int(a.size());
		this->m = int(a[0].size());
		this->log_n = floorlog2(this->n) + 1;
		this->log_m = floorlog2(this->m) + 1;
		this->table = vector<vvvll>(log_n, vvvll(log_m, vvll(n, vll(m, LLONG_MAX))));
		Loop(i, n) {
			Loop(j, m) {
				table[0][0][i][j] = a[i][j];
			}
		}
		make_sprstable();
		make_floorlogs();
	}
	ll minof(int s0, int t0, int s1, int t1) {
		int ki = floorlogs[t0 - s0];
		int kj = floorlogs[t1 - s1];
		int di = 1 << ki;
		int dj = 1 << kj;
		ll ret = LLONG_MAX;
		ret = min(ret, table[ki][kj][s0][s1]);
		ret = min(ret, table[ki][kj][t0 - di][s1]);
		ret = min(ret, table[ki][kj][s0][t1 - dj]);
		ret = min(ret, table[ki][kj][t0 - di][t1 - dj]);
		return ret;
	}
};