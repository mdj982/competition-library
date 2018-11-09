typedef ll val_t;

struct graph_t {
	int n;           // |V|, index begins with 0
	int m;           // |E|
	vector<P> edges; // E
	vector<val_t> vals; // V
	vector<ll> costs; // cost or distance
	vector<ll> caps;  // capacity
};

// solve MCA
class Chuliu_Edmonds {
private:
	struct fromedge {
		int eid;
		int id;
		ll cost;
		stack<int> included_stk;
		bool operator<(const fromedge & another) const {
			return !(cost != another.cost ? cost < another.cost : eid < another.eid);
		}
	};
	struct node {
		int overnode; bool done; bool fin; priority_queue<fromedge> from_edges; fromedge from;
	};
	vector<node> nodes;
	int n, m, root;
	stack<int> stk;
	bool no_mca;
	int topnode(int k) {
		int a = k;
		while (nodes[a].overnode != -1) {
			a = nodes[a].overnode;
		}
		if (k != a) nodes[k].overnode = a;
		return a;
	}
	void contract(int s) {
		int a = s;
		priority_queue<fromedge> new_from_edges;
		int cnt = 0;
		do {
			a = topnode(a);
			while (nodes[a].from_edges.size()) {
				fromedge from_e = nodes[a].from_edges.top();
				nodes[a].from_edges.pop();
				if (from_e.id == nodes[a].from.id) continue;
				from_e.cost -= nodes[a].from.cost;
				from_e.included_stk.push(a);
				new_from_edges.push(from_e);
			}
			nodes[a].overnode = nodes.size();
			a = nodes[a].from.id;
		} while (a != s);
		nodes.push_back({ -1, false, false, new_from_edges,{} });
	}
	void unfold() {
		while (stk.size()) {
			int a = stk.top(); stk.pop();
			if (a >= n) {
				int b = nodes[a].from.included_stk.top();
				ll d = nodes[b].from.cost;
				nodes[b].from = nodes[a].from;
				nodes[b].from.cost += d;
				nodes[b].from.included_stk.pop();
			}
			else nodes[a].fin = true;
		}
	}
public:
	Chuliu_Edmonds(graph_t G, int start) {
		n = G.n;
		m = G.edges.size();
		nodes.resize(n);
		Loop(i, n) nodes[i] = { -1, false, false, priority_queue<fromedge>(),{} };
		Loop(i, m) {
			nodes[G.edges[i].second].from_edges.push({ i, G.edges[i].first, G.costs[i], stack<int>() });
		}
		root = start;
		no_mca = false;
		nodes[root].fin = nodes[root].done = true;
		Loop(i, n) {
			if (!nodes[i].fin) {
				int a = i;
				nodes[a].done = true;
				stk.push(a);
				do {
					int b;
					do {
						if (nodes[a].from_edges.empty()) { no_mca = true; return; }
						nodes[a].from = nodes[a].from_edges.top(); nodes[a].from_edges.pop();
						b = nodes[a].from.id;
					} while (topnode(a) == topnode(b));
					if (nodes[b].fin) unfold();
					else if (nodes[b].done) {
						contract(b);
						stk.push(nodes.size() - 1);
						a = nodes.size() - 1;
					}
					else {
						nodes[b].done = true;
						stk.push(b);
						a = b;
					}
				} while (stk.size());
			}
		}
		return;
	}
	vector<P> get_tree_idpair() {
		if (no_mca) return{};
		vector<P> ret;
		Loop(i, n) {
			if (i != root) ret.push_back({ nodes[i].from.id, i });
		}
		return ret;
	}
	vi get_tree_eid() {
		if (no_mca) return{};
		vi ret;
		Loop(i, n) {
			if (i != root) ret.push_back(nodes[i].from.eid);
		}
		return ret;
	}
	ll get_weight() {
		if (no_mca) return -1;
		ll ret = 0;
		Loop(i, n) {
			if (i != root) ret += nodes[i].from.cost;
		}
		return ret;
	}
};

// Chuliu_Edmonds sample
int main() {
	graph_t G;
	cin >> G.n >> G.m;
	int r; cin >> r;
	Loop(i, G.m) {
		int s, t, c; cin >> s >> t >> c;
		G.edges.push_back({ s, t });
		G.costs.push_back(c);
	}
	Chuliu_Edmonds mca(G, r);
	cout << mca.get_weight() << endl;
	return 0;
}