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
	struct edge {
		int eid, from, to;
		ll cap;
	};
	struct node {
		int id; bool done; vector<int> to_eids;
	};
	int dual_eid(int eid) {
		if (eid < m) return eid + m;
		else return eid - m;
	}
	ll dfs(int a, ll f) {
		if (a == sink) return f;
		nodes[a].done = true;
		Foreach(eid, nodes[a].to_eids) {
			int b = edges[eid].to;
			if (!nodes[b].done && edges[eid].cap > 0) {
				ll df = dfs(b, min(f, edges[eid].cap));
				if (df > 0) {
					edges[eid].cap -= df;
					edges[dual_eid(eid)].cap += df;
					return df;
				}
			}
		}
		return 0;
	}
	vector<node> nodes;
	vector<edge> edges;
	int n, m;
	int source, sink;
public:
	Maxflow(graph_t G, int s, int t) {
		n = G.n;
		m = G.edges.size();
		nodes.resize(n);
		edges.resize(m * 2);
		Loop(i, n) nodes[i] = { i, false, {} };
		Loop(i, m) {
			int a = G.edges[i].first;
			int b = G.edges[i].second;
			nodes[a].to_eids.push_back(i);
			nodes[b].to_eids.push_back(i + m);
			edges[i] = { i, a, b, G.caps[i] };
			edges[i + m] = { i + m, b, a, 0 };
		}
		source = s;
		sink = t;
		do {
			Loop(i, n) nodes[i].done = false;
		} while(dfs(source, LLONG_MAX));
		return;
	}
	vll get_eid_flow() {
		vll ret(m);
		Loop(i, m) {
			ret[i] = edges[i + m].cap;
		}
		return ret;
	}
	ll get_maxflow() {
		ll ret = 0;
		Foreach(eid, nodes[sink].to_eids) {
			if (eid >= m) ret += edges[eid].cap;
		}
		return ret;
	}
};