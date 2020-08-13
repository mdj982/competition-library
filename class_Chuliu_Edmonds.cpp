#include "auto_util_header.hpp"

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

/*

class Union_Find {
private:
	vi p, r, c; // parent, rank, the number of connected components
public:
	Union_Find(int N) {
		p.resize(N);
		r.resize(N);
		c.resize(N);
		Loop(i, N) {
			p[i] = i;
			r[i] = 0;
			c[i] = 1;
		}
	}
	int find(int x) {
		if (p[x] == x) return x;
		else return p[x] = find(p[x]);
	}
	void unite(int x, int y) {
		x = find(x);
		y = find(y);
		if (x == y) return;
		if (r[x] == r[y]) r[x]++;
		if (r[x] < r[y]) {
			p[x] = y;
			c[y] += c[x];
		}
		else {
			p[y] = x;
			c[x] += c[y];
		}
	}
	bool is_same(int x, int y) {
		return find(x) == find(y);
	}
	int get_cnt(int x) {
		return c[find(x)];
	}
};

class Binom_Heaps {
public:
	using val_t = ll; // operator< should be defined
	using ptr_t = ll; // virtual pointer
private:
	struct node_t {
		bool enable = false;
		val_t upd = 0, add = 0, val = 0;
		node_t* parent = nullptr;
		vector<node_t*> childs;
	};
	struct heap_t {
		array<node_t*, 32> roots;
	};
	val_t init_val = LLONG_MAX; // maximum value of struct val_t
	heap_t init_heap;
	vector<node_t*> ptr_mp0;
	unordered_map<node_t*, ptr_t> ptr_mp1;
	vector<int> heap_sizes;
	vector<heap_t> heaps;
	Union_Find *uf;
	node_t* pop_core(heap_t &heap0, const int p) {
		heap_t heap1 = init_heap;
		Loop(i, p) heap1.roots[i] = heap0.roots[p]->childs[i];
		if (heap0.roots[p]->enable) {
			val_t buf = heap0.roots[p]->upd + heap0.roots[p]->add;
			Loop(i, p) {
				heap1.roots[i]->enable = true;
				heap1.roots[i]->upd = buf;
				heap1.roots[i]->add = 0;
				heap1.roots[i]->parent = nullptr;
			}
		}
		else {
			Loop(i, p) {
				heap1.roots[i]->add += heap0.roots[p]->add;
				heap1.roots[i]->parent = nullptr;
			}
		}
		node_t *ret = heap0.roots[p];
		delete(heap0.roots[p]); heap0.roots[p] = nullptr;
		heap0 = merge_heap_core(heap0, heap1);
		return ret;
	}
	node_t* merge_root_core(node_t *root0, node_t *root1) {
		val_t val0 = root0->add + (root0->enable ? root0->upd : root0->val);
		val_t val1 = root1->add + (root1->enable ? root1->upd : root1->val);
		if (val1 < val0) swap(root0, root1);
		if (root0->enable) {
			root0->enable = false;
			root0->val = root0->upd;
			Loop(i, root0->childs.size()) {
				root0->childs[i]->enable = true;
				root0->childs[i]->upd = root0->val;
				root0->childs[i]->add = 0;
			}
		}
		root0->childs.push_back(root1);
		root1->parent = root0;
		root1->add -= root0->add;
		return root0;
	}
	heap_t merge_heap_core(heap_t &heap0, heap_t &heap1) {
		heap_t carry = init_heap;
		Loop(i, 32) {
			vector<node_t*> roots;
			if (heap0.roots[i] != nullptr) roots.push_back(heap0.roots[i]);
			if (heap1.roots[i] != nullptr) roots.push_back(heap1.roots[i]);
			if (carry.roots[i] != nullptr) roots.push_back(carry.roots[i]);
			if (roots.size() == 1) {
				carry.roots[i] = roots[0];
			}
			else if (roots.size() == 2) {
				carry.roots[i + 1] = merge_root_core(roots[0], roots[1]);
				carry.roots[i] = nullptr;
			}
			else if (roots.size() == 3) {
				carry.roots[i + 1] = merge_root_core(roots[0], roots[1]);
				carry.roots[i] = roots[2];
			}
		}
		return carry;
	}
public:
	Binom_Heaps(int n) {
		Loop(i, 32) init_heap.roots[i] = nullptr;
		heaps = vector<heap_t>(n, init_heap);
		heap_sizes = vector<int>(n, 0);
		uf = new Union_Find(n);
	}
	// insert x into originally k-th heap
	ptr_t insert(int k, val_t x) {
		ptr_t ret = ptr_mp0.size();
		int h = uf->find(k);
		node_t *node = new node_t;
		node->enable = false;
		node->val = x;
		ptr_mp0.push_back(node);
		ptr_mp1[node] = ret;
		heap_t token = init_heap;
		token.roots[0] = node;
		heaps[h] = merge_heap_core(token, heaps[h]);
		heap_sizes[h] += 1;
		return ret;
	}
	// get the minimum value in originally k-th heap
	pair<val_t, ptr_t> top(int k) {
		int h = uf->find(k);
		pair<val_t, node_t*> ret = { init_val, nullptr };
		for (auto root : heaps[h].roots) {
			if (root == nullptr) continue;
			ret = min(ret, { root->add + (root->enable ? root->upd : root->val), root });
		}
		return { ret.fst, ptr_mp1[ret.snd] };
	}
	// remove the minimum value in originally k-th heap
	ptr_t pop(int k) {
		int h = uf->find(k);
		pair<val_t, int> buf = { init_val, -1 };
		int p = -1;
		for (auto root : heaps[h].roots) {
			p++;
			if (root == nullptr) continue;
			buf = min(buf, { root->add + (root->enable ? root->upd : root->val), p });
		}
		heap_sizes[h] -= 1;
		node_t *node = pop_core(heaps[h], buf.snd);
		ptr_t ret = ptr_mp1[node];
		ptr_mp0[ret] = nullptr;
		return ret;
	}
	// add x into all values in originally k-th heap 
	void add_all(int k, val_t x) {
		int h = uf->find(k);
		for (auto root : heaps[h].roots) {
			if (root == nullptr) continue;
			root->add += x;
		}
	}
	// update all values into x in originally k-th heap
	void upd_all(int k, val_t x) {
		int h = uf->find(k);
		for (auto root : heaps[h].roots) {
			if (root == nullptr) continue;
			root->enable = true;
			root->upd = x;
			root->add = 0;
		}
	}
	// merge originally k0-th heap and originally k1-th heap
	void merge(int k0, int k1) {
		int h0 = uf->find(k0);
		int h1 = uf->find(k1);
		if (h0 == h1) return;
		heap_t heap0 = heaps[h0];
		heap_t heap1 = heaps[h1];
		int heap_size0 = heap_sizes[h0];
		int heap_size1 = heap_sizes[h1];
		uf->unite(h0, h1);
		int h = uf->find(h0);
		heaps[h] = merge_heap_core(heap0, heap1);
		heap_sizes[h] = heap_size0 + heap_size1;
	}
	// get value from virtual pointer
	val_t get_val(ptr_t ptr) {
		node_t *node = ptr_mp0[ptr];
		val_t ret = node->val;
		while (node != nullptr) {
			if (node->enable) ret = node->upd;
			ret += node->add;
			node = node->parent;
		}
		return ret;
	}
	bool prioritize(ptr_t ptr, const val_t x) {
		node_t *node = ptr_mp0[ptr];
		vector<node_t*> path = { node };
		while (path.back()->parent != nullptr) path.push_back({ path.back()->parent });
		vector<val_t> accs(path.size() + 1);
		vector<val_t> vals(path.size());
		bool enable = false;
		val_t upd = 0;
		Loopr(i, path.size()) {
			if (enable) {
				vals[i] = upd;
			}
			else {
				accs[i] = path[i]->add + accs[i + 1];
				if (path[i]->enable) {
					upd = accs[i] + path[i]->upd;
					vals[i] = upd;
					enable = true;
				}
				else {
					vals[i] = accs[i] + path[i]->val;
				}
			}
		}
		if (vals[0] < x) return false;
		if (vals[0] == x) return true;
		vals[0] = x;
		int p = 0;
		Loop(i, path.size() - 1) {
			if (vals[i] < vals[i + 1]) {
				swap(vals[i], vals[i + 1]);
				swap(ptr_mp1[path[i]], ptr_mp1[path[i + 1]]);
				p = i + 1;
			}
		}
		Loop(i, path.size()) {
			ptr_mp0[ptr_mp1[path[i]]] = path[i];
		}
		Loopr(i, p + 1) {
			if (path[i]->enable) {
				for (auto child : path[i]->childs) {
					child->enable = true;
					child->upd = path[i]->upd;
					child->add = path[i]->add;
				}
				path[i]->enable = false;
				path[i]->add = 0;
				path[i]->val = vals[i] - accs[i + 1];
				break;
			}
			else {
				path[i]->val = vals[i] - accs[i];
			}
		}
		return true;
	}
	int size(int k) {
		int h = uf->find(k);
		return heap_sizes[h];
	}
};

class Chuliu_Edmonds {
private:
	Union_Find *uf;
	int n;
	int root;
	bool failed = false;
	stack<int> eid_stk;
	struct edge_t {
		int s, t;
		ll c;
	};
	vi from_eids;
	vector<edge_t> edges;
	Binom_Heaps *bhs;
	queue<int> que;
	int contract(const vi &ids) {
		int leader = ids[0];
		Foreach(id, ids) {
			bhs->add_all(id, -bhs->get_val(from_eids[id]));
		}
		Foreach(id, ids) {
			bhs->merge(leader, id);
			uf->unite(leader, id);
		}
		leader = uf->find(leader);
		from_eids[leader] = -1;
		while (bhs->size(leader)) {
			Pll buf = bhs->top(leader);
			int eid = int(buf.snd);
			if (uf->is_same(edges[eid].s, edges[eid].t)) {
				bhs->pop(leader);
			}
			else {
				from_eids[leader] = eid;
				eid_stk.push(eid);
				break;
			}
		}
		if (from_eids[leader] == -1) failed = true;
		return leader;
	}
	void process(int t) {
		vi seq = { t };
		while (true) {
			int s = uf->find(edges[from_eids[t]].s);
			if (uf->is_same(s, root)) {
				Foreach(a, seq) uf->unite(a, root);
				return;
			}
			if (s == seq[0]) {
				int leader = contract(seq);
				que.push(leader);
				return;
			}
			seq.push_back(s);
			t = s;
		}
	}
public:
	Chuliu_Edmonds(const vvi &lst, const vvll &cst, int root) {
		this->n = lst.size();
		this->root = root;
		Loop(s, n) {
			Loop(j, lst[s].size()) {
				int t = lst[s][j];
				ll c = cst[s][j];
				edges.push_back({ s, t, c });
			}
		}
		uf = new Union_Find(n);
		bhs = new Binom_Heaps(n);
		from_eids = vi(n, -1);
		Foreach(edge, edges) {
			bhs->insert(edge.t, edge.c);
		}
		Loop(t, n) {
			if (t != this->root) {
				Pll buf = bhs->top(t);
				int eid = int(buf.snd);
				from_eids[t] = eid;
				eid_stk.push(eid);
				que.push(t);
			}
		}
		while (que.size()) {
			int a = que.front(); que.pop();
			if (uf->is_same(a, this->root) || uf->find(a) != a) continue;
			process(a);
			if (failed) return;
		}
		from_eids = vi(n, -1);
		while (eid_stk.size()) {
			int eid = eid_stk.top(); eid_stk.pop();
			if (from_eids[edges[eid].t] == -1) from_eids[edges[eid].t] = eid;
		}
	}
	vector<P> get_tree_idpair() {
		vector<P> ret;
		Loop(t, n) {
			if (t != root) {
				ret.push_back({ edges[from_eids[t]].s, edges[from_eids[t]].t });
			}
		}
		return ret;
	}
	ll get_weight() {
		ll ret = 0;
		Loop(t, n) {
			if (t != root) {
				ret += edges[from_eids[t]].c;
			}
		}
		return ret;
	}
};

int main() {
	int n, m, r; cin >> n >> m >> r;
	vvi lst(n);
	vvll cst(n);
	Loop(i, m) {
		int s, t; cin >> s >> t;
		ll c; cin >> c;
		lst[s].push_back(t);
		cst[s].push_back(c);
	}
	Chuliu_Edmonds ce(lst, cst, r);
	cout << ce.get_weight() << endl;
}

*/