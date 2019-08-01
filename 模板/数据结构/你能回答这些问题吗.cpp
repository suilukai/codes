#include <cstdio>
#include <iostream>
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

const int maxn = 5e5 + 5;

struct ST {
	int l, r, sum, lmax, rmax, maxs;
	ST operator + (const ST& rhs) const {
		ST ans;
		ans.l = l, ans.r = rhs.r;
		ans.sum = sum + rhs.sum;
		ans.lmax = max(lmax, sum + rhs.lmax);
		ans.rmax = max(rhs.rmax, rhs.sum + rmax);
		ans.maxs = max(max(maxs, rhs.maxs), rmax + rhs.lmax);
		return ans;
	}
} t[4 * maxn];

inline void up(int p) {
	t[p] = t[2 * p] + t[2 * p + 1];
}

void build(int p, int l, int r) {
	t[p].l = l, t[p].r = r;
	if (l == r) {
		t[p].sum = t[p].lmax = t[p].rmax = t[p].maxs = get_num();
		return;
	}
	int mid = (l + r) / 2;
	build(2 * p, l, mid);
	build(2 * p + 1, mid + 1, r);
	up(p);
}

void modify(int p, int x, int y) {
	if (t[p].l == t[p].r) {
		t[p].sum = t[p].lmax = t[p].rmax = t[p].maxs = y;
		return;
	}
	int mid = (t[p].l + t[p].r) / 2;
	if (x <= mid) modify(2 * p, x, y);
	else modify(2 * p + 1, x, y);
	up(p);
}

ST query(int p, int l, int r) {
	if (l <= t[p].l && t[p].r <= r) return t[p];
	int mid = (t[p].l + t[p].r) / 2;
	if (l <= mid && r <= mid) return query(2 * p, l, r);
	else if (l > mid && r > mid) return query(2 * p + 1, l, r);
	else return query(2 * p, l, r) + query(2 * p + 1, l, r);
}

int main() {
	int n = get_num(), m = get_num();
	build(1, 1, n);
	for (int i = 1; i <= m; ++i) {
		int k = get_num(), x = get_num(), y = get_num();
		if (k == 1) {
			if (x > y) swap(x, y);
			printf("%d\n", query(1, x, y).maxs);
		} else modify(1, x, y);
	}
	return 0;
}
