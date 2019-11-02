class Warshallfloyd {
private:
	int n;
	bool negative_cycle;
	vvll table;
public:
	Warshallfloyd(const vvi &lst, const vvll &cst) {
		n = lst.size();
		table = vvll(n, vll(n, LLONG_MAX));
		Loop(i, n) {
			Loop(j, lst[i].size()) {
				table[i][lst[i][j]] = cst[i][j];
			}
		}
		Loop(i, n) table[i][i] = 0;
		Loop(k, n) {
			Loop(i, n) {
				Loop(j, n) {
					if (table[i][k] == LLONG_MAX || table[k][j] == LLONG_MAX) continue;
					table[i][j] = min(table[i][j], table[i][k] + table[k][j]);
				}
			}
		}
		Loop(i, n) {
			if (table[i][i] < 0) {
				negative_cycle = true;
				return;
			}
		}
		negative_cycle = false;
		return;
	}
	vvll get_table() {
		return table;
	}
	ll get_dist(int i, int j) {
		return table[i][j];
	}
	bool is_negative_cycle() {
		return negative_cycle;
	}
};