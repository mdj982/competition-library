namespace matrix_op {

	template<class T>
	void row_mul(vector<T> &u, T k) {
		Loop(i, u.size()) u[i] *= k;
	}

	template<class T>
	void row_add(vector<T> &u, const vector<T> &v) {
		Loop(i, u.size()) u[i] += v[i];
	}

	int rank(vector<vector<double>> A) {
		int n = A.size();
		int m = A.back().size();
		Loop(j, m) {
			bool is_zero = true;
			Loop(i, n) {
				if (i < j) continue;
				if (!nearlyeq(A[i][j], 0)) {
					swap(A[i], A[j]);
					is_zero = false;
					break;
				}
			}
			if (is_zero) continue;
			Loop(i, n) {
				if (i == j) row_mul(A[i], 1.0 / A[i][j]);
				else if (i > j && !nearlyeq(A[i][j], 0)) {
					row_mul(A[i], -1.0 / A[i][j]);
					row_add(A[i], A[j]);
				}
			}
		}
		int ret = 0;
		Loop(i, min(n, m)) {
			if (nearlyeq(A[i][i], 1)) ret++;
		}
		return ret;
	}

	int rank(vvll A) {
		int n = A.size();
		int m = A.back().size();
		Loop(j, m) {
			bool is_zero = true;
			Loop(i, n) {
				if (i < j) continue;
				if (A[i][j] != 0) {
					swap(A[i], A[j]);
					is_zero = false;
					break;
				}
			}
			Loop(i, n) {
				if (i > j && A[i][j] != 0) {
					ll g = gcd(A[j][j], A[i][j]);
					row_mul(A[j], A[i][j] / g);
					row_mul(A[i], -A[j][j] / g);
					row_add(A[i], A[j]);
				}
				printmx(A);
				cout << endl;
			}
		}
		int ret = 0;
		Loop(i, min(n, m)) {
			if (A[i][i] != 0) ret++;
		}
		return ret;
	}

}

using namespace matrix_op;