class AVL_Tree {
	using val_t = long long;
private:
	enum side_t {
		L, R, U
	};
	struct node_t {
		val_t val;
		side_t side;
		node_t* parent;
		std::array<node_t*, 2> childs;
		size_t cnt;
		size_t subtree_n;
		size_t height;
		int rev_local;
		int rev;
		node_t(val_t val, side_t side, node_t* nil) {
			this->val = val;
			this->side = side;
			this->parent = nil;
			this->childs = { nil, nil };
			this->cnt = 1;
			this->subtree_n = 0;
			this->height = 0;
			this->rev_local = 0;
			this->rev = 0;
		}
		inline void update_params() {
			if (this->cnt == 0) return;
			this->subtree_n = 1 + this->childs[0]->subtree_n + this->childs[1]->subtree_n;
			this->height = 1 + std::max(this->childs[0]->height, this->childs[1]->height);
		}
		inline void update_rev() {
			if (this->cnt == 0) return;
			this->rev = this->parent->rev ^ this->rev_local;
		}
	};
	bool multi_flag;
	node_t* root;
	node_t* nil;
	std::unordered_map<val_t, node_t*> mp;
	inline void cut_edge(node_t* a);
	// direction: semantical, a->side := s->parent->rev ^ direction
	inline void lnk_edge(node_t* a, node_t* p, side_t direction);
	// init
	node_t* init(const size_t l, const size_t r, const side_t side, node_t* const p, const std::vector<val_t> &vals);
	// recursively rebalance until root
	inline node_t* rebalance(node_t* a);
	// return replaced root, direction is semantical
	inline node_t* rotate(node_t* a, const side_t direction);
	// return root
	node_t* erase_node(node_t* a);
	// join two trees into one tree, tree_l -> tree_r in in-order, balanced
	node_t* join_trees(std::array<node_t*, 2> trees);
	// split to balanced trees, [, a) [a, )
	std::array<node_t*, 2> split(node_t* a);
	// return the lower bound position (if nil, then return its parent)
	node_t* lower_bound(node_t *a, const val_t &x);
	// return the L/R-most node
	inline node_t* most(node_t* a, const side_t direction, size_t height = 0);
	//
	void traverse_rec(node_t *a, std::vector<val_t> &track);
	// update rev from a to root 
	void update_rev_rec(node_t *a);
public:
	AVL_Tree(const val_t nil_val);
	AVL_Tree(const std::vector<val_t> &vals, const val_t nil_val);
	~AVL_Tree();
	void insert(const val_t x);
	bool erase(const val_t x);
	void cut(const val_t x);
	void reverse(const val_t x);
	// change the cyclic sequence of traversal [ >> x >> z y >> ] -> [ >> z << x y >> ]
	void reverse_cyclic_seq(const val_t x, const val_t y);
	// return the id of in-order sequence whose value is x
	size_t seq_id(const val_t x);
	// return the value of in-order sequence 
	val_t seq_at(const size_t k);
	std::vector<val_t> traverse();
};

AVL_Tree::AVL_Tree(const val_t nil_val) {
	this->nil = new node_t(nil_val, U, nullptr);
	this->nil->parent = this->nil;
	this->nil->childs[0] = this->nil;
	this->nil->childs[1] = this->nil;
	this->nil->cnt = 0;
	this->root = this->nil;
}

AVL_Tree::AVL_Tree(const std::vector<val_t> &vals, const val_t nil_val) {
	this->nil = new node_t(nil_val, U, nullptr);
	this->nil->parent = this->nil;
	this->nil->childs[0] = this->nil;
	this->nil->childs[1] = this->nil;
	this->nil->cnt = 0;
	this->root = this->init(0, vals.size(), U, this->nil, vals);
}

AVL_Tree::~AVL_Tree() {
	for (auto x : this->mp) {
		delete(x.snd);
	}
}

AVL_Tree::node_t* AVL_Tree::init(const size_t l, const size_t r, const side_t side, node_t* const p, const std::vector<val_t> &vals) {
	if (l == r) return nil;
	else {
		size_t m = (l + r) >> 1;
		node_t* a = new node_t(vals[m], side, this->nil);
		this->mp[vals[m]] = a;
		a->parent = p;
		a->childs[L] = this->init(l, m, L, a, vals);
		a->childs[R] = this->init(m + 1, r, R, a, vals);
		a->update_params();
		return a;
	}
}

inline void AVL_Tree::cut_edge(node_t* a) {
	if (a->side == U) return;
	node_t* p = a->parent;
	p->childs[a->side] = this->nil;
	a->side = U;
	a->parent = this->nil;
	a->rev_local = a->rev;
	p->update_params();
}

inline void AVL_Tree::lnk_edge(node_t* a, node_t* p, side_t direction) {
	if (p == this->nil) direction = U;
	if (p != this->nil) {
		p->childs[p->rev ^ direction] = a;
	}
	if (a != this->nil) {
		a->side = side_t(direction != U ? p->rev ^ direction : U);
		a->parent = p;
		a->rev_local = p->rev ^ a->rev;
	}
	p->update_params();
	return;
}

