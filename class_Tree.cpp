typedef ll nodeval_t;
typedef ll edgeval_t;

struct tree_t {
	int n;           // |V|, index begins with 0
	vector<P> edges; // E
	vector<nodeval_t> vals; // value of nodes
	vector<edgeval_t> costs; // cost, distance, or weight of edges
};

class Tree {
private:
	struct node {
		int id; vi childs; int parent = -1;
		int deg = -1; // the number of edges of the path to the root
		int eid = -1; // edge id of the edge connected by its parent and itself
		int subtree_n = 1; // the number of nodes of the partial tree rooted by itself
		int visited = -1; // time stamp of visiting on DFS
		int departed = -1; // time stamp of departure on DFS
		nodeval_t val; // value of the node itself
		edgeval_t cost; // cost of the edge connected by its parent and itself
		bool operator<(const node & another) const {
			return deg != another.deg ? deg < another.deg : id < another.id;
		}
	};
	struct edgeinfo {
		int eid; int to; edgeval_t cost;
	};
	int n;
	static const nodeval_t init_val = 0;
	static const edgeval_t init_cost = 1;
	vector<vector<edgeinfo>> edges;
	vvi sparse_ancestors;
	void tree_construction() {
		leaves = {};
		queue<int> que;
		que.push(root);
		while (que.size()) {
			int a = que.front(); que.pop();
			deg_order.push_back(a);
			if (a == Tree::root) nodes[a].deg = 0;
			int leaf_flag = true;
			Loop(i, edges[a].size()) {
				int b = edges[a][i].to;
				if (nodes[b].deg != -1) {
					nodes[a].parent = b;
					nodes[a].eid = edges[a][i].eid;
					nodes[a].cost = edges[a][i].cost;
					nodes[a].deg = nodes[b].deg + 1;
				}
				else {
					leaf_flag = false;
					nodes[a].childs.push_back(b);
					que.push(b);
				}
			}
			if (leaf_flag) leaves.push_back(a);
		}
		Loopr(i, n) {
			int a = deg_order[i];
			Loop(j, nodes[a].childs.size()) {
				int b = nodes[a].childs[j];
				nodes[a].subtree_n += nodes[b].subtree_n;
			}
		}
	}
	void solve_sparse_ancestors() {
		sparse_ancestors.resize(n);
		vector<int> current_ancestors;
		stack<int> stk;
		stk.push(Tree::root);
		int time_stamp = 1;
		while (stk.size()) {
			int a = stk.top(); stk.pop();
			nodes[a].visited = time_stamp++;
			for (int i = 1; i <= (int)(current_ancestors.size()); i *= 2) {
				sparse_ancestors[a].push_back(current_ancestors[current_ancestors.size() - i]);
			}
			if (nodes[a].childs.size()) {
				Loop(i, nodes[a].childs.size()) {
					stk.push(nodes[a].childs[i]);
				}
				current_ancestors.push_back(a);
			}
			else {
				nodes[a].departed = time_stamp++;
				while (current_ancestors.size() && (stk.empty() || nodes[stk.top()].parent != current_ancestors.back())) {
					nodes[current_ancestors.back()].departed = time_stamp++;
					current_ancestors.pop_back();
				}
			}
		}
		return;
	}
	bool is_ancestor(int descendant, int ancestor) {
		return nodes[ancestor].visited < nodes[descendant].visited
			&& nodes[descendant].departed < nodes[ancestor].departed;
	}
public:
	vector<node> nodes;
	vi deg_order; // node ids, sorted by deg
	vi leaves;
	int root;
	// T should be non-empty tree
	Tree(tree_t T, int root = -1) {
		n = T.n;
		nodes.resize(n);
		Loop(i, n) {
			nodes[i].id = i;
			nodes[i].val = (int)(T.vals.size()) > i ? T.vals[i] : init_val;
			nodes[i].cost = init_cost;
		}
		edges.resize(n);
		Loop(i, n - 1) {
			edges[T.edges[i].first].push_back({ i, T.edges[i].second, ((int)(T.costs.size()) > i ? T.costs[i] : init_cost) });
			edges[T.edges[i].second].push_back({ i, T.edges[i].first, ((int)(T.costs.size()) > i ? T.costs[i] : init_cost) });
		}
		// the node which has the greatest degree will automatically decided as the root
		if (root < 0) {
			int max_d = -1;
			Loop(i, n) {
				if ((int)(edges[i].size()) > max_d) {
					Tree::root = i;
					max_d = edges[i].size();
				}
			}
		}
		else {
			this->root = min(root, n - 1);
		}
		tree_construction();
		return;
	}
	pair<int, vi> solve_center_of_gravity() {
		pair<int, vi> ret = { INT_MAX,{} };
		vector<node> c_nodes = nodes;
		sort(c_nodes.begin(), c_nodes.end());
		vi record(n, 1);
		Loopr(i, n) {
			int x = n - 1, max_x = INT_MIN;
			Loop(j, c_nodes[i].childs.size()) {
				int b = c_nodes[i].childs[j];
				max_x = max(max_x, record[b]);
				x -= record[b];
				record[c_nodes[i].id] += record[b];
			}
			max_x = max(max_x, x);
			if (max_x < ret.first) ret = { max_x,{ c_nodes[i].id } };
			else if (max_x == ret.first) ret.second.push_back(c_nodes[i].id);
		}
		sort(ret.second.begin(), ret.second.end());
		return ret;
	}
	vi solve_node_inclusion_cnt_in_all_path(bool enable_single_node_path) {
		vi ret(n, 0);
		Loop(i, n) {
			int a = i;
			// desendants to desendants
			Loop(j, nodes[a].childs.size()) {
				int b = nodes[a].childs[j];
				ret[i] += nodes[b].subtree_n * (nodes[a].subtree_n - nodes[b].subtree_n - 1);
			}
			ret[i] /= 2; // because of double counting
			ret[i] += (nodes[a].subtree_n - 1) * (n - nodes[a].subtree_n); // desendants to the others except for itself
			ret[i] += n - 1; // itself to the others
			if (enable_single_node_path) ret[i]++; // itself
		}
		return ret;
	}
	vi solve_edge_inclusion_cnt_in_all_path() {
		vi ret(n - 1, 0);
		Loop(i, n) {
			int eid = nodes[i].eid;
			if (eid < 0) continue;
			ret[eid] = nodes[i].subtree_n * (n - nodes[i].subtree_n); // members in the partial tree to the others
		}
		return ret;
	}
	int get_lowest_common_ancestor(int u, int v) {
		if (sparse_ancestors.size() == 0) solve_sparse_ancestors();
		if (u == v) return u;
		if (is_ancestor(u, v)) return v;
		if (is_ancestor(v, u)) return u;
		int a = u;
		while (!is_ancestor(v, sparse_ancestors[a][0])) {
			int b = sparse_ancestors[a][0];
			Loop1(i, sparse_ancestors[a].size() - 1) {
				if (is_ancestor(v, sparse_ancestors[a][i])) break;
				else b = sparse_ancestors[a][i - 1];
			}
			a = b;
		}
		return sparse_ancestors[a][0];
	}
};