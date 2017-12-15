class Union_Find {
private:
	vi parent, rank;
	int find(int x) {
		if (parent[x] == x) return x;
		else return parent[x] = find(parent[x]);
	}
public:
	Union_Find(int N) {
		parent.resize(N);
		rank.resize(N);
		Loop(i, N) {
			parent[i] = i;
			rank[i] = 0;
		}
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

// Union_Find sample
int main() {
	int n; cin >> n;
	Union_Find uf(n);
	int m; cin >> m;
	Loop(i, m) {
		int a, b; cin >> a >> b;
		uf.unite(a, b);
	}
	int q; cin >> q;
	Loop(i, m) {
		int a, b; cin >> a >> b;
		uf.is_same(a, b);
	}
	return 0;
}