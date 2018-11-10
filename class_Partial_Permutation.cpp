template<typename val_t>
class Partial_Permutation {
private:
	int n;
	vector<val_t> a;
	vector<bool> used;
	vector<vector<val_t>> result;
	vvi facts; // iPj
	void core_func(int n, int r, int start) {
		if (r == 0) return;
		int m = facts[n - 1][r - 1];
		int cnt = 0;
		Loop(i, Partial_Permutation::n) {
			if (!used[i]) {
				Loop(j, m) {
					result[start + m * cnt + j].push_back(a[i]);
				}
				used[i] = true;
				core_func(n - 1, r - 1, start + m * cnt);
				used[i] = false;
				cnt++;
			}
		}
	}
public:
	vector<vector<val_t>> get_partial_permutation(vector<val_t> a, int r) {
		Partial_Permutation::a = a;
		n = int(a.size());
		if (n < r) return {};
		used = vector<bool>(n, false);
		facts = vvi(n + 1, vi(n + 1));
		Loop(i, n + 1) {
			facts[i][0] = 1;
			Loop(j, i) {
				facts[i][j + 1] = facts[i][j] * (i - j);
			}
		}
		result.resize(facts[n][r]);
		core_func(n, r, 0);
		return result;
	}
};


class Partial_Permutation_String {
private:
	int n;
	string a;
	vector<bool> used;
	vector<string> result;
	vvi facts; // iPj
	void core_func(int n, int r, int start) {
		if (r == 0) return;
		int m = facts[n - 1][r - 1];
		int cnt = 0;
		Loop(i, Partial_Permutation_String::n) {
			if (!used[i]) {
				Loop(j, m) {
					result[start + m * cnt + j] += a[i];
				}
				used[i] = true;
				core_func(n - 1, r - 1, start + m * cnt);
				used[i] = false;
				cnt++;
			}
		}
	}
public:
	vector<string> get_partial_permutation(string a, int r) {
		Partial_Permutation_String::a = a;
		n = int(a.size());
		if (n < r) return {};
		used = vector<bool>(n, false);
		facts = vvi(n + 1, vi(n + 1));
		Loop(i, n + 1) {
			facts[i][0] = 1;
			Loop(j, i) {
				facts[i][j + 1] = facts[i][j] * (i - j);
			}
		}
		result.resize(facts[n][r]);
		core_func(n, r, 0);
		return result;
	}
};