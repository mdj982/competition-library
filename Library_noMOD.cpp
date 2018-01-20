ll powll(ll n, ll p) {
  if (p == 0) return 1;
  else if (p == 1) return n;
  else {
    ll ans = powll(n, p / 2);
    ans = ans * ans;
    if (p % 2 == 1) ans = ans * n;
    return ans;
  }
}

//A[k] = k!
vll factlist(ll n) {
  if (n < 0) return{};
  else {
    vll ret((int)n + 1);
    ret[0] = 1;
    Loop1(i, (int)n) ret[i] = ret[i - 1] * i;
    return ret;
  }
}

//A[n][r] = nCr 
vvll combinationlist(int n) {
  vvll ret(n, vll(n, 1));
  Loop1(i, n - 1) {
    Loop1(j, i - 1) {
      ret[i][j] = ret[i - 1][j - 1] + ret[i - 1][j];
    }
  }
  return ret;
}

ll ceil_div(ll a, ll b) {
  if (a % b == 0) return a / b;
  else return a / b + 1;
}