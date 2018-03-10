using namespace mod_op;

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
      hash_sum_list[i] = add(x, (i >= 1 ? hash_sum_list[i - 1] : 0));
      x = mul(x, kind);
    }
  }
  ll hash(string s) {
    ll ret = 0;
    Loop(i, s.length()) {
      ret = add(ret, mul((s[i] - base + 1), hash_unit_list[i]));
    }
    return ret;
  }
  ll pop_back_on_hash(ll x, char c, int length) {
    return sub(x, mul((c - base + 1), hash_unit_list[length - 1]));
  }
  ll pop_front_on_hash(ll x, char c) {
    return sub(x, c - base + 1);
  }
  ll push_front_on_hash(ll x, char c) {
    return add(mul(x, kind), c - base + 1);
  }
  ll push_back_on_hash(ll x, char c, int length) {
    return add(x, mul(c - base + 1, hash_unit_list[length]));
  }
  // return hash list of all particular length substrings of s
  vll hash_vec_accumulate(string s, int length) {
    int n = s.length();
    if (n - length + 1 <= 0) return{};
    vll ret(n - length + 1, 0);
    if (n == 0) return ret;
    ret[0] = hash(s.substr(0, length));
    Loop1(i, ret.size() - 1) {
      ret[i] = pop_front_on_hash(push_back_on_hash(ret[i - 1], s[i + length - 1], length), s[i - 1]);
    }
    return ret;
  }
};