typedef int seg_t;

class SegTree {
private:
  int n;
  vector<seg_t> segs;
  int left_of(int index) {
    return index * 2 + 1;
  }
  int right_of(int index) {
    return index * 2 + 2;
  }
  void local_update(int index, seg_t(*update_rule)(seg_t, seg_t)) {
    int l = left_of(index), r = index * 2 + 2;
    segs[index] = update_rule(segs[l], segs[r]);
  }
  void get_index_of_range_sub(int s, int t, int l, int r, int index, vi *v) {
    if (s == l && t == r) v->push_back(index);
    else {
      int mid = (l + r) / 2;
      if (s < mid && mid < t) {
        get_index_of_range_sub(s, mid, l, mid, left_of(index), v);
        get_index_of_range_sub(mid, t, mid, r, right_of(index), v);
      }
      else if (s < mid) {
        get_index_of_range_sub(s, t, l, mid, left_of(index), v);
      }
      else if (mid < t) {
        get_index_of_range_sub(s, t, mid, r, right_of(index), v);
      }
    }
  }
public:
  SegTree(vector<seg_t> a, seg_t init, seg_t(*update_rule)(seg_t, seg_t)) {
    n = (int)pow(2, ceil(log2(a.size())));
    segs = vector<seg_t>(n * 2 - 1, init);
    Loop(i, a.size()) segs[n - 1 + i] = a[i];
    Loopr(i, n - 1) local_update(i, update_rule);
  }
  void update(int k, seg_t x, seg_t(*update_rule)(seg_t, seg_t), bool add_flag = false) {
    int index = k + n - 1;
    segs[index] = add_flag ? segs[index] + x : x;
    while (index > 0) {
      index = (index - 1) / 2;
      local_update(index, update_rule);
    }
  }
  // note: range [s, t)
  vi get_index_of_range(int s, int t) {
    vi ret = {};
    get_index_of_range_sub(s, t, 0, n, 0, &ret);
    return ret;
  }
  // write extend method from here:
  seg_t sum_of_range(int s, int t) {
    vi v = get_index_of_range(s, t);
    seg_t ret = 0;
    Loop(i, v.size()) {
      ret += segs[v[i]];
    }
    return ret;
  }
};

seg_t update_rule(seg_t a, seg_t b) {
  return a + b;
}

int main() {
  int n; cin >> n;
  int q; cin >> q;
  vector<seg_t> a(n, 0);
  SegTree segtree(a, 0, update_rule);
  Loop(i, q) {
    int c, x, y; cin >> c >> x >> y;
    if (c == 0) {
      segtree.update(x - 1, y, update_rule, true);
    }
    else {
      cout << segtree.sum_of_range(x - 1, y) << endl;
    }
  }
  return 0;
}