class Hash_String {
private:
  int kind, length;
  char base;
  vll hash_unit_list;
  vll hash_sum_list;
public:
  // only for consecutive charactors in ASCII 
  Hash_String(int kind, char base, int length) {
    Hash_String::kind = kind;
    Hash_String::base = base;
    Hash_String::length = length;
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
    return ret;
  }
  ll pop_back_on_hash(ll x, int length = -1) {
    if (length == -1) length = upper_bound(hash_sum_list.begin(), hash_sum_list.end(), x) - hash_sum_list.begin();
    if (length <= 1) return 0;
    else {
      ll y = x % hash_unit_list[length - 1];
      if (y < hash_sum_list[length - 2]) y += hash_unit_list[length - 1];
      return y;
    }
  }
  ll pop_front_on_hash(ll x) {
    return (x - 1) / kind;
  }
  ll push_front_on_hash(ll x, char c) {
    return x * kind + c - base + 1;
  }
  ll push_back_on_hash(ll x, char c, int length = -1) {
    if (length == -1) length = upper_bound(hash_sum_list.begin(), hash_sum_list.end(), x) - hash_sum_list.begin();
    return x + (c - base + 1) * hash_unit_list[length];
  }
  // return hash list of all particular length substrings of s
  vll hash_vec_accumulate(string s, int length) {
    int n = s.length();
    if (n - length + 1 <= 0) return{};
    vll ret(n - length + 1, 0);
    if (n == 0) return ret;
    ret[0] = hash(s.substr(0, length));
    Loop1(i, ret.size() - 1) {
      ret[i] = pop_front_on_hash(push_back_on_hash(ret[i - 1], s[i + length - 1], length));
    }
    return ret;
  }
};