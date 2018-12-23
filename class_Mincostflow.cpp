
typedef ll val_t;

struct graph_t {
	int n;           // |V|, index begins with 0
	int m;           // |E|
	vector<P> edges; // E
	vector<val_t> vals; // V
	vector<ll> costs; // cost or distance
	vector<ll> caps;  // capacity
};

class Mincostflow {
private:
	struct edge {
		int eid, from, to;
		ll cap, cost;
	};
	struct node {
		int id; ll d; int from_eid; vector<int> to_eids;
	};
	struct pq_t {
		int id; ll d;
		bool operator<(const pq_t & another) const {
			return d != another.d ? d > another.d : id > another.id;
		}
	};
	int dual_eid(int eid) {
		if (eid < m) return eid + m;
		else return eid - m;
	}
	vector<node> nodes;
	vector<edge> edges;
	int n, m;
	int source, sink;
	bool overflow;
public:
	Mincostflow(graph_t G, int s, int t) {
		n = G.n;
		m = int(G.edges.size());
		nodes.resize(n);
		edges.resize(m * 2);
		Loop(i, n) nodes[i] = { i, LLONG_MAX, -1, {} };
		Loop(i, m) {
			int a = G.edges[i].first;
			int b = G.edges[i].second;
			nodes[a].to_eids.push_back(i);
			nodes[b].to_eids.push_back(i + m);
			edges[i] = { i, a, b, G.caps[i], G.costs[i] };
			edges[i + m] = { i + m, b, a, 0, -G.costs[i] };
		}
		source = s;
		sink = t;
		overflow = false;
	}
	bool add_flow(ll f) {
		if (overflow) return false;
		while (f > 0) {
			Loop(i, n) {
				nodes[i].d = LLONG_MAX;
				nodes[i].from_eid = -1;
			}
			// Bellmanford
			nodes[source].d = 0;
			Loop(k, n) {
				Loop(i, n) {
					int a = i;
					if (nodes[a].d == LLONG_MAX) continue;
					Foreach(eid, nodes[a].to_eids) {
						if (edges[eid].cap == 0) continue;
						int b = edges[eid].to;
						if (nodes[a].d + edges[eid].cost < nodes[b].d) {
							nodes[b].d = nodes[a].d + edges[eid].cost;
							nodes[b].from_eid = eid;
							if (k == n - 1) {
								return false;
							}
						}
					}
				}
			}
			if (nodes[sink].d == LLONG_MAX) return false;
			int a = sink;
			ll df = f;
			while (a != source) {
				df = min(df, edges[nodes[a].from_eid].cap);
				a = edges[nodes[a].from_eid].from;
			}
			a = sink;
			while (a != source) {
				edges[nodes[a].from_eid].cap -= df;
				edges[dual_eid(nodes[a].from_eid)].cap += df;
				a = edges[nodes[a].from_eid].from;
			}
			f -= df;
		}
		return true;
	}
	vll get_eid_flow() {
		vll ret(m, -1);
		if (overflow) return ret;
		Loop(i, m) {
			ret[i] = edges[i + m].cap;
		}
		return ret;
	}
	ll get_flow() {
		if (overflow) return -1;
		ll ret = 0;
		Foreach(eid, nodes[sink].to_eids) {
			if (eid >= m) ret += edges[eid].cap;
		}
		return ret;
	}
	ll get_cost() {
		if (overflow) return -1;
		ll ret = 0;
		Loop(i, m) {
			ret += edges[i].cost * edges[i + m].cap;
		}
		return ret;
	}
};