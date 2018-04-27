template<typename val_t, typename dat_t>
class Binary_Search {
private:
	dat_t dat;
	bool reverse_flag;
	bool(*evalfunc)(val_t, dat_t);
	ll search(val_t l, val_t r) {
		if (abs(r - l) == 1) {
			if (!evalfunc(l, dat)) return r;
			else return l;
		}
		val_t m = reverse_flag ? ceildiv(l + r, 2) : ((l + r) / 2);
		if (!evalfunc(m, dat)) return search(m, r);
		else return search(l, m);
	}
public:
	ll result;
	// range = [l, r), return first value causing "t" in evalfunc that returns l->[f,...,f,t,...,t)->r
	Binary_Search(pair<val_t, val_t> range, dat_t dat, bool(*evalfunc)(val_t, dat_t)) {
		if (range.first > range.second) reverse_flag = true;
		Binary_Search::dat = dat;
		Binary_Search::evalfunc = evalfunc;
		result = search(range.first, range.second);
	}
};

struct dat_t {
	ll n, k, m, d;
};

bool evalfunc(ll x, dat_t dat) {
	if ((dat.d - 1) * dat.k + 1 <= dat.n / x) return true;
	else return false;
}