#include <iostream>
#include <iomanip>
#include <climits>
#include <algorithm>
#include <vector>
#include <deque>
#include <queue>
#include <list>
#include <stack>
#include <string>
#include <functional>
#include <numeric>
#include <map>
#include <set>
#include <cstdlib>
#include <bitset>
#include <unordered_map>
#include <random>
#define _USE_MATH_DEFINES
#include <math.h>
#include <complex>

using namespace std;

#define INF (int)1e8
#define _INF (int)-1e8
#define INFLL (long long)1e15
#define _INFLL (long long)-1e15
#define Loop(i, n) for(int i = 0; i < (int)n; i++)
#define Loop1(i, n) for(int i = 1; i <= (int)n; i++)
#define Loopr(i, n) for(int i = (int)n - 1; i >= 0; i--)
#define Loopr1(i, n) for(int i = (int)n; i >= 1; i--)
#define bitmanip(m,val) static_cast<bitset<(int)m>>(val)
typedef long long int ll;
typedef vector<int> vi;
typedef vector<vector<int>> vvi;
typedef pair<int, int> P;
typedef vector<ll> vll;
typedef vector<vector<ll>> vvll;
typedef vector<double> vd;
typedef vector<vector<double>> vvd;
typedef complex<double> cdouble;
typedef vector<complex<double>> vcd;
typedef vector<vector<complex<double>>> vvcd;

/*******************************************************/

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

void printv(vll a) {
  Loop(i, a.size()) cout << a[i] << endl;
  cout << endl;
}

void printmx(vvll A) {
  Loop(i, A.size()) {
    Loop(j, A[0].size()) {
      cout << A[i][j] << " ";
    }
    cout << endl;
  }
  cout << endl;
}

// ret[i] = a[i] + a[i + 1] + ... (for length times, with looping)
vll loop_vec_accumulate(vll a, ll length) {
  int n = a.size();
  vll ret(n, 0);
  if (n == 0) return ret;
  ll p = length / n;
  if (p > 0) {
    Loop(i, n) ret[0] += a[i];
    ret[0] *= p;
  }
  Loop(i, length % n) ret[0] += a[i];
  Loop1(i, n - 1) {
    ret[i] = ret[i - 1] - a[i - 1] + a[(i + length - 1) % n];
  }
  return ret;
}

vvll loop_mx_accumulate(vvll A, ll i_length, ll j_length) {
  int m = A.size();
  int n = A[0].size();
  Loop(i, m) A[i] = loop_vec_accumulate(A[i], j_length);
  vvll trans_A(n, vll(m, 0));
  Loop(i, n) {
    Loop(j, m) trans_A[i][j] = A[j][i];
  }
  Loop(i, n) trans_A[i] = loop_vec_accumulate(trans_A[i], i_length);
  Loop(i, m) {
    Loop(j, n) A[i][j] = trans_A[j][i];
  }
  return A;
}

vll divisors(ll x) {
  vll ret;
  Loop1(i, x) {
    ll y = (ll)i * i;
    if (y >= x) {
      if (y == x) ret.push_back(i);
      break;
    }
    else {
      if (x % i == 0) {
        ret.push_back(i);
        ret.push_back(x / i);
      }
    }
  }
  return ret;
}

vi list_prime_until(int n) {
  vi ret;
  Loop1(i, n) {
    if (i == 1) continue;
    else if (i == 2) ret.push_back(2);
    else {
      bool judge = true;
      Loop(j, ret.size()) {
        if (i % ret[j] == 0) {
          judge = false;
          break;
        }
      }
      if (judge) ret.push_back(i);
    }
  }
  return ret;
}