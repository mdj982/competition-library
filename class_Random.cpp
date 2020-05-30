#define CF

class Random_Int {
private:
	std::mt19937 *mt;
	std::uniform_int_distribution<> *distr_int;
public:
	// uniform int distribution of [0, m)
	Random_Int(int m) {
		mt = new std::mt19937((uint64_t)(
			std::chrono::duration_cast<std::chrono::nanoseconds>(
				std::chrono::high_resolution_clock::now().time_since_epoch()
				).count()
			));
		distr_int = new std::uniform_int_distribution<>(0, m - 1);
	}
	int get() {
		return (*distr_int)(*mt);
	}
};


class Random_Dynamic_Int {
private:
	std::mt19937 *mt;
    std::uniform_real_distribution<> *distr_real;
public:
	Random_Dynamic_Int() {
		mt = new std::mt19937((uint64_t)(
			std::chrono::duration_cast<std::chrono::nanoseconds>(
				std::chrono::high_resolution_clock::now().time_since_epoch()
				).count()
			));
		distr_real = new std::uniform_real_distribution<>(0, 1);
	}
	~Random_Dynamic_Int() {
		delete mt;
		delete distr_real;
	}
	// uniform int distribution of [l, r), l < r is required
    int get(int l, int r) const {
		int n = r - l;
		double x = (*distr_real)(*mt);
		return l + int(x * n);
    }
};


class Random_Real {
private:
	std::mt19937 *mt;
    std::uniform_real_distribution<> *distr_real;
public:
	// uniform int distribution of [l, r)
	Random_Real(double l, double r) {
		mt = new std::mt19937((uint64_t)(
			std::chrono::duration_cast<std::chrono::nanoseconds>(
				std::chrono::high_resolution_clock::now().time_since_epoch()
				).count()
			));
		distr_real = new std::uniform_real_distribution<>(l, r);
	}
    double get() {
        return (*distr_real)(*mt);
    }
};
