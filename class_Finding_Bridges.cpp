typedef ll val_t;

struct graph_t {
	int n;           // |V|, index begins with 0
	int m;           // |E|
	vector<P> edges; // E
	vector<val_t> vals; // V
	vector<ll> costs; // cost or distance
	vector<ll> caps;  // capacity
};

class Finding_Bridges {
private:
	struct node {
		int id; bool done; vi to_eid; vi to; int from_eid; int from; int pre; int low;
	};
	vector<node> nodes;
	int n, m;
	int ord;
	vector<P> bridges_idpair;
	vi bridges_eid;
	void lowlink_dfs(int a) {
		nodes[a].done = true;
		nodes[a].pre = nodes[a].low = ord;
		ord++;
		Loop(i, nodes[a].to.size()) {
			int b = nodes[a].to[i];
			if (b == nodes[a].from) continue;
			if (!nodes[b].done) {
				nodes[b].from = a;
				lowlink_dfs(b);
				nodes[a].low = min(nodes[a].low, nodes[b].low);
				if (nodes[a].pre < nodes[b].low) {
					if (a < b) bridges_idpair.push_back({ a,b });
					else bridges_idpair.push_back({ b,a });
					bridges_eid.push_back(nodes[a].to_eid[i]);
				}
			}
			else {
				nodes[a].low = min(nodes[a].low, nodes[b].pre);
			}
		}
		return;
	}
public:
	Finding_Bridges(graph_t G) {
		n = G.n;
		m = G.edges.size();
		nodes.resize(n);
		Loop(i, n) nodes[i] = { i, false,{},{}, -1, -1, -1, -1 };
		Loop(i, m) {
			nodes[G.edges[i].first].to_eid.push_back(i);
			nodes[G.edges[i].first].to.push_back(G.edges[i].second);
		}
		ord = 0;
		Loop(i, nodes.size()) {
			if (!nodes[i].done) lowlink_dfs(i);
		}
		sort(bridges_idpair.begin(), bridges_idpair.end());
		sort(bridges_eid.begin(), bridges_eid.end());
	}
	vector<P> get_bridges_idpair() {
		return bridges_idpair;
	}
	vi get_bridges_eid() {
		return bridges_eid;
	}
};

// finding_bridges sample
int main() {
	graph_t G;
	cin >> G.n >> G.m;
	Loop(i, G.m) {
		int s, t; cin >> s >> t;
		G.edges.push_back({ s, t });
		G.edges.push_back({ t, s });
	}
	Finding_Bridges finding_bridges(G);
	vector<P> bridges = finding_bridges.get_bridges_idpair();
	Loop(i, bridges.size()) {
		cout << bridges[i].first << " " << bridges[i].second << endl;
	}
	return 0;
}