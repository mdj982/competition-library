#include "auto_util_header.hpp"

// Ford-Fulkerson algorithm, O((n+m)f)
class Maxflow_with_LB {
private:
	struct edge_t {
		int cap;
	};
	int n, source, sink;
	int result;
	vector<bool> done;
	vector<unordered_map<int, edge_t>> lst;
	int dfs(int a, int t) {
		if (a == t) return 1;
		done[a] = true;
		Loopitr(itr, lst[a]) {
			int b = itr->fst;
			int cap = itr->snd.cap;
			if (!done[b] && cap > 0) {
				if (dfs(b, t)) {
					lst[a][b].cap--;
					lst[b][a].cap++;
					return 1;
				}
			}
		}
		return 0;
	}
	int run_flow(int s, int t, int f) {
		int ret = 0;
		Loop(i, f) {
			done = vector<bool>(n, false);
			if (dfs(s, t)) ret++;
			else break;
		}
		return ret;
	}
	bool feasible_flag;
	ll sum_flow = 0;
	ll min_flow = 0;
	ll max_flow = 0;
public:
	// make sure that bnd <= cap, solve required flow in constructor
	Maxflow_with_LB(const vvi &lst, const vvi &bnd, const vvi &cap, int s, int t) {
		this->n = lst.size() + 2;
		this->source = lst.size();
		this->sink = lst.size() + 1;
		Loop(i, lst.size()) {
			Loop(k, lst[i].size()) {
				int j = lst[i][k];
				ll b = bnd[i][k];
				ll c = cap[i][k];
				if (b > 0) {
					this->lst[source][j].cap += b;
					this->lst[i][sink].cap += b;
					sum_flow -= b;
				}
				if (c - b > 0) {
					this->lst[i][j].cap += c - b;
				}
			}
		}
		ll f00 = run_flow(source, sink, INT_MAX);
		ll f01 = run_flow(source, t, INT_MAX);
		ll f10 = run_flow(s, sink, INT_MAX);
		this->lst[t][sink].cap += INT_MAX;
		this->lst[source][s].cap += INT_MAX;
		this->lst[t][sink].cap -= f01;
		this->lst[sink][t].cap += f01;
		this->lst[source][s].cap -= f10;
		this->lst[s][source].cap += f10;
		this->feasible_flag = (this->sum_flow + f00 + f01 == 0) && (this->sum_flow + f00 + f10 == 0);
		this->sum_flow += f00 + f01 + f10;
		this->min_flow = this->sum_flow;
		this->sum_flow += run_flow(source, sink, INT_MAX);
		this->max_flow = this->sum_flow;
	}
	ll get_minflow() {
		return min_flow;
	}
	ll get_maxflow() {
		return max_flow;
	}
	bool is_feasible() {
		return this->feasible_flag;
	}
};