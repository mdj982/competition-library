struct tree_t {
  int n;           // |V|, index begins with 0
  int r;           // root
  vector<P> edges; // E
  vector<ll> cost; // cost or distance
};

class Tree {
private:
  struct node {
    int id; vi childs; int parent; ll cost; int deg; ll d;
  };
  int n;
public:
  vector<node> nodes;
  int root;
  Tree(tree_t T) {
    n = T.n;
    root = T.r;
    nodes.resize(n);
    Loop(i, n) nodes[i] = { i,{},-1,0,-1,0 };
    map<int, vector<pair<int, ll>>> edges;
    Loop(i, n - 1) {
      if (T.cost.size() == 0) {
        edges[T.edges[i].first].push_back({ T.edges[i].second,1 });
        edges[T.edges[i].second].push_back({ T.edges[i].first,1 });
      }
      else {
        edges[T.edges[i].first].push_back({ T.edges[i].second,T.cost[i] });
        edges[T.edges[i].second].push_back({ T.edges[i].first,T.cost[i] });
      }
    }
    stack<int> stk;
    stk.push(root);
    while (stk.size()) {
      int a = stk.top(); stk.pop();
      if (nodes[a].deg != -1) continue;
      if (a == root) nodes[a].deg = 0;
      Loop(i, edges[a].size()) {
        int b = edges[a][i].first;
        if (nodes[b].deg != -1) {
          nodes[a].parent = b;
          nodes[a].cost = edges[a][i].second;
          nodes[a].deg = nodes[b].deg + 1;
          nodes[a].d = nodes[b].d + nodes[a].cost;
        }
        else {
          nodes[a].childs.push_back(b);
          stk.push(b);
        }
      }
    }
    return;
  }
};