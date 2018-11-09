typedef ll val_t;

struct graph_t {
	int n;           // |V|, index begins with 0
	int m;           // |E|
	vector<P> edges; // E
	vector<val_t> vals; // V
	vector<ll> costs; // cost or distance
	vector<ll> caps;  // capacity
};

class Bellmanford {
private:
	struct node {
		int id; bool done; vi to_eid; vi to; vll cost; int from_eid; int from; ll d;
	};
	vector<node> nodes;
	int n, m, source;
	bool negative_cycle;
public:
	Bellmanford(graph_t G, int start) {
		n = G.n;
		m = G.edges.size();
		nodes.resize(n);
		Loop(i, n) nodes[i] = { i, false,{},{},{}, -1, -1, INFLL };
		Loop(i, m) {
			nodes[G.edges[i].first].to_eid.push_back(i);
			nodes[G.edges[i].first].to.push_back(G.edges[i].second);
			nodes[G.edges[i].first].cost.push_back(G.costs[i]);
		}
		source = start;
		nodes[source].d = 0;
		Loop(k, n) {
			Loop(i, n) {
				node *a = &nodes[i];
				Loop(j, a->to.size()) {
					node *b = &nodes[a->to[j]];
					if (a->d != INFLL && a->d + a->cost[j] < b->d) {
						b->d = a->d + a->cost[j];
						b->from_eid = a->to_eid[j];
						b->from = a->id;
						if (k == n - 1) {
							negative_cycle = true;
							return;
						}
					}
				}
			}
		}
		negative_cycle = false;
		return;
	}
	vi get_path(int v) {
		stack<int> stk;
		stk.push(v);
		int a = v;
		while (nodes[a].from != -1) {
			stk.push(nodes[a].from);
			a = nodes[a].from;
		}
		if (a != source) return{ -1 };
		vi ret;
		while (stk.size()) {
			ret.push_back(stk.top());
			stk.pop();
		}
		return ret;
	}
	vi get_path_eid(int v) {
		stack<int> stk;
		int a = v;
		while (nodes[a].from != -1) {
			stk.push(nodes[a].from_eid);
			a = nodes[a].from;
		}
		if (a != source) return{ -1 };
		vi ret;
		while (stk.size()) {
			ret.push_back(stk.top());
			stk.pop();
		}
		return ret;
	}
	ll get_dist(int v) {
		return nodes[v].d;
	}
	bool is_negative_cycle() {
		return negative_cycle;
	}
};

// bellmanford sample
int main() {
	graph_t G;
	cin >> G.n >> G.m;
	int start; cin >> start;
	Loop(i, G.m) {
		int s, t, c; cin >> s >> t >> c;
		G.edges.push_back({ s, t });
		G.costs.push_back(c);
	}
	Bellmanford bellmanford(G, start);
	if (bellmanford.is_negative_cycle()) {
		cout << "NEGATIVE CYCLE" << endl;
	}
	else {
		Loop(i, G.n) {
			ll ans = bellmanford.get_dist(i);
			if (ans == INFLL) cout << "INF" << endl;
			else cout << ans << endl;
		}
	}
	return 0;
}