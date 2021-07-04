struct tree_t {
	using nodeval_t = int;
	using edgeval_t = int;
	int n;           // |V|, index begins with 0
	vector<P> edges; // E
	vector<nodeval_t> vals; // value of nodes
	vector<edgeval_t> costs; // cost, distance, or weight of edges
};


// a should be sorted, return will be the root
template<class val_t, class tree_t>
int make_treap(const vector<val_t> &a, const tree_t &T, int l = 0, int r = -1, int p = -1) {
	if (r == -1) { r = a.size(); T.n = a.size(); }
	if (r - l == 0) return -1;
	int mid = (l + r) / 2;
	if (p != -1) T.edges.push_back({ mid, p });
	make_treap(a, T, l, mid, mid);
	make_treap(a, T, mid + 1, r, mid);
	return mid;
}

// #define ANCESTOR
// #define HLD
class Tree {
	using nodeval_t = int;
	using edgeval_t = int;
private:
	struct node {
		vi childs; int parent = -1;
		int deg = -1; // the number of edges of the path to the root
		int eid = -1; // edge id of the edge connected by its parent and itself
		int subtree_n = 1; // the number of nodes of the partial tree rooted by itself
#ifdef ANCESTOR
		int visited = -1; // time stamp of visiting on DFS, call solve_sprs_ancestors() for activation
		int departed = -1; // time stamp of departure on DFS, call solve_sprs_ancestors() for activation
#endif
#ifdef HLD
		int pid = -1; // path id of heavy light decompotion
		int qid = -1; // id in its path
#endif
		nodeval_t val; // value of the node itself
		edgeval_t cost; // cost of the edge connected by its parent and itself
	};
	struct edgeinfo_t {
		int eid; int to; edgeval_t cost;
	};
	int n;
	static const nodeval_t init_val = 0;
	static const edgeval_t init_cost = 1;
#ifdef ANCESTOR
	vvi sprs_ancestors; // (1 << j)-th ancestors in each node_id = i
#endif
#ifdef HLD
	vvi hld_paths; // paths
#endif
	void tree_construction(const vector<vector<edgeinfo_t>> &edges) {
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
public:
	vector<node> nodes;
	vi deg_order; // node ids, sorted by deg
	vi leaves;
	int root;
public:
	// T should be non-empty tree
	Tree(const tree_t &T, int root) {
		this->n = T.n;
		this->root = root;
		nodes.resize(n);
		Loop(i, n) {
			nodes[i].val = (int)(T.vals.size()) > i ? T.vals[i] : init_val;
			nodes[i].cost = init_cost;
		}
		vector<vector<edgeinfo_t>> edges(n);
		Loop(i, n - 1) {
			edges[T.edges[i].fst].push_back({ i, T.edges[i].snd, ((int)(T.costs.size()) > i ? T.costs[i] : init_cost) });
			edges[T.edges[i].snd].push_back({ i, T.edges[i].fst, ((int)(T.costs.size()) > i ? T.costs[i] : init_cost) });
		}
		tree_construction(edges);
		return;
	}
	int solve_diameter() {
		vi d(n, -1);
		queue<int> que;
		d[deg_order[n - 1]] = 0;
		que.push(deg_order[n - 1]);
		while (que.size()) {
			int a = que.front(); que.pop();
			int p = nodes[a].parent;
			if (d[p] == -1) {
				d[p] = d[a] + 1;
				que.push(nodes[a].parent);
			}
			Foreach(b, nodes[a].childs) {
				if (d[b] == -1) {
					d[b] = d[a] + 1;
					que.push(b);
				}
			}
		}
		return *max_element(d.begin(), d.end());
	}
	pair<int, vi> solve_center_of_gravity() {
		pair<int, vi> ret = { INT_MAX,{} };
		vi record(n, 1);
		Foreach(a, deg_order) {
			int x = n - 1, max_x = INT_MIN;
			Foreach(b, nodes[a].childs) {
				max_x = std::max(max_x, record[b]);
				x -= record[b];
				record[a] += record[b];
			}
			max_x = std::max(max_x, x);
			if (max_x < ret.fst) ret = { max_x,{ a } };
			else if (max_x == ret.fst) ret.snd.push_back(a);
		}
		sort(ret.snd.begin(), ret.snd.end());
		return ret;
	}
	vi solve_node_inclusion_cnt_in_all_path(bool enable_single_node_path) {
		vi ret(n, 0);
		Loop(i, n) {
			int a = i;
			// desendants to desendants
			Foreach(b, nodes[a].childs) {
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
#ifdef ANCESTOR
	void solve_sprs_ancestors() {
		sprs_ancestors.resize(n);
		vector<int> current_ancestors;
		stack<int> stk;
		stk.push(Tree::root);
		int time_stamp = 0;
		while (stk.size()) {
			int a = stk.top(); stk.pop();
			nodes[a].visited = time_stamp++;
			for (int i = 1; i <= (int)(current_ancestors.size()); i *= 2) {
				sprs_ancestors[a].push_back(current_ancestors[current_ancestors.size() - i]);
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
	int get_lowest_common_ancestor(int u, int v) {
		if (u == v) return u;
		if (is_ancestor(u, v)) return v;
		if (is_ancestor(v, u)) return u;
		int a = u;
		while (!is_ancestor(v, sprs_ancestors[a][0])) {
			int b = sprs_ancestors[a][0];
			Loop1(i, sprs_ancestors[a].size() - 1) {
				if (is_ancestor(v, sprs_ancestors[a][i])) break;
				else b = sprs_ancestors[a][i - 1];
			}
			a = b;
		}
		return sprs_ancestors[a][0];
	}
	int get_ancestor(int descendant, int k) {
		if (k == 0) return descendant;
		int l = (int)log2(k);
		if (l >= sprs_ancestors[descendant].size()) return -1;
		else return get_ancestor(sprs_ancestors[descendant][l], k - (1 << l));
	}
	// return first value causing "t" in evalfunc that returns descendant->[f,...,f,t,...,t]->root
	// NOTE: if [f,...,f] then return -1
	template<typename bsargv_t>
	int binary_search_upper_ancestor(int descendant, const bsargv_t &bsargv, bool(*evalfunc)(int, const bsargv_t&)) {
		if (evalfunc(descendant, bsargv)) return descendant;
		if (descendant == root) return -1;
		Loop(i, sprs_ancestors[descendant].size()) {
			if (evalfunc(sprs_ancestors[descendant][i], bsargv)) {
				if (i == 0) return binary_search_upper_ancestor(sprs_ancestors[descendant][0], bsargv, evalfunc);
				else return binary_search_upper_ancestor(sprs_ancestors[descendant][i - 1], bsargv, evalfunc);
			}
		}
		return binary_search_upper_ancestor(sprs_ancestors[descendant].back(), bsargv, evalfunc);
	}
	// return last value causing "t" in evalfunc that returns descendant->[t,...,t,f,...,f]->root
	// NOTE: if [f,...,f] then return -1
	template<typename bsargv_t>
	int binary_search_lower_ancestor(int descendant, const bsargv_t &bsargv, bool(*evalfunc)(int, const bsargv_t&)) {
		if (!evalfunc(descendant, bsargv)) return -1;
		if (descendant == root) return root;
		Loop(i, sprs_ancestors[descendant].size()) {
			if (!evalfunc(sprs_ancestors[descendant][i], bsargv)) {
				if (i == 0) return descendant;
				else return binary_search_lower_ancestor(sprs_ancestors[descendant][i - 1], bsargv, evalfunc);
			}
		}
		return binary_search_lower_ancestor(sprs_ancestors[descendant].back(), bsargv, evalfunc);
	}
	// static bool evalfunc(int id, bsargv_t bsargv);
#endif
#ifdef HLD
	void solve_hld() {
		Foreach(a, deg_order) {
			if (nodes[a].pid == -1) {
				nodes[a].pid = int(hld_paths.size());
				nodes[a].qid = 0;
				hld_paths.push_back({ a });
			}
			int max_id = -1;
			int max_subtree_n = 0;
			Foreach(b, nodes[a].childs) {
				if (nodes[b].subtree_n > max_subtree_n) {
					max_id = b;
					max_subtree_n = nodes[b].subtree_n;
				}
			}
			if (max_id == -1) continue;
			nodes[max_id].pid = nodes[a].pid;
			nodes[max_id].qid = nodes[a].qid + 1;
			hld_paths[nodes[a].pid].push_back(max_id);
		}
	}
	struct pathinfo_t {
		int id;
		int l, r; // [l, r)
	};
	// return all node ids in the single path
	vector<int> get_ids_in_path(const pathinfo_t &pathinfo) {
		vi ret(pathinfo.r - pathinfo.l);
		Loop(i, ret.size()) {
			ret[i] = hld_paths[pathinfo.id][pathinfo.l + i];
		}
		return ret;
	}
	// if weight is for each node, include_lca = true
	// if weight is for each edge, include_lca = false
	vector<pathinfo_t> get_path_in_hld(int u, int v, bool include_lca) {
		vector<pathinfo_t> ret;
		int w = get_lowest_common_ancestor(u, v);
		Foreach(x, vector<int>({ u, v })) {
			int a = x;
			while (a != w) {
				if (nodes[a].pid != nodes[w].pid) {
					ret.push_back({ nodes[a].pid, 0, nodes[a].qid + 1 });
					a = nodes[hld_paths[nodes[a].pid][0]].parent;
				}
				else {
					ret.push_back({ nodes[a].pid, nodes[w].qid + 1, nodes[a].qid + 1 });
					a = w;
				}
			}
		}
		if (include_lca) {
			Loop(i, ret.size()) {
				if (nodes[w].pid == ret[i].id) {
					ret[i].l -= 1;
					include_lca = false;
				}
			}
		}
		if (include_lca) {
			ret.push_back({ nodes[w].pid, nodes[w].qid, nodes[w].qid + 1 });
		}
		return ret;
	}
	vi get_hld_path_sizes() {
		vi ret(hld_paths.size());
		Loop(i, hld_paths.size()) {
			ret[i] = int(hld_paths[i].size());
		}
		return ret;
	}
	int get_hld_path_size(int pid) {
		return int(hld_paths[pid].size());
	}
#endif
};
