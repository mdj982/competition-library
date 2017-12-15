struct graph_t {
	int n;           // |V|, index begins with 0
	int m;           // |E|
	vector<P> edges; // E
	vector<ll> cost; // cost or distance
	vector<ll> cap;  // capacity
};

class Strongly_Connected_Components {
private:
	struct node {
		int id; bool done; vi to_eid; vi to; int from_eid; int from;
	};
	vector<node> nodes[2];
	int n;
	stack<int> stk;
	vvi sccs;
	void scc_dfs(int a, int is_r) {
		nodes[is_r][a].done = true;
		Loop(i, nodes[is_r][a].to.size()) {
			int b = nodes[is_r][a].to[i];
			if (b == nodes[is_r][a].from) continue;
			if (!nodes[is_r][b].done) {
				nodes[is_r][b].from = a;
				scc_dfs(b, is_r);
			}
		}
		if (is_r == 0) stk.push(a);
		else {
			sccs.back().push_back(a);
		}
		return;
	}
public:
	Strongly_Connected_Components(graph_t G) {
		n = G.n;
		Loop(i, 2) nodes[i].resize(n);
		Loop(i, 2) {
			Loop(j, n) nodes[i][j] = { i, false,{},{}, -1, -1 };
		}
		Loop(i, G.edges.size()) {
			nodes[0][G.edges[i].first].to_eid.push_back(i);
			nodes[0][G.edges[i].first].to.push_back(G.edges[i].second);
			nodes[1][G.edges[i].second].to_eid.push_back(i);
			nodes[1][G.edges[i].second].to.push_back(G.edges[i].first);
		}
		Loop(j, n) {
			if (!nodes[0][j].done) scc_dfs(j, 0);
		}
		while (stk.size()) {
			int a = stk.top(); stk.pop();
			if (!nodes[1][a].done) {
				sccs.push_back({});
				scc_dfs(a, 1);
				sort(sccs.back().begin(), sccs.back().end());
			}
		}
		sort(sccs.begin(), sccs.end());
		return;
	}
	vvi get_sccs() {
		return sccs;
	}
};

// strongly_connected_components sample
int main() {
	graph_t G;
	cin >> G.n >> G.m;
	Loop(i, G.m) {
		int s, t; cin >> s >> t;
		G.edges.push_back({ s, t });
	}
	Strongly_Connected_Components scc(G);
	vvi sccs = scc.get_sccs();
	Loop(i, sccs.size()) {
		Loop(j, sccs[i].size()) {
			cout << sccs[i][j] << " ";
		}
		cout << endl;
	}
	return 0;
}