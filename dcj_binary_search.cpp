// find bound of (x - pre_sum) in dist_a, which is distributed by dist_n
ll dist_bound_index(ll x, const vll &dist_a, const vll &dist_n, ll pre_acm = 0, bool upper_flag = false, bool greater_flag = false) {
	ll ans = 0;
	ll index;
	if (upper_flag) {
		if (greater_flag) index = upper_bound(dist_a.begin(), dist_a.end(), x - pre_acm, greater<ll>()) - dist_a.begin();
		else index = upper_bound(dist_a.begin(), dist_a.end(), x - pre_acm) - dist_a.begin();
	}
	else {
		if (greater_flag) index = lower_bound(dist_a.begin(), dist_a.end(), x - pre_acm, greater<ll>()) - dist_a.begin();
		else index = lower_bound(dist_a.begin(), dist_a.end(), x - pre_acm) - dist_a.begin();
	}
	if (dcj.id == 0) {
		ll buf = index;
		Loop(i, dcj.n) {
			if (i > 0) {
				Receive(i);
				buf = GetLL(i);
			}
			if (0 <= buf && buf < dist_n[i]) ans += buf;
			else ans += dist_n[i];
		}
	}
	else {
		PutLL(0, index);
		Send(0);
	}
	return ans;
}

// realize accumulate array
ll get_pre_acm(ll connect_acm) {
	ll ans = 0;
	if (dcj.id != 0) {
		Receive(dcj.id - 1);
		ans = GetLL(dcj.id - 1);
	}
	if (dcj.id != dcj.n - 1) {
		PutLL(dcj.id + 1, ans + connect_acm);
		Send(dcj.id + 1);
	}
	return ans;
}

// share common value
ll get_common_value_from(int id, ll value) {
	ll ans;
	if (dcj.id == id) {
		ans = value;
		if (dcj.id > 0) {
			PutLL(dcj.id - 1, ans);
			Send(dcj.id - 1);
		}
		if (dcj.id < dcj.n - 1) {
			PutLL(dcj.id + 1, ans);
			Send(dcj.id + 1);
		}
	}
	else if (dcj.id < id) {
		Receive(dcj.id + 1);
		ans = GetLL(dcj.id + 1);
		if (dcj.id > 0) {
			(dcj.id - 1, ans);
			Send(dcj.id - 1);
		}
	}
	else {
		Receive(dcj.id - 1);
		ans = GetLL(dcj.id - 1);
		if (dcj.id < dcj.n - 1) {
			(dcj.id + 1, ans);
			Send(dcj.id + 1);
		}
	}
	return ans;
}