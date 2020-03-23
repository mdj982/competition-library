class Random_Int {
private:
	mt19937 *mt;
	uniform_int_distribution<> *distr_int;
public:
	// uniform int distribution of [0, m)
	Random_Int(int m) {
		mt = new std::mt19937((unsigned)(
			std::chrono::duration_cast<chrono::nanoseconds>(
				chrono::high_resolution_clock::now().time_since_epoch()
				).count()
			));
		distr_int = new uniform_int_distribution<>(0, m - 1);
	}
	int get() {
		return (*distr_int)(*mt);
	}
};

// based on rolling hash, constructor should be called as little as possible
class String_Comparator {
private:
	const vll MOD_cands = {
		1000000007, 1000000403, 1000000787, 1000001447, 1000001819,
		1000002359, 1000002803, 1000003919, 1000004207, 1000004519
	};
	ll MOD;
	int max_length;
	vll bases;
	vvll pows;
	Random_Int *ri;
public:
	String_Comparator(int max_length, int n_base = 4) {
		Random_Int *rm = new Random_Int(int(MOD_cands.size()));
		this->MOD = this->MOD_cands[rm->get()];
		this->max_length = max(1, max_length);
		this->ri = new Random_Int(int(this->MOD) - 256);
		this->bases.resize(n_base);
		this->pows.resize(n_base, vll(this->max_length + 1));
		Loop(k, n_base) {
			this->bases[k] = this->ri->get() + 256;
			this->pows[k][0] = 1;
			Loop1(i, this->max_length) {
				pows[k][i] = pows[k][i - 1] * this->bases[k] % this->MOD;
			}
		}
	}
	vvll make_hashs(const string &s) {
		int n = int(s.length());
		vvll ret(this->bases.size(), vll(n + 1));
		Loop(k, this->bases.size()) {
			Loop1(i, n) {
				ret[k][i] = (ret[k][i - 1] * this->bases[k] + s[i - 1]) % this->MOD;
			}
		}
		return ret;
	}
	// Required: hashs0, hashs1 and this function should belong to the same class
	// Required: s0 + l <= str0.length() and s1 + l <= str1.length()
	bool is_equal(const vvll &hashs0, const vvll &hashs1, const int s0, const int s1, const int l) {
		Loop(k, this->bases.size()) {
			ll val0 = (hashs0[k][s0 + l] - hashs0[k][s0] * pows[k][l]) % this->MOD;
			ll val1 = (hashs1[k][s1 + l] - hashs1[k][s1] * pows[k][l]) % this->MOD;
			if (val0 < 0) val0 += this->MOD;
			if (val1 < 0) val1 += this->MOD;
			if (val0 != val1) return false;
		}
		return true;
	}
	bool is_equal(const vll &hash0, const vll &hash1) {
		Loop(k, this->bases.size()) {
			if (hash0[k] != hash1[k]) return false;
		}
		return true;
	}
	// Required: s + l <= str.length()
	vll get_hash(const vvll &hashs, const int s, const int l) {
		vll ret(bases.size());
		Loop(k, this->bases.size()) {
			ret[k] = (hashs[k][s + l] - hashs[k][s] * pows[k][l]) % this->MOD;
			if (ret[k] < 0) ret[k] += this->MOD;
		}
		return ret;
	}
};