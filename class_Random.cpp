#define CF

class Random_Int {
private:
	mt19937 *mt;
	uniform_int_distribution<> *distr_int;
public:
	// uniform int distribution of [0, m)
	Random_Int(int m) {
		mt = new std::mt19937((uint64_t)(
			std::chrono::duration_cast<chrono::nanoseconds>(
				chrono::high_resolution_clock::now().time_since_epoch()
				).count()
			));
		distr_int = new uniform_int_distribution<>(0, m - 1);
	}
	int get() {
		return (*distr_int)(*mt);
	}
};

class Random_Real {
private:
	mt19937 *mt;
    uniform_real_distribution<> *distr_real;
public:
	// uniform int distribution of [l, r)
	Random_Real(double l, double r) {
		mt = new std::mt19937((uint64_t)(
			std::chrono::duration_cast<chrono::nanoseconds>(
				chrono::high_resolution_clock::now().time_since_epoch()
				).count()
			));
		distr_real = new uniform_real_distribution<>(l, r);
	}
    double get() {
        return (*distr_real)(*mt);
    }
};