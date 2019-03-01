template<class nodeval_t, class edgeval_t>
struct tree_t {
	int n;           // |V|, index begins with 0
	vector<P> edges; // E
	vector<nodeval_t> vals; // value of nodes
	vector<edgeval_t> costs; // cost, distance, or weight of edges
};

template<class nodeval_t, class edgeval_t>
class Tree {
private:
	struct node {
		int id; vi childs; int parent = -1;
		int deg = -1; // the number of edges of the path to the root
		int eid = -1; // edge id of the edge connected by its parent and itself
		int subtree_n = 1; // the number of nodes of the partial tree rooted by itself
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
public:
	vector<node> nodes;
	vi deg_order; // node ids, sorted by deg
	vi leaves;
	int root;
	// T should be non-empty tree
	Tree(tree_t<nodeval_t, edgeval_t> T, int root) {
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
		Tree::root = root;
		tree_construction();
		return;
	}
};

// Warning: val_t should be small as array<int, 5>, when n = 1e6 with 256MB