class Hash_String {
private:
  int kind, length;
  char base;
  vll hash_unit_list;
  vll hash_sum_list;
  bool rev_flag;
  bool lock;
public:
  // only for consecutive charactors in ASCII 
  Hash_String(int kind, char base, int length, bool rev_flag = false) {
    Hash_String::kind = kind;
    Hash_String::base = base;
    Hash_String::length = length;
    Hash_String::rev_flag = rev_flag;
    lock = false;
    hash_unit_list.resize(length);
    hash_sum_list.resize(length);
    ll x = 1;
    Loop(i, length) {
      hash_unit_list[i] = x;
      hash_sum_list[i] = x + (i >= 1 ? hash_sum_list[i - 1] : 0);
      x *= kind;
    }
  }
  ll hash(string s) {
    if (!rev_flag) reverse(s.begin(), s.end());
    ll ret = 0;
    Loop(i, s.length()) {
      ret += (s[i] - base + 1) * hash_unit_list[i];
    }
    return ret;
  }
  string unhash(ll x) {
    string ret = "";
    while (x != 0) {
      ret += base + (x - 1) % kind;
      x = (x - 1) / kind;
    }
    if (!rev_flag) reverse(ret.begin(), ret.end());
    return ret;
  }
  ll pop_back(ll x) {
    if (x == 0) return 0;
    if (!rev_flag && !lock) { lock = true; return pop_front(x); }
    lock = false;
    int length = upper_bound(hash_sum_list.begin(), hash_sum_list.end(), x) - hash_sum_list.begin();
    ll y = x % hash_unit_list[length - 1];
    if (y < hash_sum_list[length - 2]) y += hash_unit_list[length - 1];
    return y;
  }
  ll pop_front(ll x) {
    if (x == 0) return 0;
    if (!rev_flag && !lock) { lock = true; return pop_back(x); }
    lock = false;
    return (x - 1) / kind;
  }
  ll push_front(ll x, char c) {
    if (!rev_flag && !lock) { lock = true; return push_back(x, c); }
    lock = false;
    return x * kind + c - base + 1;
  }
  ll push_back(ll x, char c) {
    if (!rev_flag && !lock) { lock = true; return push_front(x, c); }
    lock = false;
    int length = upper_bound(hash_sum_list.begin(), hash_sum_list.end(), x) - hash_sum_list.begin();
    return x + (c - base + 1) * hash_unit_list[length];
  }
  // return hash list of all particular length substrings of s
  vll hash_vec_accumulate(string s, int length) {
    if (!rev_flag) reverse(s.begin(), s.end());
    int n = s.length();
    if (n - length + 1 <= 0) return{};
    vll ret(n - length + 1, 0);
    if (n == 0) return ret;
    ret[0] = hash(s.substr(0, length));
    Loop1(i, ret.size() - 1) {
      ret[i] = pop_front(push_back(ret[i - 1], s[i + length - 1]));
    }
    return ret;
  }
};