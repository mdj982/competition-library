#include "auto_util_header.hpp"

class Bellmanford {
private:
	struct node {
		int id; bool done; vi to; vll cst; int from; ll d;
	};
	vector<node> nodes;
	int n, m, source;
	bool negative_cycle;
public:
	Bellmanford(const vvi &lst, const vvll &cst, int start) {
		n = lst.size();
		nodes.resize(n);
		Loop(i, n) nodes[i] = { i, false,{},{}, -1, LLONG_MAX };
		Loop(i, n) {
			Loop(j, lst[i].size()) {
				nodes[i].to.push_back(lst[i][j]);
				nodes[i].cst.push_back(cst[i][j]);
			}
		}
		source = start;
		nodes[source].d = 0;
		Loop(k, n) {
			Loop(i, n) {
				int a = i;
				if (nodes[a].d == LLONG_MAX) continue;
				Loop(j, nodes[a].to.size()) {
					int b = nodes[a].to[j];
					if (nodes[a].d + nodes[a].cst[j] < nodes[b].d) {
						nodes[b].d = nodes[a].d + nodes[a].cst[j];
						nodes[b].from = nodes[a].id;
						if (k == n - 1) {
							negative_cycle = true;
							return;
						}
					}
				}
			}
		}
		negative_cycle = false;
		return;
	}
	vi get_path(int v) {
		vector<int> stk;
		stk.push_back(v);
		int a = v;
		while (nodes[a].from != -1) {
			stk.push_back(nodes[a].from);
			a = nodes[a].from;
		}
		if (a != source) return{ -1 };
		vi ret;
		while (stk.size()) {
			ret.push_back(stk.back());
			stk.pop_back();
		}
		return ret;
	}
	ll get_dist(int v) {
		return nodes[v].d;
	}
	bool is_negative_cycle() {
		return negative_cycle;
	}
};