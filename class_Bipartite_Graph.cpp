class Bipartite_Graph {
private:
	int n;
	vvi sets; // sets[2k] -- sets[2k + 1]
	vi sids;
	bool non_bipartite_flag = false;
public:
	Bipartite_Graph(const vvi &lst) {
		int n = lst.size();
		sids = vi(n, -1);
		Loop(i, n) {
			if (sids[i] == -1) {
				int base = sets.size();
				sets.push_back({});
				sets.push_back({});
				queue<int> que;
				que.push(i);
				sids[i] = base;
				sets[sids[i]].push_back(i);
				while (que.size()) {
					int a = que.front(); que.pop();
					Foreach(b, lst[a]) {
						if (sids[b] == sids[a]) {
							non_bipartite_flag = true;
							return;
						}
						if (sids[b] == -1) {
							sids[b] = sids[a] ^ 1;
							sets[sids[b]].push_back(b);
							que.push(b);
						}
					}
				}
			}
		}
	}
	bool is_bipartite() {
		return !non_bipartite_flag;
	}
	vvi get_sets() {
		return sets;
	}
	vi get_sids() {
		return sids;
	}
};