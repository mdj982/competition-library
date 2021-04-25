#include "auto_util_header.hpp"

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
		if (val1 < val0) std::swap(root0, root1);
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
	val_t top(int k) {
		int h = uf->find(k);
		val_t ret = init_val;
		for (auto root : heaps[h].roots) {
			if (root == nullptr) continue;
			ret = std::min(ret, root->add + (root->enable ? root->upd : root->val));
		}
		return ret;
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
	val_t get_value(ptr_t ptr) {
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
				std::swap(vals[i], vals[i + 1]);
				std::swap(ptr_mp1[path[i]], ptr_mp1[path[i + 1]]);
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