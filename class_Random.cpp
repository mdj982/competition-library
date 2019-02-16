class Random {
private:
	mt19937 *mt;
	uniform_int_distribution<> *distr;
public:
	// uniform int distribution of [0, m)
	Random(int m) {
		mt = new mt19937(unsigned(time(NULL)));
		distr = new uniform_int_distribution<>(0, m - 1);
	}
	int get() {
		return (*distr)(*mt);
	}
};