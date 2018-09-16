typedef ll val_t;

struct graph_t {
	int n;           // |V|, index begins with 0
	int m;           // |E|
	vector<P> edges; // E
	vector<val_t> vals; // V
	vector<ll> costs; // cost or distance
	vector<ll> caps;  // capacity
};

class Mincost_Maxflow {
private:
  struct edgedata {
    int eid, from, to;
    ll cost, cap;
    edgedata* dual_p;
    bool operator<(const edgedata & another) const {
      return (cost != another.cost ? cost < another.cost : cap > another.cap);
    }
  };
  struct node {
    int id; bool done; edgedata* fromedge_p; list<edgedata> edges; ll d;
    bool operator<(const node & another) const {
      return !(d != another.d ? d < another.d : id < another.id);
    }
  };
  vector<node> nodes;
  int n, m;
  int source, sink;
  edgedata* empty_edge;
  bool decision;
public:
  Mincost_Maxflow(graph_t G, int s, int t, ll flow) {
    n = G.n;
    m = G.m;
    nodes.resize(n);
    empty_edge = new edgedata;
    Loop(i, n) nodes[i] = { i, false, empty_edge,{}, LLONG_MAX };
    Loop(i, G.edges.size()) {
      int a = G.edges[i].first;
      int b = G.edges[i].second;
      nodes[a].edges.push_back({ i, a, b, G.costs[i], G.caps[i], empty_edge });
      nodes[b].edges.push_back({ i - m, b, a, -1 * G.costs[i], 0, &(nodes[a].edges.back()) });
      nodes[a].edges.back().dual_p = &(nodes[b].edges.back());
    }
    source = s;
    sink = t;
    Loop(i, n) nodes[i].edges.sort();
    ll df;
    ll sumf = 0;
    while (1) {
      Loop(i, n) {
        nodes[i].d = LLONG_MAX;
        nodes[i].fromedge_p = empty_edge;
      }
      nodes[source].d = 0;
      Loop(k, n) {
        Loop(i, n) {
          node *a = &nodes[i];
          for (auto itr = (*a).edges.begin(); itr != (*a).edges.end(); ++itr) {
            if ((*itr).cap == 0) continue;
            node *b = &nodes[(*itr).to];
            if ((*a).d != LLONG_MAX && (*a).d + (*itr).cost < (*b).d) {
              if (k == n - 1) { decision = false; return; }
              (*b).d = (*a).d + (*itr).cost;
              (*b).fromedge_p = &(*itr);
            }
          }
        }
      }
      if (nodes[sink].d == LLONG_MAX) break;
      df = LLONG_MAX;
      int focus = sink;
      while (focus != source) {
        df = min(df, (*(nodes[focus].fromedge_p)).cap);
        focus = (*(nodes[focus].fromedge_p)).from;
      }
      df = min(df, flow - sumf);
      focus = sink;
      while (focus != source) {
        (*(nodes[focus].fromedge_p)).cap -= df;
        (*((*(nodes[focus].fromedge_p)).dual_p)).cap += df;
        focus = (*(nodes[focus].fromedge_p)).from;
      }
      sumf += df;
      if (sumf == flow) { decision = true; return; }
    }
    decision = false; return;
  }
  vll get_eid_flow() {
    if (!decision) return{};
    vll ret(m);
    Loop(i, n) {
      for (auto itr = nodes[i].edges.begin(); itr != nodes[i].edges.end(); ++itr) {
        if ((*itr).eid < 0) ret[(*itr).eid + m] = (*itr).cap;
      }
    }
    return ret;
  }
  ll get_cost() {
    if (!decision) return -1;
    ll ret = 0;
    Loop(i, n) {
      for (auto itr = nodes[i].edges.begin(); itr != nodes[i].edges.end(); ++itr) {
        if ((*itr).eid < 0) ret -= (*itr).cost * (*itr).cap;
      }
    }
    return ret;
  }
  ll get_maxflow() {
    ll ret = 0;
    for (auto itr = nodes[sink].edges.begin(); itr != nodes[sink].edges.end(); ++itr) {
      if ((*itr).eid < 0) ret += (*itr).cap;
    }
    return ret;
  }
};

int main() {
  graph_t G;
  cin >> G.n >> G.m;
  ll f; cin >> f;
  Loop(i, G.m) {
    int u, v;
    ll c, d;
    cin >> u >> v >> c >> d;
    G.edges.push_back({ u, v });
    G.caps.push_back(c);
    G.costs.push_back(d);
  }
  Mincost_Maxflow mincost_maxflow(G, 0, G.n - 1, f);
  cout << mincost_maxflow.get_cost() << endl;
  return 0;
}