typedef ll val_t;

struct graph_t {
  int n;           // |V|, index begins with 0
  int m;           // |E|
  vector<P> edges; // E
	vector<val_t> vals;
  vector<ll> costs; // cost or distance
  vector<ll> caps;  // capacity
};

class Union_Find {
private:
  vi parent, rank;
public:
  Union_Find(int N) {
    parent.resize(N);
    rank.resize(N);
    Loop(i, N) {
      parent[i] = i;
      rank[i] = 0;
    }
  }
  int find(int x) {
    if (parent[x] == x) return x;
    else return parent[x] = find(parent[x]);
  }
  void unite(int x, int y) {
    x = find(x);
    y = find(y);
    if (x == y) return;
    if (rank[x] < rank[y]) parent[x] = y;
    else {
      parent[y] = x;
      if (rank[x] == rank[y]) rank[x]++;
    }
  }
  bool is_same(int x, int y) {
    return find(x) == find(y);
  }
};

class Strongly_Connected_Components {
private:
  struct node {
    int id; bool done; vi to_eid; vi to; int from_eid; int from;
  };
  vector<node> nodes[2];
  int n;
  stack<int> stk;
  vvi sccs;
  vvi ccs;
  vi scc_gid;
  vi cc_gid;
  void scc_dfs(int a, int is_r) {
    nodes[is_r][a].done = true;
    Loop(i, nodes[is_r][a].to.size()) {
      int b = nodes[is_r][a].to[i];
      if (b == nodes[is_r][a].from) continue;
      if (!nodes[is_r][b].done) {
        nodes[is_r][b].from = a;
        scc_dfs(b, is_r);
      }
    }
    if (is_r == 0) stk.push(a);
    else {
      sccs.back().push_back(a);
    }
    return;
  }
public:
  Strongly_Connected_Components(graph_t G) {
    n = G.n;
    Loop(i, 2) nodes[i].resize(n);
    Loop(i, 2) {
      Loop(j, n) nodes[i][j] = { i, false,{},{}, -1, -1 };
    }
    Union_Find *uf = new Union_Find(n);
    Loop(i, G.edges.size()) {
      nodes[0][G.edges[i].first].to_eid.push_back(i);
      nodes[0][G.edges[i].first].to.push_back(G.edges[i].second);
      nodes[1][G.edges[i].second].to_eid.push_back(i);
      nodes[1][G.edges[i].second].to.push_back(G.edges[i].first);
      uf->unite(G.edges[i].first, G.edges[i].second);
    }
    map<int, vi> cc_mp;
    Loop(i, n) cc_mp[uf->find(i)].push_back(i);
    Loopitr(itr, cc_mp) ccs.push_back(itr->second);
    Loop(i, ccs.size()) {
      Loop(j, ccs[i].size()) {
        if (!nodes[0][ccs[i][j]].done) scc_dfs(ccs[i][j], 0);
      }
    }
    while (stk.size()) {
      int a = stk.top(); stk.pop();
      if (!nodes[1][a].done) {
        sccs.push_back({});
        scc_dfs(a, 1);
        sort(sccs.back().begin(), sccs.back().end());
      }
    }
    return;
  }
  // already in topological order
  vvi get_sccs() {
    return sccs;
  }
  vvi get_ccs() {
    return ccs;
  }
  vi get_scc_gid() {
    if (scc_gid.empty()) {
      scc_gid.resize(n);
      Loop(i, sccs.size()) {
        Loop(j, sccs[i].size()) {
          scc_gid[sccs[i][j]] = i;
        }
      }
    }
    return scc_gid;
  }
  vi get_cc_gid() {
    if (cc_gid.empty()) {
      cc_gid.resize(n);
      Loop(i, ccs.size()) {
        Loop(j, ccs[i].size()) {
          cc_gid[ccs[i][j]] = i;
        }
      }
    }
    return cc_gid;
  }
};

struct cnf2_t {
  int n;
  struct literal_t {
    int index;
    bool neg;
  };
  struct clause_t {
    literal_t x, y;
  };
  vector<clause_t> L;
};

class SAT2 {
private:
  int n;
  bool fail_flag;
  graph_t G;
  vvi sccs;
  vi scc_gid;
  vector<bool> result;
  int inv(int id) {
    return (id + n) % G.n;
  }
public:
  SAT2(cnf2_t CNF) {
    n = CNF.n;
    G.n = n * 2;
    G.m = CNF.L.size() * 2;
    Loop(i, CNF.L.size()) {
      G.edges.push_back({ CNF.L[i].x.index + (CNF.L[i].x.neg ? 0 : n), CNF.L[i].y.index + (CNF.L[i].y.neg ? n : 0) });
      G.edges.push_back({ CNF.L[i].y.index + (CNF.L[i].y.neg ? 0 : n), CNF.L[i].x.index + (CNF.L[i].x.neg ? n : 0) });
    }
    Strongly_Connected_Components *scc = new Strongly_Connected_Components(G);
    sccs = scc->get_sccs();
    scc_gid = scc->get_scc_gid();
    fail_flag = false;
    result.resize(n);
    Loop(i, n) {
      if (scc_gid[i] > scc_gid[inv(i)]) result[i] = true;
      else if (scc_gid[i] < scc_gid[inv(i)]) result[i] = false;
      else {
        result.clear();
        fail_flag = true;
        return;
      }
    }
    return;
  }
  bool is_satisfiable() {
    return !fail_flag;
  }
  vector<bool> get_result() {
    return result;
  }
};