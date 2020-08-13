#include "auto_util_header.hpp"

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
	~Random_Int() {
		delete mt;
		delete distr_int;
	}
	int get() const {
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
	~Random_Real() {
		delete mt;
		delete distr_real;
	}
	double get() const {
		return (*distr_real)(*mt);
	}
};

class Random_Dynamic_Real {
private:
	std::mt19937 *mt;
	std::uniform_real_distribution<> *distr_real;
public:
	// uniform int distribution of [l, r)
	Random_Dynamic_Real() {
		mt = new std::mt19937((uint64_t)(
			std::chrono::duration_cast<std::chrono::nanoseconds>(
				std::chrono::high_resolution_clock::now().time_since_epoch()
				).count()
			));
		distr_real = new std::uniform_real_distribution<>(0, 1);
	}
	~Random_Dynamic_Real() {
		delete mt;
		delete distr_real;
	}
	// uniform double-float distribution of [l, r], l < r is required
	double get(double l, double r) const {
		double x = (*distr_real)(*mt);
		return std::min(l + x * (r - l), r);
	}
};

class Sampler {
private:
	Random_Real *rr;
	std::vector<double> a;
	double x_min;
	double x_max;
	double x_interval;
	double val_integral;
public:
	Sampler(std::function<double(double)> f, double x_min, double x_max, size_t N_INTERVAL = 100) {
		this->rr = new Random_Real(0, 1);
		this->a.resize(N_INTERVAL);
		assert(x_min <= x_max);
		assert(N_INTERVAL);
		this->x_min = x_min;
		this->x_max = x_max;
		this->x_interval = (x_max - x_min) / N_INTERVAL;
		// a: f(x)
		for (size_t i = 0; i < N_INTERVAL; ++i) {
			this->a[i] = f(x_min + x_interval * (i + 0.5));
		}
		// a \propto \int_{x_min}^{x} f(k)dk
		for (size_t i = 1; i < N_INTERVAL; ++i) {
			this->a[i] += this->a[i - 1];
		}
		this->val_integral = this->a[N_INTERVAL - 1] * this->x_interval;
		// normalize
		for (size_t i = 0; i + 1 < N_INTERVAL; ++i) {
			this->a[i] /= this->a[N_INTERVAL - 1];
		}
		// make sure that lowerbound of [0, 1) is in the vector
		this->a[N_INTERVAL - 1] = 1;
	}
	~Sampler() {
		delete rr;
	}
	double get() const {
		double u0 = rr->get();
		double u1 = rr->get();
		size_t id = std::lower_bound(a.begin(), a.end(), u0) - a.begin();
		double x = x_min + x_interval * (id + u1);
		x = std::min(x, x_max);
		x = std::max(x, x_min);
		return x;
	}
	double get_integral() const {
		return this->val_integral;
	}
};
