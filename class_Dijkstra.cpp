typedef ll val_t;

struct graph_t {
	int n;           // |V|, index begins with 0
	int m;           // |E|
	vector<P> edges; // E
	vector<val_t> vals; // V
	vector<ll> costs; // cost or distance
	vector<ll> caps;  // capacity
};

class Dijkstra {
private:
	struct node {
		int id; bool done; vi to_eid; vi to; vll costs; int from_eid; int from; ll d;
		bool operator<(const node & another) const {
			return d != another.d ? d > another.d : id > another.id;
		}
	};
	struct pq_t {
		int id; ll d;
		bool operator<(const pq_t & another) const {
			return d != another.d ? d > another.d : id > another.id;
		}
	};
	vector<node> nodes;
	int n, m, source;
public:
	Dijkstra(graph_t G, int start) {
		n = G.n;
		m = G.edges.size();
		nodes.resize(n);
		Loop(i, n) nodes[i] = { i, false,{},{},{}, -1, -1, LLONG_MAX };
		Loop(i, m) {
			nodes[G.edges[i].first].to_eid.push_back(i);
			nodes[G.edges[i].first].to.push_back(G.edges[i].second);
			nodes[G.edges[i].first].costs.push_back(G.costs[i]);
			nodes[G.edges[i].second].to_eid.push_back(i);
			nodes[G.edges[i].second].to.push_back(G.edges[i].first);
			nodes[G.edges[i].second].costs.push_back(G.costs[i]);
		}
		source = start;
		nodes[source].d = 0;
		priority_queue<pq_t> pq;
		pq.push({ nodes[source].id, nodes[source].d });
		while (pq.size()) {
			node *a = &nodes[pq.top().id];
			pq.pop();
			if (nodes[a->id].done) continue;
			nodes[a->id].done = true;
			Loop(j, a->to.size()) {
				node *b = &nodes[a->to[j]];
				if (b->done) continue;
				ll buf = a->d + a->costs[j];
				if (buf < b->d) {
					b->d = buf;
					b->from_eid = a->to_eid[j];
					b->from = a->id;
					pq.push({ b->id, b->d });
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
		if (a != source) return {};
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
		if (a != source) return {};
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


// dijkstra sample
int main() {
	graph_t G;
	cin >> G.n >> G.m;
	int start; cin >> start;
	Loop(i, G.m) {
		int s, t, c; cin >> s >> t >> c;
		G.edges.push_back({ s, t });
		G.costs.push_back(c);
	}
	Dijkstra dijkstra(G, start);
	Loop(i, G.n) {
		ll ans = dijkstra.get_dist(i);
		if (ans == LLONG_MAX) cout << "INF" << endl;
		else cout << ans << endl;
	}
	return 0;
}