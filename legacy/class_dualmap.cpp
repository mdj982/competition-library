template <class T1, class T2>
class dualmap {
private:
	map<T1, T2> mp1;
	map<T2, T1> mp2;
public:
	dualmap(vector<pair<T1, T2>> init) {
		Foreach(x, init) {
			insert(x);
		}
	}
	void insert(pair<T1, T2> x) {
		mp1[x.first] = x.second;
		mp2[x.second] = x.first;
	}
	T2 find(T1 val) {
		return mp1[val];
	}
	T1 rfind(T2 val) {
		return mp2[val];
	}
};