#ifndef CLASS_RANDOM
#define CLASS_RANDOM

#include <random>
#include <chrono>
#include <functional>
#include <iostream>

class RandomInt {
private:
	std::mt19937 *mt;
	std::uniform_int_distribution<> *distr_int;
public:
	// uniform int distribution of [0, m)
	RandomInt(const int m) {
		mt = new std::mt19937((uint64_t)(
			std::chrono::duration_cast<std::chrono::nanoseconds>(
				std::chrono::high_resolution_clock::now().time_since_epoch()
				).count()
			));
		distr_int = new std::uniform_int_distribution<>(0, m - 1);
	}
	~RandomInt() {
		delete mt;
		delete distr_int;
	}
	int get() const {
		return (*distr_int)(*mt);
	}
};


class RandomDynamicInt {
private:
	std::mt19937 *mt;
    std::uniform_real_distribution<> *distr_real;
public:
	RandomDynamicInt() {
		mt = new std::mt19937((uint64_t)(
			std::chrono::duration_cast<std::chrono::nanoseconds>(
				std::chrono::high_resolution_clock::now().time_since_epoch()
				).count()
			));
		distr_real = new std::uniform_real_distribution<>(0, 1);
	}
	~RandomDynamicInt() {
		delete mt;
		delete distr_real;
	}
	// uniform int distribution of [l, r), l < r is required
    int get(const int l, const int r) const {
		int n = r - l;
		double x = (*distr_real)(*mt);
		return l + int(x * n);
    }
};

class RandomDynamicSize {
private:
	std::mt19937 *mt;
    std::uniform_real_distribution<> *distr_real;
public:
	RandomDynamicSize() {
		mt = new std::mt19937((uint64_t)(
			std::chrono::duration_cast<std::chrono::nanoseconds>(
				std::chrono::high_resolution_clock::now().time_since_epoch()
				).count()
			));
		distr_real = new std::uniform_real_distribution<>(0, 1);
	}
	~RandomDynamicSize() {
		delete mt;
		delete distr_real;
	}
	// uniform int distribution of [l, r), l < r is required
    size_t get(const size_t l, const size_t r) const {
		size_t n = r - l;
		double x = (*distr_real)(*mt);
		return l + size_t(x * n);
    }
};

class RandomReal {
private:
	std::mt19937 *mt;
    std::uniform_real_distribution<> *distr_real;
public:
	// uniform real distribution of [l, r)
	RandomReal(const double l, const double r) {
		mt = new std::mt19937((uint64_t)(
			std::chrono::duration_cast<std::chrono::nanoseconds>(
				std::chrono::high_resolution_clock::now().time_since_epoch()
				).count()
			));
		distr_real = new std::uniform_real_distribution<>(l, r);
	}
	~RandomReal() {
		delete mt;
		delete distr_real;
	}
    double get() const {
        return (*distr_real)(*mt);
    }
};

class RandomDynamicReal {
private:
	std::mt19937 *mt;
    std::uniform_real_distribution<> *distr_real;
public:
	// uniform real distribution of [l, r)
	RandomDynamicReal() {
		mt = new std::mt19937((uint64_t)(
			std::chrono::duration_cast<std::chrono::nanoseconds>(
				std::chrono::high_resolution_clock::now().time_since_epoch()
				).count()
			));
		distr_real = new std::uniform_real_distribution<>(0, 1);
	}
	~RandomDynamicReal() {
		delete mt;
		delete distr_real;
	}
	// uniform double-float distribution of [l, r], l < r is required
    double get(const double l, const double r) const {
        double x = (*distr_real)(*mt);
		return std::min(l + x * (r - l), r);
    }
};

class RandomNormal {
private:
	std::mt19937 *mt;
    std::normal_distribution<> *distr_normal;
public:
	// normal distribution of (mu, sigma)
	RandomNormal(const double mu, const double sigma) {
		mt = new std::mt19937((uint64_t)(
			std::chrono::duration_cast<std::chrono::nanoseconds>(
				std::chrono::high_resolution_clock::now().time_since_epoch()
				).count()
			));
		distr_normal = new std::normal_distribution<>(mu, sigma);
	}
	~RandomNormal() {
		delete mt;
		delete distr_normal;
	}
    double get() const {
        return (*distr_normal)(*mt);
    }
};

class RandomDynamicNormal {
private:
	std::mt19937 *mt;
    std::normal_distribution<> *distr_normal;
public:
	// normal distribution of (mu, sigma)
	RandomDynamicNormal() {
		mt = new std::mt19937((uint64_t)(
			std::chrono::duration_cast<std::chrono::nanoseconds>(
				std::chrono::high_resolution_clock::now().time_since_epoch()
				).count()
			));
		distr_normal = new std::normal_distribution<>(0.0, 1.0);
	}
	~RandomDynamicNormal() {
		delete mt;
		delete distr_normal;
	}
    double get(const double mu, const double sigma) const {
        double x = (*distr_normal)(*mt);
		return sigma * x + mu;
    }
};

class Selector {
private:
	std::mt19937 *mt;
	std::vector<size_t> st;
public:
	// choose multiple values from [n] uniformly at random
	Selector(const size_t n) {
		mt = new std::mt19937((uint64_t)(
			std::chrono::duration_cast<std::chrono::nanoseconds>(
				std::chrono::high_resolution_clock::now().time_since_epoch()
				).count()
			));
		st.resize(n);
		std::iota(st.begin(), st.end(), 0);
	}
	~Selector() {
		delete mt;
	}
	// choose r values from [n] uniformly at random, r < n is required
    std::vector<size_t> get(const size_t r) const {
        std::vector<size_t> ret;
		std::sample(st.begin(), st.end(), std::back_inserter(ret), r, *mt);
		return ret;
    }
};

class Shuffler {
private:
	std::mt19937 *mt;
public:
	Shuffler() {
		mt = new std::mt19937((uint64_t)(
			std::chrono::duration_cast<std::chrono::nanoseconds>(
				std::chrono::high_resolution_clock::now().time_since_epoch()
				).count()
			));
	}
	~Shuffler() {
		delete mt;
	}
	template <class Iterator>
    void shuffle(Iterator __first, Iterator __last) const {
		std::shuffle(__first, __last, *mt);
    }
};

#endif // CLASS_RANDOM
