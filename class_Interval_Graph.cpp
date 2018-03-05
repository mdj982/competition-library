
struct interval_graph_t {
  int n;           // |I|, index begins with 0
  vector<Pll> intervals; // I
  vll weight; // cost or weight
};

class Interval_Graph {
private:
  struct node {
    int id;
    ll l, r, weight; // interval of [l, r], l may be equal to r
    bool operator<(const node & another) const {
      return (r != another.r ? r < another.r : l < another.l);
    }
  };
  struct clique_record_t {
    ll l, r, value;
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
      range.first = min(range.first, IG.intervals[i].first);
      range.second = max(range.second, IG.intervals[i].second);
    }
    if (IG.weight.size()) {
      Loop(i, n) nodes[i].weight = IG.weight[i];
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
  vi get_max_weight_clique() {
    if (max_clique[WEIGHT].second == -1) solve_max_clique(WEIGHT);
    return max_clique[WEIGHT].first;
  }
  ll get_weight_of_max_weight_clique() {
    if (max_clique[WEIGHT].second == -1) solve_max_clique(WEIGHT);
    return max_clique[WEIGHT].second;
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
};