//------------------------
//    direction = R
//
//     p          p
//     |          |
//     a    ->    b
//    /            \
//   b              a
//    \            /
//     c          c
//------------------------
inline AVL_Tree::node_t* AVL_Tree::rotate(node_t* a, const side_t direction) {
	node_t* p = a->parent;
	node_t* b = a->childs[a->rev ^ direction ^ 1];
	b->update_rev();
	node_t* c = b->childs[b->rev ^ direction];
	c->update_rev();
	side_t side = side_t(p->rev ^ a->side);
	this->cut_edge(c);
	this->cut_edge(b);
	this->cut_edge(a);
	this->lnk_edge(c, a, side_t(direction ^ 1));
	this->lnk_edge(a, b, direction);
	this->lnk_edge(b, p, side);
	a->update_params();
	b->update_params();
	p->update_params();
	return b;
}

AVL_Tree::node_t* AVL_Tree::join_trees(std::array<node_t*, 2> trees) {
	if (trees[L] == this->nil) return trees[R];
	if (trees[R] == this->nil) return trees[L];
	if (trees[L]->height == trees[R]->height) {
		node_t* a = this->most(trees[L], R);
		node_t* pa = a->parent;
		this->cut_edge(a);
		node_t* c = a->childs[a->rev ^ L];
		c->update_rev();
		this->cut_edge(c);
		this->lnk_edge(c, pa, R);
		node_t* b = this->rebalance(pa != this->nil ? pa : c);
		a->rev = a->rev_local = 0;
		this->lnk_edge(b, a, L);
		this->lnk_edge(trees[R], a, R);
		return a;
	}
	else {
		side_t side = (trees[L]->height > trees[R]->height ? L : R);
		node_t* a = this->most(trees[side], side_t(side ^ 1), trees[side ^ 1]->height);
		node_t* pa = a->parent;
		this->cut_edge(a);
		node_t* b = this->most(a, side_t(side ^ 1));
		node_t* pb = b->parent;
		this->cut_edge(b);
		node_t* c = b->childs[b->rev ^ side];
		c->update_rev();
		this->cut_edge(c);
		this->lnk_edge(c, pb, side_t(side ^ 1));
		a = this->rebalance(c != this->nil ? c : pb);
		this->lnk_edge(b, pa, side_t(side ^ 1));
		this->lnk_edge(a, b, side);
		this->lnk_edge(trees[side ^ 1], b, side_t(side ^ 1));
		return this->rebalance(b != this->nil ? b : pa);
	}
}

std::array<AVL_Tree::node_t*, 2> AVL_Tree::split(node_t* a) {
	std::array<std::array<node_t*, 128>, 2> trees;
	std::array<size_t, 2> psize = { 0, 0 };
	a->childs[a->rev ^ L]->update_rev();
	trees[L][psize[L]++] = a->childs[a->rev ^ L];
	trees[R][psize[R]++] = a;
	while (a->side != U) {
		side_t side = side_t(a->parent->rev ^ a->side);
		trees[side ^ 1][psize[side ^ 1]++] = a->parent;
		a = a->parent;
	}
	for (size_t i = 0; i < psize[L]; ++i) this->cut_edge(trees[L][i]);
	for (size_t i = 0; i < psize[R]; ++i) this->cut_edge(trees[R][i]);
	// i == 0 && S == R
	{
		node_t* b = trees[R][0];
		node_t* c = b->childs[b->rev ^ R];
		c->update_rev();
		this->cut_edge(c);
		trees[R][0] = this->join_trees({ b, c });
	}
	for (size_t S = 0; S < 2; ++S) {
		for (size_t i = 1; i < psize[S]; ++i) {
			node_t* b = trees[S][i];
			node_t* c = b->childs[b->rev ^ S];
			c->update_rev();
			this->cut_edge(c);
			node_t* d = this->most(c, side_t(S ^ 1), trees[S][i - 1]->height);
			node_t* pd = d->parent;
			this->cut_edge(d);
			this->lnk_edge(b, pd, side_t(S ^ 1));
			this->lnk_edge(d, b, side_t(S));
			this->lnk_edge(trees[S][i - 1], b, side_t(S ^ 1));
			trees[S][i] = this->rebalance(b);
		}
	}
	return { trees[L][psize[L] - 1], trees[R][psize[R] - 1] };
}


AVL_Tree::node_t* AVL_Tree::erase_node(node_t *a) {
	node_t* p = a->parent;
	side_t side = side_t(p->rev ^ a->side);
	node_t* subtree_l = a->childs[a->rev ^ L];
	node_t* subtree_r = a->childs[a->rev ^ R];
	this->cut_edge(subtree_l);
	this->cut_edge(subtree_r);
	this->cut_edge(a);
	this->mp.erase(a->val);
	delete(a);
	node_t* b = this->join_trees({ subtree_l, subtree_r });
	this->lnk_edge(b, p, side);
	return this->rebalance(b);
}

