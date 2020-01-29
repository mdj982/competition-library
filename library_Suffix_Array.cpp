namespace Suffix_Array {
	struct sa_t {
		int r0, r1;
		int p;
		bool operator<(const sa_t &another) const {
			return r0 != another.r0 ? r0 < another.r0 : r1 < another.r1;
		}
	};
	// excluding empty substring
	vi suffix_array(const vi &s) {
		int n = s.size();
		vi ret(n);
		vector<sa_t> a(n); // fst = current rank, snd add rank
		Loop(k, ceillog2(n) + 1) {
			if (k == 0) {
				Loop(i, n) a[i] = { s[i], 0, i };
			}
			else {
				int d = 1 << (k - 1);
				Loop(i, n) {
					if (inrange(a[i].p + d, n)) a[i].r1 = ret[a[i].p + d];
					else a[i].r1 = -1;
				}
			}
			sort(a.begin(), a.end());
			sa_t pre;
			Loop(i, n) {
				if (i > 0 && a[i].r0 == pre.r0 && a[i].r1 == pre.r1) {
					a[i] = { a[i - 1].r0, 0, a[i].p };
				}
				else {
					pre = a[i];
					a[i] = { i, 0, a[i].p };
				}
				ret[a[i].p] = a[i].r0;
			}
		}
		return ret;
	}
};