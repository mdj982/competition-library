// try to consider set<int>, using set<int>::lower_bound etc.

class _01Trie_Tree {
private:
	struct node {
		bool val; node* childs[2]; int deg; node *parent; int cnt;
	};
	const int height = 32;
	node *root;
	bool multi_flag;
	bool erase_leaf(node *nodeptr) {
		if (nodeptr->deg != 32) return false;
		while (nodeptr != root) {
			bool v = nodeptr->val;
			nodeptr->cnt--;
			nodeptr = nodeptr->parent;
			if (nodeptr->childs[v]->cnt == 0) {
				delete(nodeptr->childs[v]);
				nodeptr->childs[v] = nullptr;
			}
		}
		nodeptr->cnt--;
		return true;
	}
	bool lower_bit(int x, int bitp) {
		return x & (1 << bitp);
	}
	bool upper_bit(int x, int bitp) {
		return x & (1 << (31 - bitp));
	}
public:
	_01Trie_Tree(vi a, bool multi_flag = true) {
		root = new node{ 0,{ nullptr, nullptr }, 0, nullptr, 0 };
		_01Trie_Tree::multi_flag = multi_flag;
		Loop(i, a.size()) add(a[i]);
	}
	void add(int x) {
		node *focus = root;
		Loop(i, height) {
			bool v = upper_bit(x, i);
			if (focus->childs[v] == nullptr) {
				node *node_buf = new node{ v,{ nullptr, nullptr }, focus->deg + 1, focus, 0 };
				focus->childs[v] = node_buf;
			}
			focus->cnt++;
			focus = focus->childs[v];
		}
		focus->cnt++;
		if (!multi_flag && focus->cnt >= 2) erase_leaf(focus);
		return;
	}
	bool find(int x) {
		node *focus = root;
		Loop(i, height) {
			bool v = upper_bit(x, i);
			if (focus->childs[v] == nullptr) return false;
			else focus = focus->childs[v];
		}
		return true;
	}
	bool erase(int x) {
		node *focus = root;
		Loop(i, height) {
			bool v = upper_bit(x, i);
			if (focus->childs[v] == nullptr) return false;
			else focus = focus->childs[v];
		}
		return erase_leaf(focus);
	}
};
