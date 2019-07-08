class Chuliu_Edmonds {
private:
	struct edge_t {
		int id;
		ll cost;
		stack<int> included_stk;
		bool operator<(const edge_t & another) const {
			return cost > another.cost;
		}
	};
	// edges are directed to the node itself
	struct node {
		int overnode; bool done; bool fin; priority_queue<edge_t> edges; edge_t from;
	};
	vector<node> nodes;
	int n, root;
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
		priority_queue<edge_t> new_from_edges;
		int cnt = 0;
		do {
			a = topnode(a);
			while (nodes[a].edges.size()) {
				edge_t edge = nodes[a].edges.top();
				nodes[a].edges.pop();
				if (edge.id == nodes[a].from.id) continue;
				edge.cost -= nodes[a].from.cost;
				edge.included_stk.push(a);
				new_from_edges.push(edge);
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
	Chuliu_Edmonds(const vvi &lst, const vvll &cst, int start) {
		n = lst.size();
		nodes.resize(n);
		Loop(i, n) nodes[i] = { -1, false, false, priority_queue<edge_t>(),{} };
		Loop(i, n) {
			Loop(j, lst[i].size()) {
				nodes[lst[i][j]].edges.push({ i, cst[i][j], stack<int>() });
			}
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
						if (nodes[a].edges.empty()) { no_mca = true; return; }
						nodes[a].from = nodes[a].edges.top(); nodes[a].edges.pop();
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
	ll get_weight() {
		if (no_mca) return -1;
		ll ret = 0;
		Loop(i, n) {
			if (i != root) ret += nodes[i].from.cost;
		}
		return ret;
	}
};