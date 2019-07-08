namespace dll_op {

	ll L = (ll)1e18;

	class dll {
	private:
		Pll val;
		inline Pll modify(Pll x) {
			Pll ret = { 0, 0 };
			ret.fst = x.fst + (x.snd / L);
			ret.snd = x.snd % L;
			int sign = (ret.fst < 0 ? 0b10 : 0b00) + (ret.snd < 0 ? 0b01 : 0b00);
			if (sign == 0b10 && ret.snd != 0) { ret.fst += 1; ret.snd -= L; }
			else
				if (sign == 0b01 && ret.fst != 0) { ret.fst -= 1; ret.snd += L; }
			return ret;
		}
	public:
		dll(ll init = 0) { val = modify({ 0, init }); return; }
		dll(Pll init = { 0, 0 }) { val = modify(init); return; }
		dll(const dll& another) { val = another.val; return; }
		inline dll& operator=(const dll &another) { val = another.val; return *this; }
		inline dll operator+(const dll &x) { return modify({ val.fst + x.val.fst, val.snd + x.val.snd }); }
		inline dll operator-(const dll &x) { return modify({ val.fst - x.val.fst, val.snd - x.val.snd }); }
		inline dll& operator+=(const dll &x) { val = modify({ val.fst + x.val.fst, val.snd + x.val.snd }); return *this; }
		inline dll& operator-=(const dll &x) { val = modify({ val.fst - x.val.fst, val.snd - x.val.snd }); return *this; }
		friend inline ostream& operator << (ostream &os, const dll& x) {
			int sign = (x.val.fst < 0 ? 0b10 : 0b00) + (x.val.snd < 0 ? 0b01 : 0b00);
			if (sign == 0b00 || sign == 0b01) {
				if (x.val.fst != 0) os << x.val.fst << setfill('0') << setw(18);
				os << x.val.snd;
			}
			else if (sign == 0b11 || sign == 0b10) {
				if (x.val.fst != 0) os << x.val.fst << setfill('0') << setw(18);
				os << abs(x.val.snd);
			}
			return os;
		}
	};
}

using namespace dll_op;
typedef vector<dll> vdll;
typedef vector<vector<dll>> vvdll;