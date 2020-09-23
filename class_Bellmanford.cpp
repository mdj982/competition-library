#include "auto_util_header.hpp"

class Bellmanford {
private:
	struct node {
		int id; bool done; int from; ll d; vi to; vll cst;
	};
	vector<node> nodes;
	int n, m, src;
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
		src = start;
		nodes[src].d = 0;
		negative_cycle_endpoint = -1;
		Loop(k, n) {
			Loop(i, n) {
				int a = i;
				if (nodes[a].d == LLONG_MAX) continue;
				Loop(j, nodes[a].to.size()) {
					int b = nodes[a].to[j];
					ll buf = nodes[a].d + nodes[a].cst[j];
					if (buf < nodes[b].d) {
						nodes[b].d = buf;
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
		if (a != src) return {};
		std::reverse(ret.begin(), ret.end());
		return ret;
	}
	ll get_dist(int v) {
		return nodes[v].d;
	}
	bool exists_reachable_negative_cycle() {
		return (negative_cycle_endpoint != -1);
	}
	vi get_reachable_negative_cycle() {
		if (negative_cycle_endpoint == -1) return {};
		// modify negative_cycle_endpoint as a point in the negative cycle
		{
			vector<int> done(n);
			int a = negative_cycle_endpoint;
			while (true) {
				done[a] = 1;
				a = nodes[a].from;
				if (done[a]) {
					negative_cycle_endpoint = a;
					break;
				}
			}
		}
		vector<int> ret;
		// get the negative cycle
		{
			int a = negative_cycle_endpoint;
			ret.push_back(a);
			while (true) {
				ret.push_back(nodes[a].from);
				a = nodes[a].from;
				if (a == negative_cycle_endpoint) {
					break;
				}
			}
			std::reverse(ret.begin(), ret.end());
		}
		return ret;
	}
};