inline AVL_Tree::node_t* AVL_Tree::rebalance(node_t* a) {
	while (true) {
		a->update_params();
		for (int S = 0; S < 2; ++S) {
			if (a->childs[a->rev ^ S]->height + 1 < a->childs[a->rev ^ S ^ 1]->height) {
				node_t* b = a->childs[a->rev ^ S ^ 1];
				b->update_rev();
				if (b->childs[b->rev ^ S]->height > b->childs[b->rev ^ S ^ 1]->height) {
					b = this->rotate(b, side_t(S ^ 1));
				}
				a = this->rotate(a, side_t(S));
			}
		}
		if (a->parent == this->nil) break;
		a = a->parent;
	}
	return a;
}

AVL_Tree::node_t* AVL_Tree::lower_bound(node_t* a, const val_t &x) {
	if (a == this->nil) return this->nil;
	a->update_rev();
	if (x == a->val) {
		return a;
	}
	else {
		side_t side = side_t((x < a->val ? 0 : 1) ^ a->rev);
		if (a->childs[side] == this->nil) return a;
		else return this->lower_bound(a->childs[side], x);
	}
}

inline AVL_Tree::node_t* AVL_Tree::most(node_t* a, const side_t direction, size_t height) {
	while (true) {
		a->update_rev();
		side_t side = side_t(direction ^ a->rev);
		if (a->childs[side] == this->nil || a->height <= height) break;
		else a = a->childs[side];
	}
	return a;
}

void AVL_Tree::update_rev_rec(node_t* a) {
	if (a == this->nil) return;
	else {
		this->update_rev_rec(a->parent);
		a->rev = a->parent->rev ^ a->rev_local;
	}
}

void AVL_Tree::insert(const val_t x) {
	if (this->root == this->nil) {
		this->mp[x] = this->root = new node_t(x, U, this->nil);
		this->root->update_params();
	}
	else {
		node_t *a = this->lower_bound(this->root, x);
		if (x == a->val) {
			if (multi_flag) a->cnt++;
		}
		else {
			side_t side = side_t((x < a->val ? 0 : 1) ^ a->rev);
			this->mp[x] = a->childs[side] = new node_t(x, side, this->nil);
			a->childs[side]->parent = a;
			this->root = this->rebalance(a);
		}
	}
}

bool AVL_Tree::erase(const val_t x) {
	node_t *a = this->lower_bound(this->root, x);
	if (x != a->val) return false;
	else {
		a->cnt--;
		if (a->cnt == 0) {
			this->root = this->erase_node(a);
		}
		return true;
	}
}

void AVL_Tree::cut(const val_t x) {
	node_t *a = this->mp[x];
	this->update_rev_rec(a);
	std::array<node_t*, 2> trees = this->split(a);
	std::swap(trees[0], trees[1]);
	this->root = this->join_trees(trees);
}

void AVL_Tree::reverse(const val_t x) {
	node_t *a = this->mp[x];
	this->update_rev_rec(a);
	std::array<node_t*, 2> trees = this->split(a);
	trees[R]->rev_local ^= 1;
	trees[R]->update_rev();
	this->root = this->join_trees(trees);
}

void AVL_Tree::traverse_rec(node_t* a, std::vector<val_t> &track) {
	if (a == this->nil) return;
	a->rev = a->parent->rev ^ a->rev_local;
	this->traverse_rec(a->childs[a->rev ^ L], track);
	track.push_back(a->val);
	this->traverse_rec(a->childs[a->rev ^ R], track);
}

std::vector<AVL_Tree::val_t> AVL_Tree::traverse() {
	std::vector<val_t> ret;
	this->traverse_rec(this->root, ret);
	return ret;
}

void AVL_Tree::reverse_cyclic_seq(const val_t x, const val_t y) {
	this->cut(y);
	this->reverse(x);
}

size_t AVL_Tree::seq_id(const val_t x) {
	node_t *a = this->mp[x];
	this->update_rev_rec(a);
	size_t ret = a->childs[a->rev ^ L]->subtree_n;
	while (a->side != U) {
		side_t side = side_t(a->parent->rev ^ a->side);
		if (side == R) {
			node_t *p = a->parent;
			ret += 1 + p->childs[p->rev ^ L]->subtree_n;
		}
		a = a->parent;
	}
	return ret;
}

AVL_Tree::val_t AVL_Tree::seq_at(const size_t k) {
	node_t *a = this->root;
	a->update_rev();
	size_t p = a->childs[a->rev ^ L]->subtree_n;
	while (true) {
		if (k < p) {
			node_t *b = a->childs[a->rev ^ L];
			b->update_rev();
			p -= 1 + b->childs[b->rev ^ R]->subtree_n;
			a = b;
		}
		else if (p < k) {
			node_t *b = a->childs[a->rev ^ R];
			b->update_rev();
			p += 1 + b->childs[b->rev ^ L]->subtree_n;
			a = b;
		}
		else break;
	}
	return a->val;
}