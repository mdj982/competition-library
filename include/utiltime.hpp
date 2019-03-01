#include <iostream>
#include <chrono>

enum timestamp_t {
	BEGIN, END, PAUSE
};

namespace timeval {
	std::chrono::system_clock::time_point start, end;
	timestamp_t mode = END;
	double elapsed = 0;
}

void timestamp(timestamp_t x) {
	switch (x) {
	case BEGIN:
		if (timeval::mode == BEGIN) {
			std::cout << "timestamp mode error with BEGIN -> BEGIN" << std::endl;
			exit(EXIT_SUCCESS);
		}
		timeval::mode = BEGIN;
		timeval::start = std::chrono::system_clock::now();
		break;
	case PAUSE:
		timeval::end = std::chrono::system_clock::now();
		if (timeval::mode == PAUSE) {
			std::cout << "timestamp mode error with PAUSE -> PAUSE" << std::endl;
			exit(EXIT_SUCCESS);
		}
		if (timeval::mode == END) {
			std::cout << "timestamp mode error with END -> PAUSE" << std::endl;
			exit(EXIT_SUCCESS);
		}
		timeval::elapsed += (double)std::chrono::duration_cast<std::chrono::nanoseconds>(timeval::end - timeval::start).count();
		timeval::mode = PAUSE;
		break;
	case END:
		timeval::end = std::chrono::system_clock::now();
		if (timeval::mode == END) {
			std::cout << "timestamp mode error with END -> END" << std::endl;
			exit(EXIT_SUCCESS);
		}
		if (timeval::mode == PAUSE) {
			std::cout << "elapsed time: " << timeval::elapsed / 1e6 << " ms" << std::endl;
			timeval::elapsed = 0;
		}
		else {
			timeval::elapsed += (double)std::chrono::duration_cast<std::chrono::nanoseconds>(timeval::end - timeval::start).count();
			std::cout << "elapsed time: " << timeval::elapsed / 1e6 << " ms" << std::endl;
			timeval::elapsed = 0;
		}
		timeval::mode = END;
		break;
	}
}