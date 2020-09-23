#include "auto_util_header.hpp"

// Required: all cost >= 0, O({f}{m}\log{n})
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
	void update_potential() {
		Loop(i, n) {
			nodes[i].done = false;
			nodes[i].e_from = nullptr;
			nodes[i].dst = LLONG_MAX;
		}
		nodes[src].dst = 0;
		priority_queue<pq_t> pq;
		pq.push({ nodes[src].id, nodes[src].dst });
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
			// node i is unreachable iff (i != src && nodes[i].e_from == nullptr)
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
	vector<node> nodes;
	int n, src, snk;
	ll sum_cost;
	ll sum_flow;
public:
	Mincostflow(const vvi &lst, const vvll &cap, const vvll &cst, int s, int t) {
		this->n = lst.size();
		this->src = s;
		this->snk = t;
		this->nodes.resize(n);
		Loop(i, n) {
			Loop(j, lst[i].size()) {
				add_edge(i, lst[i][j], cap[i][j], cst[i][j]);
			}
		}
		this->sum_cost = 0;
		this->sum_flow = 0;
	}
	// add flow at most f
	void add_flow(ll f) {
		while (true) {
			update_potential();
			if (nodes[snk].e_from == nullptr) break;
			update_difference();
			ll df = update_flow(f);
			if (df == 0) break;
			f -= df;
		}
	}
	ll get_cost() {
		return this->sum_cost;
	}
	ll get_flow() {
		return this->sum_flow;
	}
};