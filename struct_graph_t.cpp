struct graph_t {
	int n;           // |V|, index begins with 0
	int m;           // |E|
	vector<P> edges; // E
	vector<ll> cost; // cost or distance
	vector<ll> cap;  // capacity
};

// graph input sample
int main() {
	graph_t G;
	cin >> G.n >> G.m;
	Loop(i, G.m) {
		int s, t, c; cin >> s >> t >> c;
		G.edges.push_back({ s, t });
		G.cost.push_back(c);
	}
	return 0;
}