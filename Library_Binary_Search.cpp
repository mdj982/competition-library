// range = [l, r), return last value causing "t" in evalfunc that returns l->[t,...,t,f,...,f)->r
// NOTE: if [f,...,f) then return l - 1, if [l, r) = empty set then invalid use
template<typename val_t, typename bsargv_t>
val_t lower_binary_search(val_t l, val_t r, const bsargv_t &bsargv, bool(*evalfunc)(val_t, const bsargv_t&)) {
	if (r - l == 1) {
		if (evalfunc(l, bsargv)) return l;
		else return l - 1;
	}
	val_t m = (l + r) / 2;
	if (evalfunc(m, bsargv)) return lower_binary_search<val_t, bsargv_t>(m, r, bsargv, evalfunc);
	else return lower_binary_search<val_t, bsargv_t>(l, m, bsargv, evalfunc);
}

// range = [l, r), return first value causing "t" in evalfunc that returns l->[f,...,f,t,...,t)->r
// NOTE: if [f,...,f) then return r, if [l, r) = empty set then invalid use
template<typename val_t, typename bsargv_t>
val_t upper_binary_search(val_t l, val_t r, const bsargv_t &bsargv, bool(*evalfunc)(val_t, const bsargv_t&)) {
	if (r - l == 1) {
		if (evalfunc(l, bsargv)) return l;
		else return r;
	}
	val_t m = (l + r) / 2;
	if (evalfunc(m, bsargv)) upper_binary_search<val_t, bsargv_t>(l, m, bsargv, evalfunc);
	else return upper_binary_search<val_t, bsargv_t>(m, r, bsargv, evalfunc);
}

struct bsargv_t {
	//
};

bool evalfunc(int val, const bsargv_t &bsargv) {
	//
	return true;
}