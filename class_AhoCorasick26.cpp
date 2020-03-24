class AhoCorasick26 {
private:
	// cnt := #{the strings at this node}
	// suf := the deepest node which is a suffix of this node
	struct node_t {
		char val; array<node_t*, 26> to; int dep; node_t *parent; int cnt; node_t *suf;
	};
	const char base = 'a';
	node_t *root;
	void make_Trie(const vector<string> &strs) {
		Foreach(s, strs) {
			node_t *a = root;
			Loop(i, s.length()) {
				char c = s[i];
				if (a->to[c - base] == nullptr) {
					node_t *node_buf = new node_t{ c,{}, a->dep + 1, a, 0, nullptr };
					a->to[c - base] = node_buf;
				}
				a = a->to[c - base];
			}
			a->cnt++;
		}
	}
	void construct_by_bfs() {
		queue<node_t*> que;
		que.push(root);
		while (que.size()) {
			node_t *a = que.front(); que.pop();
			Loop(i, 26) {
				if (a->to[i] != nullptr) que.push(a->to[i]);
			}
			if (a->dep <= 1) {
				a->suf = root;
			}
			else {
				a->suf = a->parent->suf->to[a->val - base];
			}
			if (a->dep == 0) {
				Loop(i, 26) if (a->to[i] == nullptr) a->to[i] = a;
			}
			else {
				Loop(i, 26) if (a->to[i] == nullptr) a->to[i] = a->suf->to[i];
			}
		}
	}
public:
	AhoCorasick26(const vector<string> &s) {
		root = new node_t{ '\0',{}, 0, nullptr, 0, nullptr };
		make_Trie(s);
		construct_by_bfs();
	}
	// cnt := #{the strings which have a suffix of this node}
	void accumulate_cnt() {
		queue<node_t*> que;
		que.push(root);
		while (que.size()) {
			node_t *a = que.front(); que.pop();
			Loop(i, 26) {
				if (a->to[i]->dep > a->dep) que.push(a->to[i]);
			}
			a->cnt += a->suf->cnt;
		}
	}
};