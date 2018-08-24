template <class val_t>
class kdTree {
private:
	struct node {
		int id;
		val_t val;
		int deg;
		node *parent;
		node *child_l, *child_r;
		val_t range_l, range_r;
	};
	int dimension; // dimension
	int n; // the number of nodes
	node *root; // the root of the tree
	node *nil; // the node for leaves of the tree
	struct idval_t {
		int id;
		val_t val;
	};
	typedef typename vector<idval_t>::iterator vidval_iterator;
	inline void update_cover_range(node *focus, node* target) {
		if (target == nil) return;
		else {
			Loop(i, dimension) {
				focus->range_l[i] = min(focus->range_l[i], target->range_l[i]);
				focus->range_r[i] = max(focus->range_r[i], target->range_r[i]);
			}
			return;
		}
	}
	node* build_kdTree_rec(node *parent, pair<vidval_iterator, vidval_iterator> range, int depth) {
		if (distance(range.first, range.second) == 0) return nil;
		else {
			node *ret = new node;
			int axis = depth % dimension;
			vidval_iterator mid = range.first + distance(range.first, range.second) / 2;
			nth_element(range.first, mid, range.second, [=](const idval_t& l, const idval_t& r) { return l.val[axis] < r.val[axis]; });
			*ret = { mid->id, mid->val, depth, nil, nil, nil, mid->val, mid->val };
			ret->child_l = build_kdTree_rec(ret, { range.first, mid }, depth + 1);
			update_cover_range(ret, ret->child_l);
			ret->child_r = build_kdTree_rec(ret, { mid + 1,range.second }, depth + 1);
			update_cover_range(ret, ret->child_r);
			return ret;
		}
	}
	inline bool check_crossed_find_range(node *focus, pair<val_t, val_t> &range) {
		if (focus == nil) return false;
		else {
			bool ret = true;
			Loop(i, dimension) {
				if (range.first[i] <= focus->range_r[i] && focus->range_l[i] <= range.second[i]) continue;
				else {
					ret = false;
					break;
				}
			}
			return ret;
		}
	}
	inline bool check_in_range(node *focus, pair<val_t, val_t> &range) {
		if (focus == nil) return false;
		else {
			bool ret = true;
			Loop(i, dimension) {
				if (range.first[i] <= focus->val[i] && focus->val[i] <= range.second[i])continue;
				else {
					ret = false;
					break;
				}
			}
			return ret;
		}
	}
	void find_in_range_rec(node *focus, pair<val_t, val_t> &range, int depth, vi *in_range_list) {
		if (focus == nil) return;
		else {
			int axis = depth % dimension;
			if (check_in_range(focus, range)) in_range_list->push_back(focus->id);
			if (check_crossed_find_range(focus->child_l, range)) {
				find_in_range_rec(focus->child_l, range, depth + 1, in_range_list);
			}
			if (check_crossed_find_range(focus->child_r, range)) {
				find_in_range_rec(focus->child_r, range, depth + 1, in_range_list);
			}
		}
	}
public:
	kdTree(vector<val_t> A, int dimension) {
		n = (int)A.size();
		this->dimension = dimension;
		vector<idval_t> ary(n);
		Loop(i, n) {
			ary[i] = { i, A[i] };
		}
		nil = new node;
		root = build_kdTree_rec(nil, { ary.begin(), ary.end() }, 0);
		return;
	}
	// return id of vals in [range.first, range.second]
	vi find_in_range(pair<val_t, val_t> range) {
		vi ret;
		find_in_range_rec(root, range, 0, &ret);
		sort(ret.begin(), ret.end());
		return ret;
	}
};