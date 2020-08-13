#include "auto_util_header.hpp"

class Max_Queue {
private:
	vector<Pll> stk0, stk1;
public:
	void push(ll x) {
		ll y = stk1.size() ? stk1.back().second : LLONG_MIN;
		stk1.push_back({ x, max(x, y) });
	}
	void pop() {
		if (!stk0.size()) {
			while (stk1.size()) {
				ll x = stk1.back().first;
				ll y = stk0.size() ? stk0.back().second : LLONG_MIN;
				stk0.push_back({ x, max(x, y) });
				stk1.pop_back();
			}
		}
		stk0.pop_back();
	}
	size_t size() {
		return stk0.size() + stk1.size();
	}
	void clear() {
		while (stk0.size()) stk0.pop_back();
		while (stk1.size()) stk1.pop_back();
	}
	ll get_max() {
		ll x = LLONG_MIN, y = LLONG_MIN;
		if (stk0.size()) x = stk0.back().second;
		if (stk1.size()) y = stk1.back().second;
		return max(x, y);
	}
};