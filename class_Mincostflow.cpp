#include "auto_util_header.hpp"

// O({f}{m}{n}) to fill negative cycles
// O({f}{m}\log{n}) to fill positive paths
class Mincostflow {
private:
	struct edge_t {
		int from; int to; ll cap; 
		ll cst; // original cost
		ll dif; // difference considering potential
		edge_t* rev;
	};
	struct node {
		int id; bool done; edge_t* e_from;
		ll dst; // distance considering potential
		ll pot; // potential
		list<edge_t*> edges;
	};
	struct pq_t {
		int id; ll d;
		bool operator<(const pq_t & another) const {
			return d != another.d ? d > another.d : id > another.id;
		}
	};
	void add_edge(int a, int b, ll cap, ll cst) {
		edge_t *e_pre;
		edge_t *e_rev;
		e_pre = new edge_t({ a, b, cap, cst, cst, nullptr });
		e_rev = new edge_t({ b, a, 0, -cst, -cst, nullptr });
		e_pre->rev = e_rev;
		e_rev->rev = e_pre;
		nodes[a].edges.push_back(e_pre);
		nodes[b].edges.push_back(e_rev);
	}
	void update_potential_by_Bellman_Ford(int s) {
		Loop(i, n) {
			nodes[i].done = false;
			nodes[i].e_from = nullptr;
			nodes[i].dst = LLONG_MAX;
		}
        // solve distance from source node, and also check reachable negative cycles
        {
            nodes[s].dst = 0;
            nodes[s].done = true;
            negative_cycle_endpoint = -1;
            Loop(k, n) {
                Loop(i, n) {
                    int a = i;
                    if (nodes[a].dst == LLONG_MAX) continue;
                    for (edge_t* e : nodes[a].edges) {
                        if (e->cap == 0) continue;
                        int b = e->to;
                        nodes[b].done |= nodes[a].done;
                        ll buf = nodes[a].dst + e->dif;
                        if (buf < nodes[b].dst) {
                            nodes[b].dst = buf;
                            nodes[b].e_from = e;
                            if (k == n - 1) {
                                negative_cycle_endpoint = a;
                                return;
                            }
                        }
                    }
                }
            }
        }
        // check unreachable negative cycles
        {
            Loop(i, n) {
                if (!nodes[i].done) nodes[i].dst = 0;
            }
            Loop(k, n) {
                Loop(a, n) {
                    if (nodes[a].done) continue; // skip reachable nodes
                    if (nodes[a].dst == LLONG_MAX) continue;
                    for (edge_t* e : nodes[a].edges) {
                        int b = e->to;
                        if (e->cap == 0) continue; // skip hidden edges
                        if (nodes[b].done) continue; // skip reachable nodes
                        ll buf = nodes[a].dst + e->dif;
                        if (buf < nodes[b].dst) {
                            nodes[b].dst = buf;
                            nodes[b].e_from = e;
                            if (k == n - 1) {
                                negative_cycle_endpoint = a;
                                return;
                            }
                        }
                    }
                }
            }
        }
		Loop(i, n) {
			// When node i is unreachable, the potential value will never be used.
			// node i is reachable iff (nodes[i].done == true)
			nodes[i].pot += nodes[i].dst;
		}
		return;
	}
	void update_potential_by_Dijkstra(int s) {
		Loop(i, n) {
			nodes[i].done = false;
			nodes[i].e_from = nullptr;
			nodes[i].dst = LLONG_MAX;
		}
		nodes[s].dst = 0;
		priority_queue<pq_t> pq;
		pq.push({ nodes[s].id, nodes[s].dst });
		while (pq.size()) {
			int a = pq.top().id;
			pq.pop();
			if (nodes[a].done) continue;
			nodes[a].done = true;
			for (edge_t* e : nodes[a].edges) {
				if (e->cap == 0) continue;
				int b = e->to;
				if (nodes[b].done) continue;
				ll buf = nodes[a].dst + e->dif;
				if (buf < nodes[b].dst) {
					nodes[b].dst = buf;
					nodes[b].e_from = e;
					pq.push({ b, nodes[b].dst });
				}
			}
		}
		Loop(i, n) {
			// When node i is unreachable, the potential will overflow, but never be used.
			// node i is reachable iff (nodes[i].done == true)
			nodes[i].pot += nodes[i].dst;
		}
	}
	void update_difference() {
		Loop(a, n) {
			for (edge_t* e : nodes[a].edges) {
				int b = e->to;
				e->dif = e->cst + nodes[a].pot - nodes[b].pot;
			}
		}
	}
	ll update_flow(ll df) {
		{
			int a = snk;
			while (a != src) {
				edge_t* e = nodes[a].e_from;
				df = min(df, e->cap);
				a = e->from;
			}
		}
		{
			int a = snk;
			ll unit_cost = 0;
			while (a != src) {
				edge_t* e = nodes[a].e_from;
				e->cap -= df;
				e->rev->cap += df;
				unit_cost += e->cst;
				a = e->from;
			}
			this->sum_flow += df;
			this->sum_cost += unit_cost * df;
		}
		return df;
	}
	ll update_flow_in_negative_cycle() {
		ll df = LLONG_MAX;
		// modify negative_cycle_endpoint as a point in the negative cycle
		{
			vector<bool> done(n);
			int a = negative_cycle_endpoint;
			while (true) {
				done[a] = true;
				edge_t *e = nodes[a].e_from;
				a = e->from;
				if (done[a]) {
					negative_cycle_endpoint = a;
					break;
				}
			}
		}
		// get miminium capacity among the edges
		{
			int a = negative_cycle_endpoint;
			while (true) {
				edge_t *e = nodes[a].e_from;
				df = min(df, e->cap);
				a = e->from;
				if (a == negative_cycle_endpoint) break;
			}
		}
		// run flow
		{
			int a = negative_cycle_endpoint;
			ll unit_cost = 0;
			while (true) {
				edge_t* e = nodes[a].e_from;
				e->cap -= df;
				e->rev->cap += df;
				unit_cost += e->cst;
				a = e->from;
				if (a == negative_cycle_endpoint) break;
			}
			// do not sum up flow
			this->sum_cost += unit_cost * df;
		}
		return df;
	}
	vector<node> nodes;
	int n, src, snk;
	ll sum_cost;
	ll sum_flow;
	bool BF_mode = false; // Bellman-Ford
	int negative_cycle_endpoint = -1;
public:
	Mincostflow(const vvi &lst, const vvll &cap, const vvll &cst, int s, int t) {
		this->n = lst.size();
		this->src = s;
		this->snk = t;
		this->nodes.resize(n);
		this->sum_cost = 0;
		this->sum_flow = 0;
		Loop(i, n) {
			this->nodes[i] = { i, false, nullptr, LLONG_MAX, 0, {} };
		}
		Loop(i, n) {
			Loop(j, lst[i].size()) {
				if (cst[i][j] >= 0) {
					add_edge(i, lst[i][j], cap[i][j], cst[i][j]);
				}
				else {
					if (i == lst[i][j]) {
						this->sum_cost += cst[i][j] * cap[i][j];
					}
					else {
						add_edge(i, lst[i][j], cap[i][j], cst[i][j]);
						this->BF_mode = true;
					}
				}
			}
		}
		if (BF_mode) {
			while (true) {
				update_potential_by_Bellman_Ford(src);
				if (negative_cycle_endpoint != -1) {
					update_flow_in_negative_cycle();
				}
				else {
					BF_mode = false;
					break;
				}
			}
		}
		else {
			update_potential_by_Dijkstra(src);
		}
	}
	// add flow at most f
	void add_flow(ll f) {
		while (true) {
			if (!nodes[snk].done) break;
            update_difference();
			ll df = update_flow(f);
			if (df == 0) break;
			f -= df;
			update_potential_by_Dijkstra(src);
		}
	}
	ll get_cost() {
		return this->sum_cost;
	}
	ll get_flow() {
		return this->sum_flow;
	}
};