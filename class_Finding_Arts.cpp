typedef ll val_t;

struct graph_t {
	int n;           // |V|, index begins with 0
	int m;           // |E|
	vector<P> edges; // E
	vector<val_t> vals; // V
	vector<ll> costs; // cost or distance
	vector<ll> caps;  // capacity
};

class Finding_Arts {
private:
	struct node {
		int id; bool done; vi to_eid; vi to; int from_eid; int from; int pre; int low;
	};
	vector<node> nodes;
	int n, m;
	int ord;
	vi arts;
	void lowlink_dfs(int a, bool isroot) {
		nodes[a].done = true;
		nodes[a].pre = nodes[a].low = ord;
		ord++;
		int cnt = 0;
		Loop(i, nodes[a].to.size()) {
			int b = nodes[a].to[i];
			if (b == nodes[a].from) continue;
			if (!nodes[b].done) {
				nodes[b].from = a;
				lowlink_dfs(b, false);
				nodes[a].low = min(nodes[a].low, nodes[b].low);
				if (nodes[a].pre <= nodes[b].low) cnt++;
			}
			else {
				nodes[a].low = min(nodes[a].low, nodes[b].pre);
			}
		}
		if (cnt > (isroot ? 1 : 0)) arts.push_back(a);
		return;
	}
public:
	Finding_Arts(graph_t G) {
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
			if (!nodes[i].done) lowlink_dfs(i, true);
		}
		sort(arts.begin(), arts.end());
	}
	vi get_arts() {
		return arts;
	}
};

// finding_arts sample
int main() {
	graph_t G;
	cin >> G.n >> G.m;
	Loop(i, G.m) {
		int s, t; cin >> s >> t;
		G.edges.push_back({ s, t });
		G.edges.push_back({ t, s });
	}
	Finding_Arts finding_arts(G);
	vi arts = finding_arts.get_arts();
	Loop(i, arts.size()) {
		cout << arts[i] << endl;
	}
	return 0;
}