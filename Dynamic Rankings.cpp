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

const int maxn = 1e5 + 5, maxm = 1e5 + 5, inf = 1e9;

struct Question {
	int op, x, y, z;
} q[maxn + 2 * maxm], lq[maxn + 2 * maxm], rq[maxn + 2 * maxm];

int n, m, tot, qt, a[maxn], bit[maxn], ans[maxm];

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
			if (q[i].op > 0) ans[q[i].op] = lv;
		return;
	}
	int mid = (lv + rv) >> 1, lt = 0, rt = 0;
	for (int i = l; i <= r; ++i) {
		if (q[i].op <= 0) {
			if (q[i].y <= mid) add(q[i].x, q[i].z), lq[++lt] = q[i];
			else rq[++rt] = q[i];
		} else {
			int cnt = ask(q[i].y) - ask(q[i].x - 1);
			if (q[i].z <= cnt) lq[++lt] = q[i];
			else q[i].z -= cnt, rq[++rt] = q[i];
		}
	}
	for (int i = r; i >= l; --i)
		if (q[i].op <= 0 && q[i].y <= mid) add(q[i].x, -q[i].z);
	for (int i = 1; i <= lt; ++i) q[l + i - 1] = lq[i];
	for (int i = 1; i <= rt; ++i) q[l + lt + i - 1] = rq[i];
	solve(lv, mid, l, l + lt - 1);
	solve(mid + 1, rv, l + lt, r);
}

int main() {
	n = get_num(), m = get_num();
	for (int i = 1; i <= n; ++i) {
		a[i] = get_num();
		q[++tot].op = 0, q[tot].x = i, q[tot].y = a[i], q[tot].z = 1;
	}
	char op[2];
	while (m--) {
		scanf("%s", op);
		if (op[0] == 'Q') {
			int l = get_num(), r = get_num(), k = get_num();
			q[++tot].op = ++qt, q[tot].x = l, q[tot].y = r, q[tot].z = k;
		}
		else if (op[0] == 'C') {
			int x = get_num(), y = get_num();
			q[++tot].op = -1, q[tot].x = x, q[tot].y = a[x], q[tot].z = -1;
			q[++tot].op = 0, q[tot].x = x, q[tot].y = y, q[tot].z = 1;
			a[x] = y;
		}
	}
	solve(0, inf, 1, tot);
	for (int i = 1; i <= qt; ++i) printf("%d\n", ans[i]);
	return 0;
}
