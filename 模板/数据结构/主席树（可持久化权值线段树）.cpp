#include <cstdio>
#include <algorithm>

using namespace std;

inline int get_num() {
	int num = 0, flag = 1;
	char c = getchar();
	while (c < '0' || c > '9') {
		if (c == '-') flag = -1;
		c = getchar();
	}
	while (c >= '0' && c <= '9')
		num = num * 10 + c - '0', c = getchar();
	return flag * num;
}

const int maxn = 2e5 + 5, maxm = 2e5 + 5;

int a[maxn], b[maxn];

struct ST {
	int lc, rc, cnt;
} t[20 * maxn];

int tot, root[maxm];

inline int update(int p) {
	t[p].cnt = t[t[p].lc].cnt + t[t[p].rc].cnt;
	return p;
}

int build(int l, int r) {
	int p = ++tot;
	if (l == r) {
		t[p].cnt = 0;
		return p;
	}
	int mid = (l + r) / 2;
	t[p].lc = build(l, mid);
	t[p].rc = build(mid + 1, r);
	return update(p);
}

int add(int now, int l, int r, int x, int d) {
	int p = ++tot;
	t[p] = t[now];
	if (l == r) {
		++t[p].cnt;
		return p;
	}
	int mid = (l + r) / 2;
	if (x <= mid) t[p].lc = add(t[p].lc, l, mid, x, d);
	else t[p].rc = add(t[p].rc, mid + 1, r, x, d);
	return update(p);
}

int query(int p, int q, int l, int r, int k) {
	if (l == r) return b[l];
	int mid = (l + r) / 2, lcnt = t[t[q].lc].cnt - t[t[p].lc].cnt;
	if (k <= lcnt) return query(t[p].lc, t[q].lc, l, mid, k);
	else return query(t[p].rc, t[q].rc, mid + 1, r, k - lcnt);
}

int main() {
	int n = get_num(), m = get_num();
	for (int i = 1; i <= n; ++i) a[i] = b[i] = get_num();
	sort(b + 1, b + n + 1);
	b[0] = unique(b + 1, b + n + 1) - b - 1;
	root[0] = build(1, b[0]);
	for (int i = 1; i <= n; ++i) {
		a[i] = lower_bound(b + 1, b + b[0] + 1, a[i]) - b;
		root[i] = add(root[i - 1], 1, b[0], a[i], 1);
	}
	for (int i = 1; i <= m; ++i) {
		int l = get_num(), r = get_num(), k = get_num();
		printf("%d\n", query(root[l - 1], root[r], 1, b[0], k));
	}
	return 0;
}
