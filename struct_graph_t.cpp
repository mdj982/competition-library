struct graph_t {
	int n;           // |V|, index begins with 0
	int m;           // |E|
	vector<P> edges; // E
	vector<ll> cost; // cost or distance
	vector<ll> cap;  // capacity
};


graph_t contract(graph_t G, vi u, int cand) {
  if (u.empty()) return G;
  else {
    set<int> st;
    Loop(i, u.size()) st.insert(u[i]);
    graph_t ret = { G.n, G.m,{},{},{} };
    Loop(i, G.edges.size()) {
      bool judge[2];
      judge[0] = (st.find(G.edges[i].first) == st.end());
      judge[1] = (st.find(G.edges[i].second) == st.end());
      if (judge[0]) {
        if (judge[1]) {
          ret.edges.push_back({ G.edges[i].first, G.edges[i].second });
        }
        else {
          ret.edges.push_back({ G.edges[i].first, cand });
        }
      }
      else {
        if (judge[1]) {
          ret.edges.push_back({ cand, G.edges[i].second });
        }
      }
    }
    return ret;
  }
}

// graph input sample
int main() {
	graph_t G;
	cin >> G.n >> G.m;
	Loop(i, G.m) {
		int s, t, c; cin >> s >> t >> c;
		G.edges.push_back({ s, t });
		G.cost.push_back(c);
	}
	return 0;
}