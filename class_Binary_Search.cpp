template<typename val_t, typename argv_t>
class Binary_Search {
private:
	argv_t argv;
	bool(*evalfunc)(val_t, argv_t);
	val_t search(val_t l, val_t r) {
		if (r - l == 1) return l;
		val_t m = (l + r) / 2;
		if (evalfunc(m, argv)) return search(m, r);
		else return search(l, m);
	}
public:
	val_t result;
	// range = [l, r), return last value causing "t" in evalfunc that returns l->[t,...,t,f,...,f)->r
	Binary_Search(pair<val_t, val_t> range, argv_t argv, bool(*evalfunc)(val_t, argv_t)) {
		Binary_Search::argv = argv;
		Binary_Search::evalfunc = evalfunc;
		result = search(range.first, range.second);
	}
};