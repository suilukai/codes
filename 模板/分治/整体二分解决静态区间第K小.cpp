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

struct Question {
	int op, x, y, z;
} q[maxn + maxm], lq[maxn + maxm], rq[maxn + maxm];

int n, m, a[maxn], b[maxn], bit[maxn], ans[maxm];

inline int ask(int x) {
	int ret = 0;
	while (x) ret += bit[x], x -= x & -x;
	return ret;
}

inline void add(int x, int d) {
	while (x <= n) bit[x] += d, x += x & -x;
}

void solve(int lv, int rv, int l, int r) {
	if (l > r) return;
	if (lv == rv) {
		for (int i = l; i <= r; ++i)
			if (q[i].op) ans[q[i].op] = b[lv];
		return;
	}
	int mid = (lv + rv) / 2, lt = 0, rt = 0;
	for (int i = l; i <= r; ++i) {
		if (!q[i].op) {
			if (q[i].y <= mid) add(q[i].x, 1), lq[++lt] = q[i];
			else rq[++rt] = q[i];
		} else {
			int cnt = ask(q[i].y) - ask(q[i].x - 1);
			if (q[i].z <= cnt) lq[++lt] = q[i];
			else q[i].z -= cnt, rq[++rt] = q[i];
		}
	}
	for (int i = r; i >= l; --i)
		if (!q[i].op && q[i].y <= mid) add(q[i].x, -1);
	for (int i = 1; i <= lt; ++i) q[l + i - 1] = lq[i];
	for (int i = 1; i <= rt; ++i) q[l + lt + i - 1] = rq[i];
	solve(lv, mid, l, l + lt - 1);
	solve(mid + 1, rv, l + lt, r);
}

int main() {
	n = get_num(), m = get_num();
	for (int i = 1; i <= n; ++i) a[i] = b[i] = get_num();
	sort(b + 1, b + n + 1);
	b[0] = unique(b + 1, b + n + 1) - b - 1;
	for (int i = 1; i <= n; ++i) {
		a[i] = lower_bound(b + 1, b + b[0] + 1, a[i]) - b;
		q[i].op = 0, q[i].x = i, q[i].y = a[i];
	}
	for (int i = 1; i <= m; ++i) {
		int l = get_num(), r = get_num(), k = get_num();
		q[n + i].op = i, q[n + i].x = l, q[n + i].y = r, q[n + i].z = k;
	}
	solve(1, b[0], 1, n + m);
	for (int i = 1; i <= m; ++i) printf("%d\n", ans[i]);
	return 0;
}
