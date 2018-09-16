typedef ll val_t;

struct graph_t {
	int n;           // |V|, index begins with 0
	int m;           // |E|
	vector<P> edges; // E
	vector<val_t> vals; // V
	vector<ll> costs; // cost or distance
	vector<ll> caps;  // capacity
};

class Maxflow {
private:
  struct edgedata {
    int eid, from, to;
    ll cap;
    edgedata* dual_p;
    bool operator<(const edgedata & another) const {
      return (cap != another.cap ? cap > another.cap : eid < another.eid);
    }
  };
  struct node {
    int id; bool done; list<edgedata> edges; ll d;
    bool operator<(const node & another) const {
      return !(d != another.d ? d < another.d : id < another.id);
    }
  };
  vector<node> nodes;
  int n, m;
  int source, sink;
  edgedata* empty_edge;
public:
  Maxflow(graph_t G, int s, int t) {
    n = G.n;
    m = G.m;
    nodes.resize(n);
    Loop(i, n) nodes[i] = { i, false,{}, LLONG_MAX };
    empty_edge = new edgedata;
    Loop(i, G.edges.size()) {
      int a = G.edges[i].first;
      int b = G.edges[i].second;
      nodes[a].edges.push_back({ i, a, b, G.caps[i], empty_edge });
      nodes[b].edges.push_back({ i - m, b, a, 0, &(nodes[a].edges.back()) });
      nodes[a].edges.back().dual_p = &(nodes[b].edges.back());
    }
    source = s;
    sink = t;
    Loop(i, n) nodes[i].edges.sort();
    vector<pair<int, edgedata*>> stk;
    int a; // the node which is focused on
    ll df; // how much flow in one operation
    while (1) {
      Loop(i, n) nodes[i].done = false;
      a = source;
      nodes[source].done = true;
      while (a != sink) {
        int b = -1;
        Loopitr(itr, nodes[a].edges) {
          if (itr->cap > 0) {
            b = itr->to;
            if (nodes[b].done) b = -1;
            else {
              stk.push_back(make_pair(a, &(*itr)));
              nodes[b].done = true;
              break;
            }
          }
        }
        if (b == -1) {
          if (stk.empty()) break;
          a = stk.back().first; stk.pop_back();
        }
        else a = b;
      }
      if (stk.empty()) break;
      df = LLONG_MAX;
      Loop(i, stk.size()) df = min(df, (*(stk[i].second)).cap);
      while (stk.size()) {
        (*(stk.back().second)).cap -= df;
        (*((*(stk.back().second)).dual_p)).cap += df;
        stk.pop_back();
      }
    }
    return;
  }
  vll get_eid_flow() {
    vll ret(m);
    Loop(i, n) {
      Loopitr(itr, nodes[i].edges) {
        if (itr->eid < 0) ret[itr->eid + m] = itr->cap;
      }
    }
    return ret;
  }
  ll get_maxflow() {
    ll ret = 0;
    Loopitr(itr, nodes[sink].edges) {
      if (itr->eid < 0) ret += itr->cap;
    }
    return ret;
  }
};

int main() {
  graph_t G;
  cin >> G.n >> G.m;
  Loop(i, G.m) {
    int u, v;
    ll c;
    cin >> u >> v >> c;
    G.edges.push_back({ u, v });
    G.caps.push_back(c);
  }
  Maxflow maxflow(G, 0, G.n - 1);
  cout << maxflow.get_maxflow() << endl;
  return 0;
}