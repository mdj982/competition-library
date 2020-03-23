class Trie26 {
private:
	struct node {
		char val; array<node*, 27> childs; ll deg; node *parent; int cnt;
	};
	const char base = 'a';
	const int eos = 26;
	bool erase_leaf(node *ptr) {
		if (ptr->val == '\0') {
			{
				char v = ptr->val;
				ptr->cnt--;
				ptr = ptr->parent;
				if (ptr->childs[eos]->cnt == 0) {
					delete(ptr->childs[eos]);
					ptr->childs[eos] = nullptr;
				}
			}
			while (ptr != root) {
				char v = ptr->val;
				ptr->cnt--;
				ptr = ptr->parent;
				if (ptr->childs[v - base]->cnt == 0) {
					delete(ptr->childs[v - base]);
					ptr->childs[v - base] = nullptr;
				}
			}
			ptr->cnt--;
			return true;
		}
		else return false;
	}
	node *root;
	bool multi_flag;
public:
	Trie26(bool multi_flag) {
		root = new node{ '\0',{}, 0, nullptr, 0 };
		Trie26::multi_flag = multi_flag;
	}
	void add(const string &s) {
		node *a = root;
		Loop(i, s.length()) {
			char c = s[i];
			if (a->childs[c - base] == nullptr) {
				node *node_buf = new node{ c,{},a->deg + 1, a, 0 };
				a->childs[c - base] = node_buf;
			}
			a->cnt++;
			a = a->childs[c - base];
		}
		if (a->childs[eos] == nullptr) {
			node *nil = new node{ '\0',{}, a->deg + 1, a, 0 };
			a->childs[eos] = nil;
		}
		a->cnt++;
		a = a->childs[eos];
		a->cnt++;
		if (!multi_flag && a->cnt >= 2) erase_leaf(a);
	}
	bool find(const string &s) {
		node *a = root;
		Loop(i, s.length()) {
			char c = s[i];
			if (a->childs[c - base] == nullptr) return false;
			else a = a->childs[c - base];
		}
		if (a->childs[eos] != nullptr) return true;
		else return false;
	}
	bool erase(const string &s) {
		node *a = root;
		Loop(i, s.length()) {
			char c = s[i];
			if (a->childs[c - base] == nullptr) return false;
			else a = a->childs[c - base];
		}
		if (a->childs[eos] != nullptr) {
			a = a->childs[eos];
			if (erase_leaf(a)) return true;
			else return false;
		}
		else return false;
	}
};