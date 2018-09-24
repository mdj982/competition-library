typedef ll val_t;

struct graph_t {
	int n;           // |V|, index begins with 0
	int m;           // |E|
	vector<P> edges; // E
	vector<val_t> vals; // V
	vector<ll> costs; // cost or distance
	vector<ll> caps;  // capacity
};

class Maxflow {
private:
	// note: eid of dual_edge is negative
	struct edgedata {
		int eid, from, to;
		ll cap;
		int dual_p;
	};
	struct node {
		int id; bool done; vector<edgedata> edges;
	};
	ll dfs(int a, ll c) {
		if (a == sink) return c;
		nodes[a].done = true;
		Loop(i, nodes[a].edges.size()) {
			int b = nodes[a].edges[i].to;
			if (!nodes[b].done && nodes[a].edges[i].cap > 0) {
				ll df = dfs(b, min(c, nodes[a].edges[i].cap));
				if (df > 0) {
					nodes[a].edges[i].cap -= df;
					nodes[b].edges[nodes[a].edges[i].dual_p].cap += df;
					return df;
				}
			}
		}
		return 0;
	}
	vector<node> nodes;
	int n, m;
	int source, sink;
	edgedata* empty_edge;
public:
	Maxflow(graph_t G, int s, int t) {
		n = G.n;
		m = G.m;
		nodes.resize(n);
		Loop(i, n) nodes[i] = { i, false,{} };
		empty_edge = new edgedata;
		Loop(i, G.edges.size()) {
			int a = G.edges[i].first;
			int b = G.edges[i].second;
			nodes[a].edges.push_back({ i, a, b, G.caps[i], (int)nodes[b].edges.size() });
			nodes[b].edges.push_back({ i - m, b, a, 0, (int)nodes[a].edges.size() - 1 });
		}
		source = s;
		sink = t;
		vector<pair<int, edgedata*>> stk;
		while (dfs(source, LLONG_MAX)) {
			Loop(i, n) nodes[i].done = false;
		}
		return;
	}
	vll get_eid_flow() {
		vll ret(m);
		Loop(i, n) {
			Loopitr(itr, nodes[i].edges) {
				if (itr->eid < 0) ret[itr->eid + m] = itr->cap;
			}
		}
		return ret;
	}
	ll get_maxflow() {
		ll ret = 0;
		Loopitr(itr, nodes[sink].edges) {
			if (itr->eid < 0) ret += itr->cap;
		}
		return ret;
	}
};