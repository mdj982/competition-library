#include "auto_util_header.hpp"

vi topological_sort(const vvi &lst) {
	vi ret = {};
	int n = lst.size();
	vi cnt(n);
	Loop(a, n) {
		Foreach(b, lst[a]) cnt[b]++;
	}
	set<int> st;
	Loop(a, n) {
		if (cnt[a] == 0) st.insert(a);
	}
	while (st.size()) {
		auto itr = st.begin(); st.erase(itr);
		int a = *itr;
		ret.push_back(a);
		Foreach(b, lst[a]) {
			cnt[b]--;
			if (cnt[b] == 0) st.insert(b);
		}
	}
	if (ret.size() != n) return {};
	else return ret;
}