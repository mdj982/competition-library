#include <bits/stdc++.h>

auto partialbind = [](auto f, auto&& x)
{
	if constexpr (std::is_lvalue_reference<decltype(x)>{}) {
		return [f, &x](auto&& ... args) mutable {
			return f(x, args...);
		};
	}
	else {
		return [f, x = std::forward<decltype(x)>(x)](auto&& ... args) mutable {
			return f(x, args...);
		};
	}
};

/*
void output_and_inc_arg1_arg5(int &x, const int y, const std::vector<int> &a, const int &z, int* p, const std::string &s) {
	std::cout << s << " " << x << " " << y << " " << a[0] << " " << z << " " << *p << " " << std::endl;
	x++;
	(*p)++;
}

void test_output_and_inc_arg1_arg5() {
	int x = 13;
	int y = 7;
	std::vector<int> a(30000000);
	std::iota(a.begin(), a.end(), 1000);
	int z = 23;
	int p = 31;
	//
	auto c1 = partialbind(output_and_inc_arg1_arg5, x);
	auto c2 = partialbind(c1, y);
	auto c3 = partialbind(c2, a);
	auto c4 = partialbind(c3, z);
	auto c5 = partialbind(c4, &p);
	//
	output_and_inc_arg1_arg5(x, 7, a, 23, &p, "expected(13, 7, 1000, 23, 31): ");
	c1(y, a, 23, &p, "expected(14, 7, 1000, 23, 32): ");
	c2(a, z, &p, "expected(15, 7, 1000, 23, 33): ");
	c3(z, &p, "expected(16, 7, 1000, 23, 34): ");
	c4(&p, "expected(17, 7, 1000, 23, 35): ");
	c5("expected(18, 7, 1000, 23, 36): ");
	//
	auto c6 = partialbind(c5, "expected(19, 7, 1000, 23, 37): ");
	c6();
}

class Timestamp {
private:
	enum timestamp_t {
		BEGIN, END, PAUSE
	};
	std::chrono::system_clock::time_point start, finish;
	timestamp_t mode;
	double elapsed;
public:
	Timestamp() {
		mode = END;
		elapsed = 0;
	}
	void begin() {
		if (mode == BEGIN) {
			std::cout << "timestamp mode error with BEGIN -> BEGIN" << std::endl;
			exit(EXIT_FAILURE);
		}
		mode = BEGIN;
		start = std::chrono::system_clock::now();
	}
	void pause() {
		finish = std::chrono::system_clock::now();
		if (mode == PAUSE) {
			std::cout << "timestamp mode error with PAUSE -> PAUSE" << std::endl;
			exit(EXIT_FAILURE);
		}
		if (mode == END) {
			std::cout << "timestamp mode error with END -> PAUSE" << std::endl;
			exit(EXIT_FAILURE);
		}
		elapsed += double(std::chrono::duration_cast<std::chrono::nanoseconds>(finish - start).count());
		mode = PAUSE;
	}
	double end() {
		finish = std::chrono::system_clock::now();
		double ret = 0;
		if (mode == END) {
			std::cout << "timestamp mode error with END -> END" << std::endl;
			exit(EXIT_FAILURE);
		}
		if (mode == PAUSE) {
			ret = elapsed / 1e6;
			elapsed = 0;
		}
		else {
			elapsed += double(std::chrono::duration_cast<std::chrono::nanoseconds>(finish - start).count());
			ret = elapsed / 1e6;
			elapsed = 0;
		}
		mode = END;
		return ret;
	}
    double check() {
        if (mode == BEGIN) {
	        std::chrono::system_clock::time_point cur = std::chrono::system_clock::now();
            return double(std::chrono::duration_cast<std::chrono::nanoseconds>(cur - start).count()) / 1e6;
		}
        else {
			return elapsed / 1e6;
		}
    }
};

int binary_search1(const std::vector<int> &a, const int x) {
	return std::lower_bound(a.begin(), a.end(), x) - a.begin();
}

int binary_search2(const int x, const std::vector<int> &a) {
	return std::lower_bound(a.begin(), a.end(), x) - a.begin();
}

void test_binary_search() {
	std::vector<int> a(10000000);
	std::iota(a.begin(), a.end(), 0);
	long long int ref_1 = 0;
	long long int ans_1 = 0;
	long long int ref_2 = 0;
	long long int ans_2 = 0;
	Timestamp *ts = new Timestamp();
	ts->begin();
	for (int i = 0; i < int(a.size()); ++i) {
		ref_1 += binary_search1(a, i);
	}
	std::cout << ts->end() << " ms, sum = " << ref_1 << std::endl;
	ts->begin();
	for (int i = 0; i < int(a.size()); ++i) {
		auto c = partialbind(binary_search1, a);
		ans_1 += c(i);
	}
	std::cout << ts->end() << " ms, sum = " << ans_1 << std::endl;
	ts->begin();
	for (int i = 0; i < int(a.size()); ++i) {
		ref_2 += binary_search2(i, a);
	}
	std::cout << ts->end() << " ms, sum = " << ref_2 << std::endl;
	ts->begin();
	for (int i = 0; i < int(a.size()); ++i) {
		auto c = partialbind(binary_search2, i);
		ans_2 += c(a);
	}
	std::cout << ts->end() << " ms, sum = " << ans_2 << std::endl;
}

int main()
{
	test_output_and_inc_arg1_arg5();
	test_binary_search();
}
*/
