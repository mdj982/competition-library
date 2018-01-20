class LIS {
  vll result;
  int n;
public:
  LIS(vll a) {
    result = {};
    Loop(i, a.size()) {
      auto itr = lower_bound(result.begin(), result.end(), a[i]);
      if (itr == result.end()) {
        result.push_back(a[i]);
      }
      else *itr = a[i];
    }
    n = result.size();
    result = {};
    Loop(i, a.size()) {
      auto itr = lower_bound(result.begin(), result.end(), a[i]);
      if (itr == result.end()) {
        result.push_back(a[i]);
        if (result.size() == n) break;
      }
      else *itr = a[i];
    }
  }
  vll get_lis() {
    return result;
  }
  int get_lisn() {
    return n;
  }
};