#include <cstdio>
#include <algorithm>

using namespace std;

inline int read() {
	int num = 0;
	char c = getchar();
	while (c < '0' || c > '9') c = getchar();
	while (c >= '0' && c <= '9')
		num = num * 10 + c - '0', c = getchar();
	return num;
}

const int maxn = 1e5 + 5, maxk = 2e5 + 5;

struct Point {
	int x, y, z, cnt, f;

	bool operator == (const Point& rhs) const {
		return x == rhs.x && y == rhs.y && z == rhs.z;
	}

	bool operator < (const Point& rhs) const {
		if (x == rhs.x) {
			if (y == rhs.y) return z < rhs.z;
			return y < rhs.y;
		} return x < rhs.x;
	}
} p0[maxn], p[maxn], tmp[maxn];

int n, k, t, bit[maxk], d[maxn];

inline int ask(int x) {
	int ret = 0;
	while (x) ret += bit[x], x -= x & -x;
	return ret;
}

inline void add(int x, int d) {
	while (x <= k) bit[x] += d, x += x & -x;
}

void cdq(int l, int r) {
	if (l == r) return;
	int mid = (l + r) >> 1, lp = l, rp = mid + 1, tp = l;
	cdq(l, mid);
	cdq(mid + 1, r);
	while (lp <= mid || rp <= r) {
		if (rp > r || (lp <= mid && p[lp].y <= p[rp].y))
			add(p[lp].z, p[lp].cnt), tmp[tp++] = p[lp++];
		else p[rp].f += ask(p[rp].z), tmp[tp++] = p[rp++];
	}
	for (int i = l; i < lp; ++i) add(p[i].z, -p[i].cnt);
	for (int i = l; i <= r; ++i) p[i] = tmp[i];
}

int main() {
	n = read(), k = read();
	for (int i = 1; i <= n; ++i)
		p0[i].x = read(), p0[i].y = read(), p0[i].z = read();
	sort(p0 + 1, p0 + n + 1);
	for (int i = 1; i <= n; ++i) {
		if (t && p0[i] == p[t]) ++p[t].cnt;
		else p[++t] = p0[i], p[t].cnt = 1;
	}
	cdq(1, t);
	for (int i = 1; i <= t; ++i) d[p[i].f + p[i].cnt - 1] += p[i].cnt;
	for (int i = 1; i <= n; ++i) printf("%d\n", d[i - 1]);
	return 0;
}
