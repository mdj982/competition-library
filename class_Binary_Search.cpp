
class Binary_Search {
private:
  ll l;
  ll r;
  ll m;
public:
  ll result;
  // search [l, r), rule should be [y,...,y,n...,n], and return the index that causes the last 'y'
  Binary_Search(ll range_l, ll range_r, bool(*rule)(ll), bool upper_flag) {
    l = range_l;
    r = range_r;
    while (r - l > 1) {
      m = (l + r) / 2;
      if (rule(m)) l = m;
      else r = m;
    }
    result = upper_flag ? l + 1 : l;
  }
};

namespace Binary_Search_Rules {
  vll a = { 1,3,4,5,5,6,7,8,9 };
  ll x = 5;
  bool bs_rule(ll k) {
    if (a[(int)k] < x) return true;
    else return false;
  }
}

using namespace Binary_Search_Rules;