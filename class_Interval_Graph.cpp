
struct interval_graph_t {
	int n;           // |I|, index begins with 0
	vector<Pll> intervals; // I
	vll weights; // cost or weight
};

class Interval_Graph {
private:
	struct node {
		int id;
		ll l, r, weight; // interval of [l, r], l may be equal to r
		bool operator<(const node & another) const {
			return (r != another.r ? r < another.r : (l != another.l ? l < another.l : id < another.id));
		}
	};
	struct clique_record_t {
		ll l, r, value;
		bool operator<(const clique_record_t & another) const {
			return (l != another.l ? l < another.l : (r != another.r ? r < another.r : value < another.value));
		}
	};
	struct stableset_record_t {
		int id;
		ll r, value;
		bool operator<(const stableset_record_t & another) const {
			return (r != another.r ? r > another.r : (value != another.value ? value > another.value : id < another.id));
		}
	};
	enum exist_t { BEGIN, END };
	enum value_t { WEIGHT, NUM };
	vector<node> nodes;
	int n;
	Pll range;
	vll ranges_l, ranges_r;
	map<pair<ll, exist_t>, vi> mp; // at point <ll>, nodes[<int>] begins or ends
	vector<clique_record_t> clique_record[2];
	set<stableset_record_t> max_stableset_record[2];
	pair<vi, ll> max_clique[2], max_stableset[2];
	void update_clique_record(value_t valuetype, ll event_point, ll value) {
		if (clique_record[valuetype].size() != 0) {
			if (clique_record[valuetype].back().l == event_point) {
				clique_record[valuetype].back().value = value;
			}
			else {
				clique_record[valuetype].back().r = event_point - 1;
				if (event_point <= range.second) clique_record[valuetype].push_back({ event_point, range.second, value });
			}
		}
		else {
			if (event_point <= range.second) clique_record[valuetype].push_back({ event_point, range.second, value });
		}
	}
	void solve_clique(value_t valuetype) {
		ll recent_value = 0;
		for (auto itr = mp.begin(); itr != mp.end(); itr++) {
			Loop(i, itr->second.size()) {
				ll change = (valuetype == WEIGHT) ? nodes[itr->second[i]].weight : 1;
				recent_value += (itr->first.second == BEGIN) ? change : -change;
			}
			update_clique_record(valuetype, itr->first.first + (itr->first.second == END ? 1 : 0), recent_value);
		}
	}
	void solve_max_clique(value_t valuetype) {
		if (clique_record[valuetype].empty()) solve_clique(valuetype);
		ll max_point = -1;
		ll max_value = 0;
		Loop(i, clique_record[valuetype].size()) {
			if (clique_record[valuetype][i].value > max_value) {
				max_point = clique_record[valuetype][i].l;
				max_value = clique_record[valuetype][i].value;
			}
		}
		Loop(i, n) {
			if (nodes[i].l <= max_point && max_point <= nodes[i].r) {
				max_clique[valuetype].first.push_back(i);
			}
		}
		max_clique[valuetype].second = max_value;
	}
	void solve_max_stableset(value_t valuetype) {
		vector<node> nodesbuf = nodes;
		sort(nodesbuf.begin(), nodesbuf.end());
		vi parents(n, -1);
		max_stableset_record[valuetype].insert({ -1, LLONG_MIN, 0 });
		int focus_id = -1;
		ll max_value = 0;
		Loop(i, n) {
			stableset_record_t find_target = { -1, nodesbuf[i].l, -1 };
			auto itr = lower_bound(max_stableset_record[valuetype].begin(), max_stableset_record[valuetype].end(), find_target);
			ll recent_value = itr->value + ((valuetype == WEIGHT) ? nodesbuf[i].weight : 1);
			parents[nodesbuf[i].id] = itr->id;
			if (recent_value > max_value) {
				focus_id = nodesbuf[i].id;
				max_value = recent_value;
				max_stableset_record[valuetype].insert({ nodesbuf[i].id, nodesbuf[i].r, recent_value });
			}
		}
		while (focus_id != -1) {
			max_stableset[valuetype].first.push_back(focus_id);
			focus_id = parents[focus_id];
		}
		sort(max_stableset[valuetype].first.begin(), max_stableset[valuetype].first.end());
		max_stableset[valuetype].second = max_value;
	}
public:
	Interval_Graph(interval_graph_t IG) {
		n = IG.n;
		nodes.resize(n);
		range = { LLONG_MAX, LLONG_MIN };
		Loop(i, n) {
			nodes[i] = { i, IG.intervals[i].first, IG.intervals[i].second, 1 };
			mp[{IG.intervals[i].first, BEGIN}].push_back(i);
			mp[{IG.intervals[i].second, END}].push_back(i);
			ranges_l.push_back(IG.intervals[i].first);
			ranges_r.push_back(IG.intervals[i].second);
		}
		sort(ranges_l.begin(), ranges_l.end());
		sort(ranges_r.begin(), ranges_r.end());
		if (n > 0) range.first = ranges_l[0];
		if (n > 0) range.second = ranges_r[n - 1];
		if (IG.weights.size()) {
			Loop(i, n) nodes[i].weight = IG.weights[i];
		}
		Loop(i, 2) clique_record[i] = {};
		Loop(i, 2) max_clique[i] = { {}, -1 };
		Loop(i, 2) max_stableset[i] = { {}, -1 };
	}
	vi get_max_clique() {
		if (max_clique[NUM].second == -1) solve_max_clique(NUM);
		return max_clique[NUM].first;
	}
	ll get_num_of_max_clique() {
		if (max_clique[NUM].second == -1) solve_max_clique(NUM);
		return max_clique[NUM].second;
	}
	// range [l, r] interval must be less than memory capacity 
	vll count_up_clique_num(Pll range) {
		vll ret((int)(range.second - range.first) + 1, 0);
		if (clique_record[NUM].empty()) solve_clique(NUM);
		clique_record_t target = { range.first, range.first, 0 };
		auto itr = lower_bound(clique_record[NUM].begin(), clique_record[NUM].end(), target);
		if (itr == clique_record[NUM].end()) return ret;
		else {
			for (ll k = range.first; k <= range.second; k++) {
				if (k < itr->l) continue;
				if (itr->r < k) {
					itr++;
					if (itr == clique_record[NUM].end()) break;
				}
				ret[(int)(k - range.first)] = itr->value;
			}
			return ret;
		}
	}
	vi get_max_weight_clique() {
		if (max_clique[WEIGHT].second == -1) solve_max_clique(WEIGHT);
		return max_clique[WEIGHT].first;
	}
	ll get_weight_of_max_weight_clique() {
		if (max_clique[WEIGHT].second == -1) solve_max_clique(WEIGHT);
		return max_clique[WEIGHT].second;
	}
	// range [l, r] interval must be less than memory capacity 
	vll count_up_clique_weight(Pll range) {
		vll ret((int)(range.second - range.first + 1), 0);
		if (clique_record[WEIGHT].empty()) solve_clique(WEIGHT);
		clique_record_t target = { range.first, range.first, 0 };
		auto itr = lower_bound(clique_record[WEIGHT].begin(), clique_record[WEIGHT].end(), target);
		if (itr == clique_record[WEIGHT].end()) return ret;
		else {
			for (ll k = range.first; k <= range.second; k++) {
				if (k < itr->l) continue;
				if (itr->r < k) {
					itr++;
					if (itr == clique_record[WEIGHT].end()) break;
				}
				ret[(int)(k - range.first)] = itr->value;
			}
			return ret;
		}
	}
	vi get_max_stableset() {
		if (max_stableset[NUM].second == -1) solve_max_stableset(NUM);
		return max_stableset[NUM].first;
	}
	ll get_num_of_max_stableset() {
		if (max_stableset[NUM].second == -1) solve_max_stableset(NUM);
		return max_stableset[NUM].second;
	}
	vi get_max_weight_stableset() {
		if (max_stableset[WEIGHT].second == -1) solve_max_stableset(WEIGHT);
		return max_stableset[WEIGHT].first;
	}
	ll get_weight_of_max_weight_stableset() {
		if (max_stableset[WEIGHT].second == -1) solve_max_stableset(WEIGHT);
		return max_stableset[WEIGHT].second;
	}
	// range[l, r], O(mlogm) where m is number of iteration
	ll get_num_of_subset(Pll range) {
		ll ret = 0;
		if (range.first <= range.second) {
			set<int> st;
			auto itr = mp.lower_bound({ range.first, BEGIN });
			while (itr != mp.end() && itr->first.first <= range.second) {
				if (itr->first.second == BEGIN) {
					Loop(i, itr->second.size()) st.insert(itr->second[i]);
				}
				else {
					Loop(i, itr->second.size()) if (st.find(itr->second[i]) != st.end()) ret++;
				}
				itr++;
			}
		}
		return ret;
	}
	// range[l, r], O(mlogm) where m is number of iteration
	ll get_num_of_superset(Pll range) {
		int index_l = upper_bound(ranges_l.begin(), ranges_l.end(), range.first) - ranges_l.begin();
		int index_r = lower_bound(ranges_r.begin(), ranges_r.end(), range.second) - ranges_r.begin();
		ll ret = ((index_l)+(n - index_r) - (index_r)-(n - index_l)) / 2 + get_num_of_subset({ range.first + 1, range.second - 1 });
		return ret;
	}
};