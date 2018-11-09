typedef ll val_t;

struct graph_t {
	int n;           // |V|, index begins with 0
	int m;           // |E|
	vector<P> edges; // E
	vector<val_t> vals; // V
	vector<ll> costs; // cost or distance
	vector<ll> caps;  // capacity
};

class Topological_Sort {
private:
	struct node {
		int id; bool done; vi to; int from_cnt;
	};
	vector<node> nodes;
	int n, m;
	vi result;
	bool cycle_existing_flag;
public:
	Topological_Sort(graph_t G) {
		n = G.n;
		m = G.edges.size();
		nodes.resize(n);
		Loop(i, n) nodes[i] = { i, false,{}, 0 };
		Loop(i, m) {
			nodes[G.edges[i].first].to.push_back(G.edges[i].second);
			nodes[G.edges[i].second].from_cnt++;
		}
		result = {};
		set<int> st;
		Loop(i, n) {
			if (nodes[i].from_cnt == 0) st.insert(i);
		}
		while (st.size()) {
			int a = *(st.begin()); st.erase(a);
			result.push_back(a);
			Loop(i, nodes[a].to.size()) {
				int b = nodes[a].to[i];
				nodes[b].from_cnt--;
				if (nodes[b].from_cnt == 0) st.insert(b);
			}
		}
		if (result.size() != n) cycle_existing_flag = true;
		else cycle_existing_flag = false;
		return;
	}
	vi get_result() {
		return result;
	}
	bool is_cycle_existing() {
		return cycle_existing_flag;
	}
};