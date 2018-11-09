typedef ll val_t;

struct graph_t {
	int n;           // |V|, index begins with 0
	int m;           // |E|
	vector<P> edges; // E
	vector<val_t> vals; // V
	vector<ll> costs; // cost or distance
	vector<ll> caps;  // capacity
};

class _01Bfs {
private:
	struct node {
		int id; bool done; vi to_eid; vi to; vll cost; int from_eid; int from; ll d;
	};
	vector<node> nodes;
	int n, m, source;
public:
	_01Bfs(graph_t G, int start) {
		n = G.n;
		m = G.edges.size();
		nodes.resize(n);
		Loop(i, n) nodes[i] = { i,false,{},{},{}, -1, -1, INFLL };
		Loop(i, m) {
			nodes[G.edges[i].first].to_eid.push_back(i);
			nodes[G.edges[i].first].to.push_back(G.edges[i].second);
			nodes[G.edges[i].first].cost.push_back(G.costs[i]);
		}
		source = start;
		nodes[source].d = 0;
		deque<int> deq;
		deq.push_front(source);
		while (deq.size()) {
			int a = deq.front(); deq.pop_front();
			if (nodes[a].done) continue;
			nodes[a].done = true;
			Loop(j, nodes[a].to.size()) {
				int b = nodes[a].to[j];
				ll buf = nodes[a].d + nodes[a].cost[j];
				if (buf < nodes[b].d) {
					nodes[b].d = buf;
					nodes[b].from_eid = nodes[a].to_eid[j];
					nodes[b].from = nodes[a].id;
					if (nodes[a].cost[j]) deq.push_back(b);
					else deq.push_front(b);
				}
			}
		}
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
};

// _01bfs sample
int main() {
	graph_t G;
	cin >> G.n >> G.m;
	int start; cin >> start;
	Loop(i, G.m) {
		int s, t, c; cin >> s >> t >> c;
		G.edges.push_back({ s, t });
		G.costs.push_back(c);
	}
	_01Bfs _01bfs(G, start);
	Loop(i, G.n) {
		ll ans = _01bfs.get_dist(i);
		if (ans == INFLL) cout << "INF" << endl;
		else cout << ans << endl;
	}
	return 0;
}