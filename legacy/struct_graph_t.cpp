typedef ll val_t;

struct graph_t {
	int n;           // |V|, index begins with 0
	int m;           // |E|
	vector<P> edges; // E
	vector<val_t> vals; // V
	vector<ll> costs; // cost or distance
	vector<ll> caps;  // capacity
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

class Graph {
private:
	struct node {
		int id; val_t val; bool done; vi to_eid; vi to;
	};
	vector<node> nodes;
	int n;
public:
	Graph(graph_t G) {
		n = G.n;
		nodes.resize(n);
		Loop(i, n) nodes[i] = { i,G.vals[i],false,{},{} };
		Loop(i, G.edges.size()) {
			nodes[G.edges[i].first].to_eid.push_back(i);
			nodes[G.edges[i].first].to.push_back(G.edges[i].second);
		}
		return;
	}
};


// graph input sample
int main() {
	graph_t G;
	cin >> G.n >> G.m;
	Loop(i, G.m) {
		int s, t, c; cin >> s >> t >> c;
		G.edges.push_back({ s, t });
		G.costs.push_back(c);
	}
	return 0;
}