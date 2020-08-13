#include "auto_util_header.hpp"

class Finding_Arts {
private:
	struct node {
		int id; bool done; vi to; int from; int pre; int low;
	};
	vector<node> nodes;
	int n;
	int ord;
	vi arts;
	void lowlink_dfs(int a, bool isroot) {
		nodes[a].done = true;
		nodes[a].pre = nodes[a].low = ord;
		ord++;
		int cnt = 0;
		Loop(i, nodes[a].to.size()) {
			int b = nodes[a].to[i];
			if (b == nodes[a].from) continue;
			if (!nodes[b].done) {
				nodes[b].from = a;
				lowlink_dfs(b, false);
				nodes[a].low = min(nodes[a].low, nodes[b].low);
				if (nodes[a].pre <= nodes[b].low) cnt++;
			}
			else {
				nodes[a].low = min(nodes[a].low, nodes[b].pre);
			}
		}
		if (cnt > (isroot ? 1 : 0)) arts.push_back(a);
		return;
	}
public:
	Finding_Arts(const vvi &lst) {
		n = lst.size();
		nodes.resize(n);
		Loop(i, n) nodes[i] = { i, false, {}, -1, -1, -1 };
		Loop(i, n) {
			Foreach(j, lst[i]) {
				nodes[i].to.push_back(j);
			}
		}
		ord = 0;
		Loop(i, nodes.size()) {
			if (!nodes[i].done) lowlink_dfs(i, true);
		}
		sort(arts.begin(), arts.end());
	}
	vi get_arts() {
		return arts;
	}
};