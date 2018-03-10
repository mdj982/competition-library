using namespace mod_op;

class String_Numeric {
private:
  char base;
  int decimal, length;
public:
  vll unit_list;
  String_Numeric(char base, int decimal, int length) {
    String_Numeric::base = base;
    String_Numeric::decimal = decimal;
    String_Numeric::length = length;
    unit_list = vll(length, 1);
    Loop1(i, length - 1) {
      unit_list[i] = mul(unit_list[i - 1], decimal);
    }
  }
  string addstr(string a, string b) {
    int n = max(a.size(), b.size());
    string ret;
    reverse(a.begin(), a.end());
    reverse(b.begin(), b.end());
    int carry = 0;
    Loop(i, n) {
      int sum;
      if (i >= a.size()) {
        sum = (b[i] - base) + carry;
      }
      else if (i >= b.size()) {
        sum = (a[i] - base) + carry;
      }
      else {
        sum = (a[i] - base) + (b[i] - base) + carry;
      }
      if (sum >= decimal) {
        sum %= decimal;
        carry = 1;
      }
      else {
        carry = 0;
      }
      ret += base + sum;
    }
    if (carry) ret += base + 1;
    reverse(ret.begin(), ret.end());
    return ret;
  }
  bool has_leading_zero(string s) {
    if (s.length() > 1 && s[0] == base) return true;
    else return false;
  }
  ll modstr(string s) {
    int n = s.length();
    ll ret = 0;
    Loop(i, n) {
      ret = add(ret, mul(s[n - i - 1] - base, unit_list[i]));
    }
    return ret;
  }
};