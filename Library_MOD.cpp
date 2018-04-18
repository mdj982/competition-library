namespace mod_op {

  const ll MOD = (ll)1e9 + 7;

  class Extended_Euclid {
  private:
    ll m, n;
    vvll mx_multiply(vvll mx1, vvll mx2) {
      vvll ret(mx1.size(), vll(mx2[0].size(), 0));
      Loop(i, mx1.size()) {
        Loop(j, mx1[0].size()) {
          Loop(k, mx2[0].size()) {
            ret[i][j] += mx1[i][k] * mx2[k][j];
          }
        }
      }
      return ret;
    }
  public:
    // solve x, y s.t. mx + ny = gcd(m,n)
    ll x, y, gcd;
    Extended_Euclid(ll M, ll N) {
      m = M;
      n = N;
      vll r(100), k(100);
      bool swapflag = false, m_negflag = false, n_negflag = false;
      if (m < n) { swap(m, n); swapflag = true; }
      if (m < 0) { m *= -1; m_negflag = true; }
      if (n < 0) { n *= -1; n_negflag = true; }
      r[0] = m;
      r[1] = n;
      int h = 1;
      while (1) {
        k[h - 1] = r[h - 1] / r[h];
        r[h + 1] = r[h - 1] % r[h];
        if (r[h + 1] == 0) break;
        h++;
      }
      gcd = r[h];
      vvll mx1 = { { 0, 1 },{ 1, (-1) * k[h - 1] } };
      Loopr(i, h - 1) {
        vvll mx2 = { { 0, 1 },{ 1, (-1) * k[i] } };
        mx1 = mx_multiply(mx1, mx2);
      }
      x = mx1[0][0];
      y = mx1[0][1];
      if (n_negflag) { n *= -1; y *= -1; }
      if (m_negflag) { m *= -1; x *= -1; }
      if (swapflag) { swap(m, n); swap(x, y); }
    }
  };

  class modll {
  private:
    ll mod;
    ll val;
    inline ll modify(ll x) { return x - (x >= 0 ? x : (x - mod + 1)) / mod * mod; }
    inline ll inv(ll x) { if (x == 0) return 1 / x; Extended_Euclid ee(x, -mod); return modify(ee.x); }
  public:
    modll(ll init = 0, ll mod = MOD) { modll::mod = mod; val = modify(init); return; }
    modll(const modll& another) { val = another.val; mod = another.mod; return; }
    inline operator ll() { return val; }
    inline modll& operator=(const modll &another) { val = modify(another.val); return *this; }
    inline modll operator+(const ll &x) { modll ret = modify(val + x); return ret; }
    inline modll operator-(const ll &x) { modll ret = modify(val - x); return ret; }
    inline modll operator*(const ll &x) { modll ret = modify(val * x); return ret; }
    inline modll operator/(const ll &x) { modll ret = modify(val * inv(x)); return ret; }
    inline modll& operator+=(const ll &x) { val = modify(val + x); return *this; }
    inline modll& operator-=(const ll &x) { val = modify(val - x); return *this; }
    inline modll& operator*=(const ll &x) { val = modify(val * x); return *this; }
    inline modll& operator/=(const ll &x) { val = modify(val * inv(x)); return *this; }
    friend inline istream& operator >> (istream &is, modll& x) { is >> x.val; return is; }
    ll get_mod() { return mod; }
  };

  modll pow(modll n, ll p) {
    modll ret;
    if (p == 0) ret = 1;
    else if (p == 1) ret = n;
    else {
      ret = pow(n, p / 2);
      ret *= ret;
      if (p % 2 == 1) ret *= n;
    }
    return ret;
  }

  vector<modll> facts;

  void make_facts(int n) {
    if (facts.empty()) facts.push_back(modll(1));
    for (int i = facts.size(); i <= n; ++i) facts.push_back(modll(facts.back() * (ll)i));
    return;
  }

  //nCr
  modll combination(modll n, ll r) {
    modll ret;
    make_facts((int)n);
    ret = facts[(unsigned)n] / (facts[(unsigned)r] * facts[(unsigned)(n - r)]);
    return ret;
  }

  //a^x=b, if x does not exist, return -1
  ll disc_log(modll a, modll b) {
    ll ret = -1;
    ll m = ceilsqrt(a.get_mod());
    unordered_map<ll, ll> mp;
    modll x = 1;
    Loop(i, m) {
      mp[x] = i;
      x *= a;
    }
    x = 1 / pow(a, m);
    modll k = b;
    Loop(i, m) {
      if (mp.find(k) == mp.end()) k *= x;
      ret = i * m + mp[k];
    }
    return ret;
  }
}

using namespace mod_op;