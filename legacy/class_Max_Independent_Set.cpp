class Max_Independent_Set {
private:
	// x = -1 -> erase, x = 1 -> add
	void set_edge(vvi &mx, vi &d, int s, int t, int x) {
		mx[s][t] += x;
		mx[t][s] += x;
		d[s] += x;
		d[t] += x;
	}
	int max_independent_set_rec(vvi &mx, vi &d) {
		int n = mx.size();
		int max_d = 0, max_id = 0;
		Loop(i, n) {
			if (d[i] > max_d) {
				max_d = d[i];
				max_id = i;
			}
		}
		if (max_d == 0) return 0;
		else {
			int r0 = 0, r1 = 1;
			vi nb_ids;
			vi is_nb(n);
			vvi nb2_ids(n);
			Loop(j, n) {
				if (mx[max_id][j] == 1) {
					nb_ids.push_back(j);
					is_nb[j] = 1;
					set_edge(mx, d, max_id, j, -1);
					if (d[j] == 0) r0++;
				}
			}
			r0 += max_independent_set_rec(mx, d);
			Foreach(id, nb_ids) {
				Loop(j, n) {
					if (mx[id][j] == 1) {
						nb2_ids[id].push_back(j);
						set_edge(mx, d, id, j, -1);
						if (d[j] == 0 && is_nb[j] == 0) r1++;
					}
				}
			}
			r1 += max_independent_set_rec(mx, d);
			Foreach(id, nb_ids) {
				Foreach(j, nb2_ids[id]) {
					set_edge(mx, d, id, j, 1);
				}
			}
			Foreach(j, nb_ids) {
				set_edge(mx, d, max_id, j, 1);
			}
			return max(r0, r1);
		}
	}
public:
	// O(1.466^n)
	int max_independent_set(vvi &mx) {
		int n = mx.size();
		vi d(n);
		Loop(i, n) {
			Loop(j, n) {
				if (mx[i][j] == 1) d[i]++;
			}
		}
		int ret = 0;
		Loop(i, n) if (d[i] == 0) ret++;
		ret += max_independent_set_rec(mx, d);
		return ret;
	}
};