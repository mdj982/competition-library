class LIS {
  vll result;
  vi id_result;
  int n;
public:
  LIS(vll a, bool strict_flag) {
    int n = a.size();
    vll record;
    vi id_record, parents(n, -1);
    Loop(i, n) {
      auto itr = strict_flag ? lower_bound(record.begin(), record.end(), a[i])
        : upper_bound(record.begin(), record.end(), a[i]);
      if (itr == record.end()) {
        record.push_back(a[i]);
        id_record.push_back(i);
        itr = record.end();
        itr--;
      }
      else {
        *itr = a[i];
        id_record[distance(record.begin(), itr)] = i;
      }
      if (itr != record.begin()) {
        parents[i] = id_record[distance(record.begin(), itr) - 1];
      }
    }
    result = {};
    id_result = {};
    int focus = id_record.back();
    do {
      id_result.push_back(focus);
      result.push_back(a[focus]);
      focus = parents[focus];
    } while (focus != -1);
    reverse(result.begin(), result.end());
    reverse(id_result.begin(), id_result.end());
  }
  vll get_lis() {
    return result;
  }
  vi get_lisid() {
    return id_result;
  }
  int get_lisn() {
    return result.size();
  }
};

