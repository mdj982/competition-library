#include "auto_util_header.hpp"

class Trie26 {
private:
	// cnt := #{the strings passing through this node}
	struct node_t {
		char val; array<node_t*, 27> childs; int dep; node_t *parent; int cnt;
	};
	const char base = 'a';
	const int eos = 26;
	bool erase_leaf(node_t *ptr) {
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
	node_t *root;
	bool multi_flag;
public:
	Trie26(bool multi_flag) {
		root = new node_t{ '\0',{}, 0, nullptr, 0 };
		Trie26::multi_flag = multi_flag;
	}
	void add(const string &s) {
		node_t *a = root;
		Loop(i, s.length()) {
			char c = s[i];
			if (a->childs[c - base] == nullptr) {
				node_t *node_buf = new node_t{ c,{},a->dep + 1, a, 0 };
				a->childs[c - base] = node_buf;
			}
			a->cnt++;
			a = a->childs[c - base];
		}
		if (a->childs[eos] == nullptr) {
			node_t *nil = new node_t{ '\0',{}, a->dep + 1, a, 0 };
			a->childs[eos] = nil;
		}
		a->cnt++;
		a = a->childs[eos];
		a->cnt++;
		if (!multi_flag && a->cnt >= 2) erase_leaf(a);
	}
	bool find(const string &s) {
		node_t *a = root;
		Loop(i, s.length()) {
			char c = s[i];
			if (a->childs[c - base] == nullptr) return false;
			else a = a->childs[c - base];
		}
		if (a->childs[eos] != nullptr) return true;
		else return false;
	}
	bool erase(const string &s) {
		node_t *a = root;
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