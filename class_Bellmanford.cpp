#include "auto_util_header.hpp"

class Bellmanford {
private:
	struct node {
		int id; bool done; int from; ll d; vi to; vll cst;
	};
	vector<node> nodes;
	int n, m, source;
	int negative_cycle_endpoint; // -1 if there is no negative cycle
public:
	Bellmanford(const vvi &lst, const vvll &cst, int start) {
		n = lst.size();
		nodes.resize(n);
		Loop(i, n) nodes[i] = { i, false, -1, LLONG_MAX, {}, {} };
		Loop(i, n) {
			Loop(j, lst[i].size()) {
				nodes[i].to.push_back(lst[i][j]);
				nodes[i].cst.push_back(cst[i][j]);
			}
		}
		source = start;
		nodes[source].d = 0;
		negative_cycle_endpoint = -1;
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
							negative_cycle_endpoint = a;
							return;
						}
					}
				}
			}
		}
	}
	vi get_path(int v) {
		vector<int> ret;
		int a = v;
		ret.push_back(a);
		while (nodes[a].from != -1) {
			ret.push_back(nodes[a].from);
			a = nodes[a].from;
		}
		if (a != source) return {};
		std::reverse(ret.begin(), ret.end());
		return ret;
	}
	ll get_dist(int v) {
		return nodes[v].d;
	}
	bool exists_negative_cycle() {
		return (negative_cycle_endpoint != -1);
	}
	vi get_negative_cycle() {
		if (negative_cycle_endpoint == -1) return {};
		vector<int> ret;
		int a = negative_cycle_endpoint;
		ret.push_back(a);
		while (nodes[a].from != negative_cycle_endpoint) {
			ret.push_back(nodes[a].from);
			a = nodes[a].from;
		}
		std::reverse(ret.begin(), ret.end());
		return ret;
	}
};