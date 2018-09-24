// a.size() should be 2^m
vector<cdouble> FFT(vector<cdouble> a) {
	int n = int(a.size());
	vector<cdouble> f(n);
	if (n == 1) f = a;
	else {
		vector<cdouble> a0(n / 2), a1(n / 2);
		Loop(i, n / 2) {
			a0[i] = a[i * 2];
			a1[i] = a[i * 2 + 1];
		}
		vector<cdouble> q = FFT(a0);
		vector<cdouble> r = FFT(a1);
		Loop(i, n / 2) {
			cdouble x = exp(cdouble({ 0, 2 * PI * i / n }));
			f[i] = q[i] + x * r[i];
			f[i + n / 2] = q[i] - x * r[i];
		}
	}
	return f;
}

// f.size() should be 2^m
vector<cdouble> IFFT(vector<cdouble> f) {
	int n = int(f.size());
	vector<cdouble> a(n);
	if (n == 1) a = f;
	else {
		vector<cdouble> q(n / 2), r(n / 2);
		Loop(i, n / 2) {
			cdouble x = exp(cdouble({ 0, 2 * PI * i / n }));
			q[i] = (f[i] + f[i + n / 2]) / 2.0;
			r[i] = (f[i] - f[i + n / 2]) / (x * 2.0);
		}
		vector<cdouble> a0 = IFFT(q);
		vector<cdouble> a1 = IFFT(r);
		Loop(i, n / 2) {
			a[i * 2] = a0[i];
			a[i * 2 + 1] = a1[i];
		}
	}
	return a;
}