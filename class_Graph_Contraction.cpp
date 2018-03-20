struct graph_t {
  int n;           // |V|, index begins with 0
  int m;           // |E|
  vector<P> edges; // E
  vector<ll> cost; // cost or distance
  vector<ll> cap;  // capacity
};

class Graph_Contraction {
private:
  vi original_node_mapping; // original -> contracted
  vvi contracted_node_mapping; // contracted -> original
  graph_t original_graph;
  graph_t contracted_graph;
  bool fail_flag;
public:
  Graph_Contraction(graph_t G, vvi contract_list, bool allow_self_cycle_flag) {
    original_graph = G;
    original_node_mapping = vi(G.n, -1);
    int node_index = 0;
    Loop(i, contract_list.size()) {
      Loop(j, contract_list[i].size()) {
        if (original_node_mapping[contract_list[i][j]] == -1) {
          original_node_mapping[contract_list[i][j]] = i;
        }
        else {
          fail_flag = true;
          return;
        }
      }
      if (contract_list[i].size() >= 1) {
        contracted_node_mapping.push_back(contract_list[i]);
        node_index++;
      }
    }
    Loop(i, G.n) {
      if (original_node_mapping[i] == -1) {
        original_node_mapping[i] = node_index;
        contracted_node_mapping.push_back({ i });
        node_index++;
      }
    }
    contracted_graph.n = node_index;
    Loop(i, G.edges.size()) {
      int a = original_node_mapping[G.edges[i].first];
      int b = original_node_mapping[G.edges[i].second];
      if (a == b && !allow_self_cycle_flag) continue;
      else {
        contracted_graph.edges.push_back({ a,b });
        if (G.cost.size()) contracted_graph.cost.push_back(G.cost[i]);
        if (G.cap.size()) contracted_graph.cost.push_back(G.cap[i]);
      }
    }
    contracted_graph.m = contracted_graph.edges.size();
    fail_flag = false;
    return;
  }
  bool is_succeeded() {
    return !fail_flag;
  }
  vi get_original_node_mapping() {
    return original_node_mapping;
  }
  vvi get_contracted_node_mapping() {
    return contracted_node_mapping;
  }
  graph_t get_original_graph() {
    return original_graph;
  }
  graph_t get_contracted_graph() {
    return contracted_graph;
  }
};