#ifndef CLASS_TIMESTAMP_HPP
#define CLASS_TIMESTAMP_HPP

#include <iostream>
#include <chrono>

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

#endif // CLASS_TIMESTAMP_HPP