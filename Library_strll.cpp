#include "auto_util_header.hpp"

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
		inline string uadd_core(const string &s, const string &t) {
			int n = s.length();
			int m = t.length();
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
		inline string usub_core(const string &s, const string &t) {
			int n = s.length();
			int m = t.length();
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
		inline string umul_core(const string &s, const string &t) {
			int n = s.length();
			int m = t.length();
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
		inline bool uge_core(const string &s, const string &t) const {
			int n = s.length();
			int m = t.length();
			while (n > 1 && s[n - 1] == '0') n--;
			while (m > 1 && t[m - 1] == '0') m--;
			if (n > m) return true;
			else if (n < m) return false;
			else {
				Loopr(i, n) {
					if (s[i] > t[i]) return true;
					if (s[i] < t[i]) return false;
				}
				return true;
			}
		}
		inline string udiv_core(string s, const string &t, bool rem_flag) {
			int n = s.length();
			int m = t.length();
			string ret = "0";
			Loopr(i, n - m + 1) {
				ret += '0';
				string sbuf = s.substr(i, m + 1);
				while (uge_core(sbuf, t)) {
					sbuf = usub_core(sbuf, t);
					ret.back()++;
				}
				Loop(j, min(m + 1, n)) {
					s[i + j] = j < sbuf.size() ? sbuf[j] : '0';
				}
			}
			reverse(ret.begin(), ret.end());
			if (rem_flag) ret = s;
			while (ret.size() > 1 && ret.back() == '0') ret.pop_back();
			return ret;
		}
		inline string add(const string &s, const string &t) {
			int n = int(s.length());
			int m = int(t.length());
			string ret = "";
			int mode = (s[n - 1] == '-' ? 0b10 : 0) + (t[m - 1] == '-' ? 0b01 : 0);
			switch (mode) {
			case 0b00:
				ret = uadd_core(s, t);
				break;
			case 0b01:
				if (uge_core(s.substr(0, n), t.substr(0, m - 1))) ret = usub_core(s, t.substr(0, m - 1));
				else ret = usub_core(t.substr(0, m - 1), s) + '-';
				break;
			case 0b10:
				if (uge_core(s.substr(0, n - 1), t.substr(0, m))) ret = usub_core(s.substr(0, n - 1), t) + '-';
				else ret = usub_core(t, s.substr(0, n - 1));
				break;
			case 0b11:
				ret = uadd_core(s.substr(0, n - 1), t.substr(0, m - 1)) + '-';
				break;
			}
			if (ret == "0-") ret.pop_back();
			return ret;
		}
		inline string sub(const string &s, const string &t) {
			string ret = "";
			int n = s.length();
			int m = t.length();
			int mode = (s[n - 1] == '-' ? 0b10 : 0) + (t[m - 1] == '-' ? 0b01 : 0);
			switch (mode) {
			case 0b00:
				if (uge_core(s.substr(0, n), t.substr(0, m))) ret = usub_core(s, t);
				else ret = usub_core(t, s) + '-';
				break;
			case 0b01:
				ret = uadd_core(s, t.substr(0, m - 1));
				break;
			case 0b10:
				ret = uadd_core(s.substr(0, n - 1), t) + '-';
				break;
			case 0b11:
				if (uge_core(s.substr(0, n - 1), t.substr(0, m - 1))) ret = usub_core(s.substr(0, n - 1), t.substr(0, m - 1)) + '-';
				else ret = usub_core(t.substr(0, m - 1), s.substr(0, n - 1));
				break;
			}
			if (ret == "0-") ret.pop_back();
			return ret;
		}
		inline string mul(const string &s, const string &t) {
			string ret;
			int n = s.length();
			int m = t.length();
			int mode = (s[n - 1] == '-' ? 0b10 : 0) + (t[m - 1] == '-' ? 0b01 : 0);
			switch (mode) {
			case 0b00:
				ret = umul_core(s, t);
				break;
			case 0b01:
				ret = umul_core(s, t.substr(0, m - 1)) + '-';
				break;
			case 0b10:
				ret = umul_core(s.substr(0, n - 1), t) + '-';
				break;
			case 0b11:
				ret = umul_core(s.substr(0, n - 1), t.substr(0, m - 1));
				break;
			}
			if (ret == "0-") ret.pop_back();
			return ret;
		}
		inline bool ge(const string &s, const string &t) const {
			bool ret;
			int n = s.length();
			int m = t.length();
			int mode = (s[n - 1] == '-' ? 0b10 : 0) + (t[m - 1] == '-' ? 0b01 : 0);
			switch (mode) {
			case 0b00:
				ret = uge_core(s, t);
				break;
			case 0b01:
				ret = true;
				break;
			case 0b10:
				ret = false;
				break;
			case 0b11:
				if (s == t) ret = true;
				else ret = !uge_core(s.substr(0, n - 1), t.substr(0, m - 1));
				break;
			}
			return ret;
		}
		inline string div(const string &s, const string &t, bool rem_flag) {
			string ret;
			int n = s.length();
			int m = t.length();
			int mode = (s[n - 1] == '-' ? 0b10 : 0) + (t[m - 1] == '-' ? 0b01 : 0);
			switch (mode) {
			case 0b00:
				ret = udiv_core(s, t, rem_flag);
				break;
			case 0b01:
				ret = udiv_core(s, t.substr(0, m - 1), rem_flag);
				if (!rem_flag) ret += '-';
				break;
			case 0b10:
				ret = udiv_core(s.substr(0, n - 1), t, rem_flag) + '-';
				break;
			case 0b11:
				ret = udiv_core(s.substr(0, n - 1), t.substr(0, m - 1), rem_flag);
				if (rem_flag) ret += '-';
				break;
			}
			if (ret == "0-") ret.pop_back();
			return ret;
		}
	public:
		strll(string init = "0") { reverse(init.begin(), init.end()); val = init; return; }
		strll(ll init) { val = ll_to_strll(init); return; }
		strll(const strll& another) { val = another.val; return; }
		inline strll& operator=(const strll &another) { val = another.val; return *this; }
		inline strll operator+(const strll &x) { strll ret; ret.val = add(val, x.val); return ret; }
		inline strll operator-(const strll &x) { strll ret; ret.val = sub(val, x.val); return ret; }
		inline strll operator*(const strll &x) { strll ret; ret.val = mul(val, x.val); return ret; }
		inline strll operator/(const strll &x) { strll ret; ret.val = div(val, x.val, false); return ret; }
		inline strll operator%(const strll &x) { strll ret; ret.val = div(val, x.val, true); return ret; }
		inline strll& operator+=(const strll &x) { val = add(val, x.val); return *this; }
		inline strll& operator-=(const strll &x) { val = sub(val, x.val); return *this; }
		inline strll& operator*=(const strll &x) { val = mul(val, x.val); return *this; }
		inline strll& operator/=(const strll &x) { val = div(val, x.val, false); return *this; }
		inline strll& operator%=(const strll &x) { val = div(val, x.val, true); return *this; }
		inline bool operator>=(const strll &x) { return ge(val, x.val); }
		inline bool operator>(const strll &x) { return ge(val, x.val) && val != x.val; }
		inline bool operator<=(const strll &x) { return ge(x.val, val); }
		inline bool operator<(const strll &x) { return ge(x.val, val) && val != x.val; }
		inline bool operator==(const strll &x) { return val == x.val; }
		inline bool operator!=(const strll &x) { return val != x.val; }
		inline bool operator<(const strll &x) const { return !ge(val, x.val); }
		friend inline istream& operator >> (istream &is, strll& x) { is >> x.val; reverse(x.val.begin(), x.val.end()); return is; }
		friend inline ostream& operator << (ostream &os, const strll& x) { os << x.get_val(); return os; }
		string get_val() const { string ret = val; reverse(ret.begin(), ret.end()); return ret; }
	};
}

using namespace strll_op;
typedef vector<strll> vstrll;
typedef vector<vector<strll>> vvstrll;