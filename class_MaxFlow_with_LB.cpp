#include "auto_util_header.hpp"

class MaxFlow_with_LB {
private:
	struct edge_t {
		int from; int to; ll cap; edge_t* rev;
	};
	struct node_t {
		int skip = false;
		list<edge_t*> edges;
	};
	struct graph_t {
		int n, m;
		int s, t;
		vector<node_t> nodes;
	};
	void add_edge(int a, int b, ll c) {
		edge_t *e_pre;
		edge_t *e_rev;
		if (pre_edges[{a, b}] == nullptr) {
			e_pre = new edge_t({ a, b, c, nullptr });
			e_rev = new edge_t({ b, a, 0, nullptr });
			e_pre->rev = e_rev;
			e_rev->rev = e_pre;
			pre_edges[{a, b}] = e_pre;
			this->G.nodes[a].edges.push_back(e_pre);
			this->G.nodes[b].edges.push_back(e_rev);
		}
		else {
			pre_edges[{a, b}]->cap += c;
		}
	}
	void make_level_graph(int s, int t) {
		LG.n = G.n;
		LG.m = 0;
		LG.s = s;
		LG.t = t;
		LG.nodes = vector<node_t>(G.n);
		vi levels(G.n, INT_MAX);
		queue<int> que;
		levels[s] = 0;
		que.push(s);
		while (que.size()) {
			int a = que.front(); que.pop();
			for (edge_t* e : G.nodes[a].edges) {
				if (e->cap > 0) {
					if (levels[e->to] == INT_MAX) {
						levels[e->to] = levels[a] + 1;
						que.push(e->to);
					}
					if (levels[e->to] == levels[a] + 1) {
						LG.nodes[a].edges.push_back(e);
						LG.m += 1;
					}
				}
			}
		}
		if (levels[t] == INT_MAX) LG.t = -1;
	}
	ll add_flow_on_LG(int a, ll f) {
		if (a == LG.t) return f;
		ll ret = 0;
		for (auto itr = LG.nodes[a].edges.begin(); itr != LG.nodes[a].edges.end();) {
			if (f == 0) break;
			edge_t* e = *itr;
			if (LG.nodes[e->to].skip) {
				itr = LG.nodes[a].edges.erase(itr);
			}
			else {
				ll df = add_flow_on_LG(e->to, min(e->cap, f));
				if (df > 0) {
					e->cap -= df;
					e->rev->cap += df;
					ret += df;
					f -= df;
					if (e->cap == 0) itr = LG.nodes[a].edges.erase(itr);
					else itr++;
				}
				else {
					LG.nodes[e->to].skip = true;
					itr = LG.nodes[a].edges.erase(itr);
				}
			}
		}
		return ret;
	}
	// return achieved flow
	ll add_flow_sub(int s, int t, ll f) {
		ll ret = 0;
		Loop(_, G.n) {
			if (f == 0) break;
			make_level_graph(s, t);
			if (LG.t == -1) break;
			ll df = add_flow_on_LG(LG.s, f);
			if (df == 0) break;
			ret += df;
			f -= df;
		}
		return ret;
	}
	map<P, edge_t*> pre_edges;
	graph_t G, LG;
	bool feasible_flag;
	ll sum_flow = 0;
public:
	// make sure that bnd <= cap, solve required flow in constructor
	MaxFlow_with_LB(const vvi &lst, const vvll &bnd, const vvll &cap, int s, int t) {
		this->G.n = lst.size() + 2;
		this->G.s = lst.size();
		this->G.t = lst.size() + 1;
		this->G.nodes = vector<node_t>(G.n);
		Loop(i, lst.size()) {
			Loop(k, lst[i].size()) {
				int j = lst[i][k];
				ll b = bnd[i][k];
				ll c = cap[i][k];
				if (b > 0) {
					add_edge(G.s, j, b);
					add_edge(i, G.t, b);
					sum_flow -= b;
				}
				if (c - b > 0) {
					add_edge(i, j, c - b);
				}
			}
		}
		ll f00 = add_flow_sub(G.s, G.t, LLONG_MAX);
		ll f01 = add_flow_sub(G.s, t, LLONG_MAX);
		ll f10 = add_flow_sub(s, G.t, LLONG_MAX);
		add_edge(t, G.t, LLONG_MAX);
		add_edge(G.s, s, LLONG_MAX);
		pre_edges[{t, G.t}]->cap -= f01;
		pre_edges[{t, G.t}]->rev->cap += f01;
		pre_edges[{G.s, s}]->cap -= f10;
		pre_edges[{G.s, s}]->rev->cap += f10;
		this->feasible_flag = (this->sum_flow + f00 + f01 == 0) && (this->sum_flow + f00 + f10 == 0);
		this->sum_flow += f00 + f01 + f10;
	}
	~MaxFlow_with_LB() {
		for (const auto &x : this->pre_edges) {
			edge_t *e = x.snd;
			delete e->rev;
			delete e;
		}
	}
	// return achieved flow, Dinic's algorithm, O(min(n^2 m, fm))
	void add_flow(ll f = LLONG_MAX) {
		this->sum_flow += add_flow_sub(this->G.s, this->G.t, f);
	}
	ll get_flow() {
		return sum_flow;
	}
	bool is_feasible() {
		return this->feasible_flag;
	}
};