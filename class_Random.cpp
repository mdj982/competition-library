class Random {
private:
	random_device *rnd;
	mt19937 *mt;
	uniform_int_distribution<> *distr;
public:
	// uniform int distribution of [0, m)
	Random(int m) {
		rnd = new random_device;
		mt = new mt19937((*rnd)());
		distr = new uniform_int_distribution<>(0, m - 1);
	}
	int get_rand() {
		return (*distr)(*mt);
	}
};