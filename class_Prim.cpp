#include "auto_util_header.hpp"

class Prim {
private:
	struct node {
		int id; bool done; vi to; vll cst; int from; ll d;
	};
	struct pq_t {
		int id; ll d;
		bool operator<(const pq_t & another) const {
			return d != another.d ? d > another.d : id > another.id;
		}
	};
	vector<node> nodes;
	int n, m;
public:
	Prim(const vvi &lst, const vvll &cst) {
		n = lst.size();
		nodes.resize(n);
		Loop(i, n) nodes[i] = { i, false,{},{}, -1, LLONG_MAX };
		Loop(i, n) {
			Loop(j, lst[i].size()) {
				nodes[i].to.push_back(lst[i][j]);
				nodes[i].cst.push_back(cst[i][j]);
			}
		}
		nodes[0].d = 0;
		priority_queue<pq_t> pq;
		pq.push({ nodes[0].id, nodes[0].d });
		while (pq.size()) {
			int a = pq.top().id;
			pq.pop();
			if (nodes[a].done) continue;
			nodes[a].done = true;
			Loop(j, nodes[a].to.size()) {
				int b = nodes[a].to[j];
				if (nodes[b].done) continue;
				ll buf = nodes[a].cst[j];
				if (buf < nodes[b].d) {
					nodes[b].d = buf;
					nodes[b].from = a;
					pq.push({ b, nodes[b].d });
				}
			}
		}
		return;
	}
	vector<P> get_result() {
		vector<P> ret;
		Loop1(i, n - 1) {
			int a = i;
			int b = nodes[i].from;
			if (a > b) std::swap(a, b);
			ret.push_back({ a, b });
		}
	}
	ll get_weight() {
		ll ret = 0;
		Loop(i, n) {
			ret += nodes[i].d;
		}
		return ret;
	}
};