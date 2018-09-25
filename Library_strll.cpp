namespace strll_op {

	class strll {
	private:
		string val; // interior process is always reversed
		inline string ll_to_strll(ll x) {
			if (x == 0) return "0";
			bool neg_flag = false;
			if (x < 0) { neg_flag = true;	x *= -1; }
			string ret = "";
			while (x > 0) {
				ret += '0' + (x % 10);
				x /= 10;
			}
			if (neg_flag) ret += '-';
			return ret;
		}
		inline string uadd_core(const string &s, const string &t, int n, int m) {
			string ret = "";
			int v_digits = max(n, m) + 1;
			vi v(v_digits, 0);
			Loop(i, v_digits - 1) {
				if (i < n) v[i] += s[i] - '0';
				if (i < m) v[i] += t[i] - '0';
				if (v[i] >= 10) {
					v[i] -= 10;
					v[i + 1] += 1;
				}
			}
			if (v[v_digits - 1] == 0) v_digits = max(1, v_digits - 1);
			Loop(i, v_digits) ret += '0' + v[i];
			return ret;
		}
		inline string usub_core(const string &s, const string &t, int n, int m) {
			string ret = "";
			int v_digits = 1;
			vi v(n, 0);
			Loop(i, n) {
				v[i] += s[i] - '0';
				if (i < m) v[i] -= (t[i] - '0');
				if (v[i] < 0) {
					v[i] += 10;
					v[i + 1] -= 1;
				}
				if (v[i] > 0) v_digits = i + 1;
			}
			Loop(i, v_digits) ret += '0' + v[i];
			return ret;
		}
		inline string umul_core(const string &s, const string &t, int n, int m) {
			string ret = "";
			vi v(n + m, 0);
			Loop(i, n) {
				Loop(j, m) {
					int z = (s[i] - '0') * (t[j] - '0');
					v[i + j] += z % 10;
					v[i + j + 1] += z / 10;
				}
			}
			int v_digits = 1;
			Loop(i, n + m - 1) {
				v[i + 1] += v[i] / 10;
				v[i] %= 10;
				if (v[i + 1] > 0) v_digits = i + 2;
			}
			Loop(i, v_digits) ret += '0' + v[i];
			return ret;
		}
		inline bool ult_core(const string &s, const string &t, int n, int m) {
			if (n < m) return true;
			if (m < n) return false;
			Loopr(i, n) {
				if (s[i] < t[i]) return true;
				else if (s[i] > t[i]) return false;
			}
			return false;
		}
		inline strll add(const string &s, const string &t) {
			strll ret;
			int n = int(s.length());
			int m = int(t.length());
			int mode = (s[n - 1] == '-' ? 0b10 : 0) + (t[m - 1] == '-' ? 0b01 : 0);
			switch (mode) {
			case 0b00:
				ret.val = uadd_core(s, t, n, m);
				break;
			case 0b01:
				if (ult_core(s, t, n, m - 1)) ret.val = usub_core(t, s, m - 1, n) + '-';
				else ret.val = usub_core(s, t, n, m - 1);
				break;
			case 0b10:
				if (ult_core(s, t, n - 1, m)) ret.val = usub_core(t, s, m, n - 1);
				else ret.val = usub_core(s, t, n - 1, m) + '-';
				break;
			case 0b11:
				ret.val = uadd_core(s, t, n - 1, m - 1) + '-';
				break;
			}
			return ret;
		}
		inline strll sub(const string &s, const string &t) {
			strll ret;
			int n = int(s.length());
			int m = int(t.length());
			int mode = (s[n - 1] == '-' ? 0b10 : 0) + (t[m - 1] == '-' ? 0b01 : 0);
			switch (mode) {
			case 0b00:
				if (ult_core(s, t, n, m)) ret.val = usub_core(t, s, m, n) + '-';
				else ret.val = usub_core(s, t, n, m);
				break;
			case 0b01:
				ret.val = uadd_core(s, t, n, m - 1);
				break;
			case 0b10:
				ret.val = uadd_core(s, t, n - 1, m) + '-';
				break;
			case 0b11:
				if (ult_core(s, t, n - 1, m - 1)) ret.val = usub_core(t, s, m - 1, n - 1);
				else ret.val = usub_core(s, t, n - 1, m - 1) + '-';
				break;
			}
			return ret;
		}
		inline strll mul(const string &s, const string &t) {
			strll ret;
			int n = int(s.length());
			int m = int(t.length());
			int mode = (s[n - 1] == '-' ? 0b10 : 0) + (t[m - 1] == '-' ? 0b01 : 0);
			switch (mode) {
			case 0b00:
				ret.val = umul_core(s, t, n, m);
				break;
			case 0b01:
				ret.val = umul_core(s, t, n, m - 1) + '-';
				break;
			case 0b10:
				ret.val = umul_core(s, t, n - 1, m) + '-';
				break;
			case 0b11:
				ret.val = umul_core(s, t, n - 1, m - 1);
				break;
			}
			return ret;
		}
	public:
		strll(string init = "0") { reverse(init.begin(), init.end()); val = init; return; }
		strll(ll init) { val = ll_to_strll(init); return; }
		strll(const strll& another) { val = another.val; return; }
		inline strll& operator=(const strll &another) { val = another.val; return *this; }
		inline strll operator+(const strll &x) { return add(val, x.val); }
		inline strll operator-(const strll &x) { return sub(val, x.val); }
		inline strll operator*(const strll &x) { return mul(val, x.val); }
		// inline strll operator/(const strll &x) { return div(val, x.val); }
		inline strll& operator+=(const strll &x) { val = add(val, x.val).val; return *this; }
		inline strll& operator-=(const strll &x) { val = sub(val, x.val).val; return *this; }
		inline strll& operator*=(const strll &x) { val = mul(val, x.val).val; return *this; }
		// inline strll& operator/=(const strll &x) { val = div(val, x.val); return *this; }
		inline bool operator==(const strll &x) { return val == x.val; }
		inline bool operator!=(const strll &x) { return val != x.val; }
		friend inline istream& operator >> (istream &is, strll& x) { is >> x.val; reverse(x.val.begin(), x.val.end()); return is; }
		friend inline ostream& operator << (ostream &os, const strll& x) { os << x.get_val(); return os; }
		string get_val() const { string ret = val; reverse(ret.begin(), ret.end()); return ret; }
	};
}

using namespace strll_op;
typedef vector<strll> vstrll;
typedef vector<vector<strll>> vvstrll;