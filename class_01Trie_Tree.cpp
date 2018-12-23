class _01Trie_Tree {
private:
	struct node {
		int val; node* childs[2]; int deg; node *parent; int cnt;
	};
	int height;
	node *root;
	bool multi_flag;
	bool erase_leaf(node *nodeptr) {
		if (nodeptr->deg != height) return false;
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
	int lower_bit(ll x, int bitp) {
		return (x >> bitp) & 1LL;
	}
	int upper_bit(ll x, int bitp) {
		return (x >> (height - 1 - bitp)) & 1LL;
	}
public:
	_01Trie_Tree(vll a, int height = 63, bool multi_flag = true) {
		_01Trie_Tree::height = height;
		root = new node{ 0,{ nullptr, nullptr }, 0, nullptr, 0 };
		_01Trie_Tree::multi_flag = multi_flag;
		Loop(i, a.size()) add(a[i]);
	}
	void add(ll x) {
		node *focus = root;
		Loop(i, height) {
			int v = upper_bit(x, i);
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
	bool find(ll x) {
		node *focus = root;
		Loop(i, height) {
			int v = upper_bit(x, i);
			if (focus->childs[v] == nullptr) return false;
			else focus = focus->childs[v];
		}
		return true;
	}
	bool erase(ll x) {
		node *focus = root;
		Loop(i, height) {
			bool v = upper_bit(x, i);
			if (focus->childs[v] == nullptr) return false;
			else focus = focus->childs[v];
		}
		return erase_leaf(focus);
	}
	ll prior_find(ll x) {
		node *focus = root;
		if (focus->cnt == 0) return -1;
		ll ret = 0;
		Loop(i, height) {
			int v = upper_bit(x, i);
			if (focus->childs[v] == nullptr) v ^= 1;
			ret += ((ll)v << (height - 1 - i));
			focus = focus->childs[v];
		}
		return ret;
	}
};