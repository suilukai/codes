#include <cstdio>

typedef long long ll;

inline ll get_num() {
	ll num = 0;
	char c = getchar();
	while (c < '0' || c > '9') c = getchar();
	while (c >= '0' && c <= '9')
		num = num * 10 + c - '0', c = getchar();
	return num;
}

const int maxn = 1e5 + 5;

struct ST {
	int l, r;
	ll sum, mul, add;
} t[4 * maxn];

int n, m;
ll mod;

inline void up(int p) {
	t[p].sum = (t[2 * p].sum + t[2 * p + 1].sum) % mod;
}

inline void mark(int p, ll mul, ll add) {
	t[p].sum = t[p].sum * mul % mod;
	t[p].sum = (t[p].sum + add * (t[p].r - t[p].l + 1) % mod) % mod;
	t[p].mul = t[p].mul * mul % mod;
	t[p].add = (t[p].add * mul % mod + add) % mod;
}

inline void down(int p) {
	if (t[p].mul != 1 || t[p].add) {
		mark(2 * p, t[p].mul, t[p].add);
		mark(2 * p + 1, t[p].mul, t[p].add);
		t[p].mul = 1, t[p].add = 0;
	}
}

void build(int p, int l, int r) {
	t[p].l = l, t[p].r = r;
	t[p].mul = 1, t[p].add = 0;
	if (l == r) {
		t[p].sum = get_num();
		return;
	}
	int mid = (l + r) / 2;
	build(2 * p, l, mid);
	build(2 * p + 1, mid + 1, r);
	up(p);
}

void modify1(int p, int l, int r, ll mul) {
	if (l <= t[p].l && t[p].r <= r) {
		mark(p, mul, 0);
		return;
	}
	down(p);
	int mid = (t[p].l + t[p].r) / 2;
	if (l <= mid) modify1(2 * p, l, r, mul);
	if (r > mid) modify1(2 * p + 1, l, r, mul);
	up(p);
}

void modify2(int p, int l, int r, ll add) {
	if (l <= t[p].l && t[p].r <= r) {
		mark(p, 1, add);
		return;
	}
	down(p);
	int mid = (t[p].l + t[p].r) / 2;
	if (l <= mid) modify2(2 * p, l, r, add);
	if (r > mid) modify2(2 * p + 1, l, r, add);
	up(p);
}

ll query(int p, int l, int r) {
	if (l <= t[p].l && t[p].r <= r) return t[p].sum;
	down(p);
	int mid = (t[p].l + t[p].r) / 2;
	ll sum = 0;
	if (l <= mid) sum = (sum + query(2 * p, l, r)) % mod;
	if (r > mid) sum = (sum + query(2 * p + 1, l, r)) % mod;
	return sum;
}

int main() {
	n = get_num(), m = get_num(), mod = get_num();
	build(1, 1, n);
	for (int i = 1; i <= m; ++i) {
		int op = get_num();
		if (op == 1) {
			int x = get_num(), y = get_num();
			ll k = get_num() % mod;
			modify1(1, x, y, k);
		}
		else if (op == 2) {
			int x = get_num(), y = get_num();
			ll k = get_num() % mod;
			modify2(1, x, y, k);
		}
		else {
			int x = get_num(), y = get_num();
			printf("%lld\n", query(1, x, y));
		}
	}
	return 0;
}
