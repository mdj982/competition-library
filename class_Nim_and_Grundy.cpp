class Nim {
private:
  bool result;
public:
  Nim(vll a) {
    ll x = 0;
    Loop(i, a.size()) x ^= a[i];
    if (x != 0) result = true;
    else result = false;
  }
  bool get_result() {
    return result;
  }
  string get_winner(string player1, string player2) {
    if (result) return player1;
    else return player2;
  }
};

class Grundy {
private:
  bool result;
public:
  Grundy(vi a, vi b) {
    sort(b.begin(), b.end());
    int max_a = *max_element(a.begin(), a.end());
    vi grundies((unsigned)max_a + 1, 0);
    Loop1(i, max_a) {
      set<int> s;
      Loop(j, b.size()) {
        int buf = i - b[j];
        if (buf >= 0) s.insert(grundies[buf]);
      }
      Loop(j, i + 1) {
        if (s.find(j) == s.end()) {
          grundies[i] = j;
          break;
        }
      }
    }
    vll x(a.size());
    Loop(i, a.size()) x[i] = grundies[a[i]];
    Nim *nim = new Nim(x);
    result = nim->get_result();
  }
  bool get_result() {
    return result;
  }
  string get_winner(string player1, string player2) {
    if (result) return player1;
    else return player2;
  }
};