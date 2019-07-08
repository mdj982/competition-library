class Connected_Components {
private:
	int n;
	vi cc_gid;
	vvi ccs;
	void dfs(int a, int gid, const vvi &lst) {
		cc_gid[a] = gid;
		ccs.back().push_back(a);
		Foreach(b, lst[a]) {
			if (cc_gid[b] == -1) dfs(b, gid, lst);
		}
	}
public:
	Connected_Components(const vvi &lst) {
		n = lst.size();
		cc_gid = vi(n, -1);
		int gid = 0;
		Loop(i, n) {
			if (cc_gid[i] == -1) {
				ccs.push_back({});
				dfs(i, gid, lst);
				gid++;
			}
		}
	}
	vi get_cc_gid() {
		return cc_gid;
	}
	vvi get_ccs() {
		return ccs;
	}
};