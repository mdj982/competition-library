#define MOD (ll)(1e9+7)

ll ceil_div(ll a, ll b) {
  if (a % b == 0) return a / b;
  else return a / b + 1;
}

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
  bool adjust_into_range(ll m, ll n, ll *x, ll *y, Pll x_range = { LLONG_MIN, LLONG_MAX }, Pll y_range = { LLONG_MIN, LLONG_MAX }) {
    if ((*x) < x_range.first && (*y) < y_range.first) return false;
    if ((*x) > x_range.second && (*y) > y_range.second) return false;
    ll d;
    if ((*y) > y_range.second) {
      d = ceil_div((*y) - y_range.second, m);
      (*x) += n * d;
      (*y) -= m * d;
      if ((*x) > x_range.second || (*y) < y_range.first) return false;
    }
    if ((*y) < y_range.first) {
      d = ceil_div(y_range.first - (*y), m);
      (*x) -= n * d;
      (*y) += m * d;
      if ((*x) < x_range.first || (*y) > y_range.second) return false;
    }
    if ((*x) < x_range.first) {
      d = ceil_div(x_range.first - (*x), n);
      (*x) += n * d;
      (*y) -= m * d;
      if ((*x) > x_range.second || (*y) < y_range.first) return false;
    }
    if ((*x) > x_range.second) {
      d = ceil_div((*x) - x_range.second, n);
      (*x) -= n * d;
      (*y) += m * d;
      if ((*x) < x_range.first || (*y) > y_range.second) return false;
    }
    return true;
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
  // solve mx + ny = r, where x is in x_range and y is in y_range inclusively, if impossible then return false
  bool solve_equation(ll r, ll *x, ll *y, Pll x_range = { LLONG_MIN, LLONG_MAX }, Pll y_range = { LLONG_MIN, LLONG_MAX }) {
    if (r % gcd != 0) return false;
    else {
      bool m_negflag = false, n_negflag = false;
      if (m < 0) { m *= -1; this->x *= -1; x_range = { -x_range.second, -x_range.first }; m_negflag = true; }
      if (n < 0) { n *= -1; this->y *= -1; y_range = { -y_range.second, -y_range.first }; n_negflag = true; }
      ll ret_x = this->x * (r / gcd);
      ll ret_y = this->y * (r / gcd);
      bool valid = adjust_into_range(m / gcd, n / gcd, &ret_x, &ret_y, x_range, y_range);
      if (n_negflag) { n *= -1; this->y *= -1; ret_y *= -1; }
      if (m_negflag) { m *= -1; this->x *= -1; ret_x *= -1; }
      if (!valid) return false;
      else {
        *x = ret_x;
        *y = ret_y;
        return true;
      }
    }
  }
};

namespace mod_op {

  //0 ~ mod - 1
  ll modify(ll n) {
    ll ret;
    if (n > 0) ret = n - n / MOD * MOD;
    else ret = n - (n - MOD + 1) / MOD * MOD;
    return ret;
  }

  ll mul(ll x, ll y) {
    return x * y % MOD;
  }

  ll add(ll x, ll y) {
    return (x + y) % MOD;
  }

  ll sub(ll x, ll y) {
    return modify(x - y);
  }

  //A[k] = k!
  vll factlist(ll n) {
    if (n < 0) return{};
    else {
      vll ret((int)n + 1);
      ret[0] = 1;
      Loop1(i, (int)n) ret[i] = mul(ret[i - 1], i);
      return ret;
    }
  }

  //n^p O(log p)
  ll powm(ll n, ll p) {
    if (p == 0) return 1;
    else if (p == 1) return n;
    else {
      ll ans = pow(n, p / 2);
      ans = mul(ans, ans);
      if (p % 2 == 1) ans = mul(ans, n);
      return ans;
    }
  }

  //x^-1
  ll inv(ll n) {
    Extended_Euclid ee(n, MOD*(-1));
    return modify(ee.x);
  }

  //nCr
  ll mod_combination(ll n, ll r) {
    vll facts = factlist(n);
    return mul(facts[n], inv(mul(facts[r], facts[n - r])));
  }
}