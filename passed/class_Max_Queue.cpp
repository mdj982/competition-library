class Max_Queue {
private:
	stack<Pll> stk0, stk1;
public:
	void push(ll x) {
		ll y = stk1.size() ? stk1.top().second : LLONG_MIN;
		stk1.push({ x, max(x, y) });
	}
	void pop() {
		if (!stk0.size()) {
			while (stk1.size()) {
				ll x = stk1.top().first;
				ll y = stk0.size() ? stk0.top().second : LLONG_MIN;
				stk0.push({ x, max(x, y) });
				stk1.pop();
			}
		}
		stk0.pop();
	}
	size_t size() {
		return stk0.size() + stk1.size();
	}
	void clear() {
		while (stk0.size()) stk0.pop();
		while (stk1.size()) stk1.pop();
	}
	ll get_max() {
		ll x = LLONG_MIN, y = LLONG_MIN;
		if (stk0.size()) x = stk0.top().second;
		if (stk1.size()) y = stk1.top().second;
		return max(x, y);
	}
};