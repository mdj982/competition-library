class Trie_Tree {
private:
  struct node {
    char val; map<char, node*> childs_mp; ll deg; node *parent; int cnt;
  };
  bool erase_leaf(node *nodeptr) {
    if (nodeptr->val != '\0') {
      do {
        char v = nodeptr->val;
        nodeptr->cnt--;
        nodeptr = nodeptr->parent;
        if (nodeptr->childs_mp[v]->cnt == 0) {
          delete(nodeptr->childs_mp[v]);
          nodeptr->childs_mp.erase(v);
        }
      } while (nodeptr != root);
      nodeptr->cnt--;
      return true;
    }
    else return false;
  }
  node *root;
  bool multi_flag;
public:
  Trie_Tree(vector<string> s, bool multi_flag) {
    root = new node{ '\0',{}, 0, nullptr, 0 };
    Loop(i, s.size()) add_str(s[i]);
    Trie_Tree::multi_flag = multi_flag;
  }
  void add_str(string s) {
    node *focus = root;
    Loop(i, s.length()) {
      char c = s[i];
      if (focus->childs_mp.find(c) == focus->childs_mp.end()) {
        node *node_buf = new node{ c,{},focus->deg + 1, focus, 0 };
        focus->childs_mp[c] = node_buf;
      }
      focus->cnt++;
      focus = focus->childs_mp[c];
    }
    if (focus->childs_mp.find('\0') == focus->childs_mp.end()) {
      node *nil = new node{ '\0',{}, focus->deg + 1, focus, 0 };
      focus->childs_mp['\0'] = nil;
    }
    focus->cnt++;
    focus = focus->childs_mp['\0'];
    focus->cnt++;
    if (!multi_flag && focus->cnt >= 2) erase_leaf(focus);
  }
  bool find_str(string s) {
    node *focus = root;
    Loop(i, s.length()) {
      char c = s[i];
      if (focus->childs_mp.find(c) == focus->childs_mp.end()) return false;
      else focus = focus->childs_mp[c];
    }
    if (focus->childs_mp.find('\0') != focus->childs_mp.end()) return true;
    else return false;
  }
  bool erase_str(string s) {
    node *focus = root;
    Loop(i, s.length()) {
      char c = s[i];
      if (focus->childs_mp.find(c) == focus->childs_mp.end()) return false;
      else focus = focus->childs_mp[c];
    }
    if (focus->childs_mp.find('\0') != focus->childs_mp.end()) {
      if (erase_leaf(focus)) return true;
      else return false;
    }
    else return false;
  }
};