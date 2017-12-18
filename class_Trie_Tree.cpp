class Trie_Tree {
private:
  struct node {
    char val; map<char, node*> childs_mp; ll deg;
  };
  node *root;
public:
  Trie_Tree(vector<string> s) {
    root = new node{ '\0',{}, 0 };
    Loop(i, s.size()) add_str(s[i]);
  }
  void add_str(string s) {
    node *focus = root;
    Loop(i, s.length()) {
      char c = s[i];
      if (focus->childs_mp[c] == nullptr) {
        node *node_buf = new node{ c,{},focus->deg + 1 };
        focus->childs_mp[c] = node_buf;
      }
      focus = focus->childs_mp[c];
    }
    node *nil = new node{ '\0',{}, focus->deg + 1 };
    focus->childs_mp['\0'] = nil;
  }
  bool find_str(string s) {
    node *focus = root;
    Loop(i, s.length()) {
      char c = s[i];
      if (focus->childs_mp[c] == nullptr) return false;
      else focus = focus->childs_mp[c];
    }
    if (focus->childs_mp['\0'] != nullptr) return true;
    else return false;
  }
};