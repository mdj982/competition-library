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
	struct node {
		int id; bool done; vi to_eid; vi to; vll cost; int from_eid; int from; ll d;
		bool operator<(const node & another) const {
			return d != another.d ? d > another.d : id > another.id;
		}
	};
	vector<node> nodes;
	int n, root;
public:
	Prim(graph_t G, int start) {
		n = G.n;
		nodes.resize(n);
		Loop(i, n) nodes[i] = { i, false,{},{},{}, -1, -1, LLONG_MAX };
		Loop(i, G.edges.size()) {
			nodes[G.edges[i].first].to_eid.push_back(i);
			nodes[G.edges[i].first].to.push_back(G.edges[i].second);
			nodes[G.edges[i].first].cost.push_back(G.costs[i]);
		}
		root = start;
		nodes[root].d = 0;
		priority_queue<node> pq;
		pq.push(nodes[root]);
		while (pq.size()) {
			node a = pq.top(); pq.pop();
			if (nodes[a.id].done) continue;
			nodes[a.id].done = true;
			Loop(j, a.to.size()) {
				node *b = &nodes[a.to[j]];
				if (b->done) continue;
				ll buf = a.cost[j];
				if (buf < b->d) {
					b->d = buf;
					b->from_eid = a.to_eid[j];
					b->from = a.id;
					pq.push(*b);
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
	vi get_tree_eid() {
		vi ret;
		Loop(i, n) {
			if (i != root) ret.push_back(nodes[i].from_eid);
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

// prim sample
int main() {
	graph_t G;
	cin >> G.n >> G.m;
	Loop(i, G.m) {
		int s, t, c; cin >> s >> t >> c;
		G.edges.push_back({ s, t });
		G.costs.push_back(c);
		G.edges.push_back({ t, s });
		G.costs.push_back(c);
	}
	Prim prim(G, 0);
	cout << prim.get_weight() << endl;
	return 0;
}