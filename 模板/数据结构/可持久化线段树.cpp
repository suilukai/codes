#include <cstdio>

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

const int maxn = 1e6 + 5, maxm = 1e6 + 5;

struct ST {
	int lc, rc, val;
} t[25 * maxn];

int tot, root[maxm];

int build(int l, int r) {
	int p = ++tot;
	if (l == r) {
		t[p].val = get_num();
		return p;
	}
	int mid = (l + r) / 2;
	t[p].lc = build(l, mid);
	t[p].rc = build(mid + 1, r);
	return p;
}

int modify(int now, int l, int r, int x, int val) {
	int p = ++tot;
	t[p] = t[now];
	if (l == r) {
		t[p].val = val;
		return p;
	}
	int mid = (l + r) / 2;
	if (x <= mid) t[p].lc = modify(t[p].lc, l, mid, x, val);
	else t[p].rc = modify(t[p].rc, mid + 1, r, x, val);
	return p;
}

int query(int p, int l, int r, int x) {
	if (l == r) return t[p].val;
	int mid = (l + r) / 2;
	if (x <= mid) return query(t[p].lc, l, mid, x);
	else return query(t[p].rc, mid + 1, r, x);
}

int main() {
	int n = get_num(), m = get_num();
	root[0] = build(1, n);
	for (int i = 1; i <= m; ++i) {
		int v = get_num(), op = get_num(), x = get_num();
		if (op == 1) {
			int val = get_num();
			root[i] = modify(root[v], 1, n, x, val);
		} else {
			printf("%d\n", query(root[v], 1, n, x));
			root[i] = root[v];
		}
	}
	return 0;
}
