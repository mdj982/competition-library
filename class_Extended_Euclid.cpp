#include "auto_util_header.hpp"

class Extended_Euclid {
private:
	bool inrange(ll x, ll y, Pll x_rng, Pll y_rng) {
		if (x_rng.fst <= x && x <= x_rng.snd && y_rng.fst <= y && y <= y_rng.snd) return true;
		else return false;
	}
	bool subst_d(ll &x, ll &y, ll m, ll n, ll g, ll d, Pll x_rng, Pll y_rng) {
		ll xc = x, yc = y;
		Loop(k, 3) {
			x = xc + n / g * (d + k - 1);
			y = yc - m / g * (d + k - 1);
			if (inrange(x, y, x_rng, y_rng)) return true;
		}
		return false;
	}
public:
    // mx + ny = gcd(m, n), runtime error for (m, n) = (0, 0)
    ll ex_euclid(ll m, ll n, ll &x, ll &y) {
        if (n == 0) { x = 1; y = 0; return m; }
        ll g = ex_euclid(n, m % n, y, x);
        y -= m / n * x;
        return g;
    }
    // mx + ny = gcd(m, n), x \in [x_rng.fst, x_rng.snd], y \in [y_rng.fst, y_rng.snd]
    // TIPS: x can move at intervals of n / gcd(m, n), y can move at intervals of m / gcd(m, n)
	bool solve(ll m, ll n, ll &x, ll &y, ll z, Pll x_rng = { LLONG_MIN, LLONG_MAX }, Pll y_rng = { LLONG_MIN, LLONG_MAX }) {
		ll g = gcd(m, n);
        if (z % g != 0) return false;
		ex_euclid(m, n, x, y);
        ll q = z / g;
        x *= q;
        y *= q;
        if (inrange(x, y, x_rng, y_rng)) return true;
        if (x_rng.fst != LLONG_MIN) {
            ll d = (x_rng.fst - x) / (n / g);
            if (subst_d(x, y, m, n, g, d, x_rng, y_rng)) return true;
        }
        if (x_rng.snd != LLONG_MAX) {
            ll d = (x_rng.snd - x) / (n / g);
            if (subst_d(x, y, m, n, g, d, x_rng, y_rng)) return true;
        }
        if (y_rng.fst != LLONG_MIN) {
            ll d = (y_rng.fst - y) / (m / g);
            if (subst_d(x, y, m, n, g, -d, x_rng, y_rng)) return true;
        }
        if (x_rng.snd != LLONG_MAX) {
            ll d = (y_rng.snd - y) / (m / g);
            if (subst_d(x, y, m, n, g, -d, x_rng, y_rng)) return true;
        }
        return false;
	}
};