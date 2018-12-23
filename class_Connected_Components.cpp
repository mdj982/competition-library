typedef ll val_t;

struct graph_t {
	int n;           // |V|, index begins with 0
	int m;           // |E|
	vector<P> edges; // E
	vector<val_t> vals; // V
	vector<ll> costs; // cost or distance
	vector<ll> caps;  // capacity
};

class Union_Find {
private:
	vi parent, rank;
public:
	Union_Find(int N) {
		parent.resize(N);
		rank.resize(N);
		Loop(i, N) {
			parent[i] = i;
			rank[i] = 0;
		}
	}
	int find(int x) {
		if (parent[x] == x) return x;
		else return parent[x] = find(parent[x]);
	}
	void unite(int x, int y) {
		x = find(x);
		y = find(y);
		if (x == y) return;
		if (rank[x] < rank[y]) parent[x] = y;
		else {
			parent[y] = x;
			if (rank[x] == rank[y]) rank[x]++;
		}
	}
	bool is_same(int x, int y) {
		return find(x) == find(y);
	}
};

class Connected_Components {
private:
	int n, m;
	vvi ccs;
	vi cc_gid;
public:
	Connected_Components(graph_t G) {
		n = G.n;
		m = G.edges.size();
		Union_Find *uf = new Union_Find(n);
		Loop(i, m) {
			uf->unite(G.edges[i].first, G.edges[i].second);
		}
		map<int, vi> cc_mp;
		Loop(i, n) cc_mp[uf->find(i)].push_back(i);
		Loopitr(itr, cc_mp) ccs.push_back(itr->second);
		return;
	}
	vvi get_ccs() {
		return ccs;
	}
	vi get_cc_gid() {
		if (cc_gid.empty()) {
			cc_gid.resize(n);
			Loop(i, ccs.size()) {
				Loop(j, ccs[i].size()) {
					cc_gid[ccs[i][j]] = i;
				}
			}
		}
		return cc_gid;
	}
};