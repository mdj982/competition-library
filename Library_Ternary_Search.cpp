#include "auto_util_header.hpp"

// range = [l, r) in integer, return argmin in the convex function
template<class xval_t, class yval_t, class tsargv_t>
xval_t int_tsearch_convex(xval_t l, xval_t r, const tsargv_t &v, yval_t(*evalfunc)(xval_t, const tsargv_t&)) {
	if (r - l <= 2) {
		xval_t x_best = l;
		yval_t y_best = evalfunc(l, v);
		Loop1(i, r - l - 1) {
			xval_t x_buf = l + i;
			yval_t y_buf = evalfunc(x_buf, v);
			if (y_buf < y_best) {
				x_best = x_buf;
				y_best = y_buf;
			}
		}
		return y_best;
	}
	xval_t m0 = (l * 2 + r) / 3;
	xval_t m1 = (l + r * 2) / 3;
	if (evalfunc(m0, v) < evalfunc(m1, v)) return int_tsearch_convex<xval_t, yval_t, tsargv_t>(l, m1, v, evalfunc);
	else return int_tsearch_convex<xval_t, yval_t, tsargv_t>(m0, r, v, evalfunc);
}

// range = [l, r) in real, return argmin in the convex function
template<class xval_t, class yval_t, class tsargv_t>
xval_t real_tsearch_convex(xval_t l, xval_t r, int depth, const tsargv_t &v, yval_t(*evalfunc)(xval_t, const tsargv_t&)) {
	if (depth <= 0) {
		return l;
	}
	xval_t m0 = (l * 2 + r) / 3;
	xval_t m1 = (l + r * 2) / 3;
	if (evalfunc(m0, v) < evalfunc(m1, v)) return real_tsearch_convex<xval_t, yval_t, tsargv_t>(l, m1, depth - 1, v, evalfunc);
	else return real_tsearch_convex<xval_t, yval_t, tsargv_t>(m0, r, depth - 1, v, evalfunc);
}

struct tsargv_t {
	//
};

ll evalfunc(ll val, const tsargv_t &v) {
	//
	return 0;
}