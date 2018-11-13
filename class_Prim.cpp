typedef ll val_t;

struct graph_t {
	int n;           // |V|, index begins with 0
	int m;           // |E|
	vector<P> edges; // E
	vector<val_t> vals; // V
	vector<ll> costs; // cost or distance
	vector<ll> caps;  // capacity
};

class Prim {
private:
	// note: eid of dual_edge is negative
	struct node {
		int id; bool done; vi to_eid; vi to; vll costs; int from_eid; int from; ll d;
	};
	struct pq_t {
		int id; ll d;
		bool operator<(const pq_t & another) const {
			return d != another.d ? d > another.d : id > another.id;
		}
	};
	vector<node> nodes;
	int n, m, root;
public:
	Prim(graph_t G, int start) {
		n = G.n;
		m = G.edges.size();
		nodes.resize(n);
		Loop(i, n) nodes[i] = { i, false,{},{},{}, -1, -1, LLONG_MAX };
		Loop(i, m) {
			nodes[G.edges[i].first].to_eid.push_back(i);
			nodes[G.edges[i].first].to.push_back(G.edges[i].second);
			nodes[G.edges[i].first].costs.push_back(G.costs[i]);
			nodes[G.edges[i].second].to_eid.push_back(i - m);
			nodes[G.edges[i].second].to.push_back(G.edges[i].first);
			nodes[G.edges[i].second].costs.push_back(G.costs[i]);
		}
		root = start;
		nodes[root].d = 0;
		priority_queue<pq_t> pq;
		pq.push({ nodes[root].id, nodes[root].d });
		while (pq.size()) {
			node *a = &nodes[pq.top().id];
			pq.pop();
			if (nodes[a->id].done) continue;
			nodes[a->id].done = true;
			Loop(j, a->to.size()) {
				node *b = &nodes[a->to[j]];
				if (b->done) continue;
				ll buf = a->costs[j];
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
	vector<P> get_tree_idpair() {
		vector<P> ret;
		Loop(i, n) {
			if (i != root) ret.push_back({ nodes[i].from, i });
		}
		return ret;
	}
	// here negative eid will transformed to non-negative eid
	vi get_tree_eid() {
		vi ret;
		Loop(i, n) {
			if (i != root) {
				ret.push_back(nodes[i].from_eid);
				if (ret.back() < 0) ret.back() += m;
			}
		}
		return ret;
	}
	ll get_weight() {
		ll ret = 0;
		Loop(i, n) {
			if (i != root) ret += nodes[i].d;
		}
		return ret;
	}
};