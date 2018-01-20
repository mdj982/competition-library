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
    bool operator<(const node & another) const {
      return deg != another.deg ? deg < another.deg : id < another.id;
    }
  };
  int n;
public:
  vector<node> nodes;
  vi leaves;
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
    leaves = {};
    stack<int> stk;
    stk.push(root);
    while (stk.size()) {
      int a = stk.top(); stk.pop();
      if (nodes[a].deg != -1) continue;
      if (a == root) nodes[a].deg = 0;
      int leaf_flag = true;
      Loop(i, edges[a].size()) {
        int b = edges[a][i].first;
        if (nodes[b].deg != -1) {
          nodes[a].parent = b;
          nodes[a].cost = edges[a][i].second;
          nodes[a].deg = nodes[b].deg + 1;
          nodes[a].d = nodes[b].d + nodes[a].cost;
        }
        else {
          leaf_flag = false;
          nodes[a].childs.push_back(b);
          stk.push(b);
        }
      }
      if (leaf_flag) leaves.push_back(a);
    }
    if (n == 1) leaves.push_back(root);
    return;
  }
  pair<int, vi> get_center_of_gravity() {
    pair<int, vi> ret = { INT_MAX,{} };
    vector<node> c_nodes = nodes;
    sort(c_nodes.begin(), c_nodes.end());
    vi record(n, 1);
    Loopr(i, n) {
      int x = n - 1, max_x = INT_MIN;
      Loop(j, c_nodes[i].childs.size()) {
        int b = c_nodes[i].childs[j];
        max_x = max(max_x, record[b]);
        x -= record[b];
        record[c_nodes[i].id] += record[b];
      }
      max_x = max(max_x, x);
      if (max_x < ret.first) ret = { max_x,{ c_nodes[i].id } };
      else if (max_x == ret.first) ret.second.push_back(c_nodes[i].id);
    }
    sort(ret.second.begin(), ret.second.end());
    return ret;
  }
};