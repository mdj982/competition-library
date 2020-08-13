#include "auto_util_header.hpp"

class Union_Find {
private:
	vi p, r, c; // parent, rank, the number of connected components
public:
	Union_Find(int N) {
		p.resize(N);
		r.resize(N);
		c.resize(N);
		Loop(i, N) {
			p[i] = i;
			r[i] = 0;
			c[i] = 1;
		}
	}
	int find(int x) {
		if (p[x] == x) return x;
		else return p[x] = find(p[x]);
	}
	void unite(int x, int y) {
		x = find(x);
		y = find(y);
		if (x == y) return;
		if (r[x] == r[y]) r[x]++;
		if (r[x] < r[y]) {
			p[x] = y;
			c[y] += c[x];
		}
		else {
			p[y] = x;
			c[x] += c[y];
		}
	}
	bool is_same(int x, int y) {
		return find(x) == find(y);
	}
	int get_cnt(int x) {
		return c[find(x)];
	}
};

class Kruskal {
private:
	struct edge_t {
		int s, t;
        ll d;
        bool operator<(const edge_t & another) const {
			return d > another.d;
		}
	};
	int n;
    vector<edge_t> result;
public:
    Kruskal(const vvi &lst, const vvll &cst) {
        this->n = lst.size();
        priority_queue<edge_t> pq;
        Loop(i, lst.size()) {
            Loop(j, lst[i].size()) {
                int s = i;
                int t = lst[i][j];
                ll d = cst[i][j];
                pq.push({s, t, d});
            }
        }
        Union_Find uf(n);
        while (pq.size()) {
            edge_t edge = pq.top(); pq.pop();
            if (uf.is_same(edge.s, edge.t)) continue;
            else {
                result.push_back(edge);
                uf.unite(edge.s, edge.t);
            }
        }
	}
	vector<P> get_result() {
        vector<P> ret;
		Foreach(edge, result) {
            ret.push_back({edge.s, edge.t});
        }
        return ret;
	}
	ll get_weight() {
		ll ret = 0;
		Foreach(edge, result) {
			ret += edge.d;
		}
		return ret;
	}
};