#include <cstdio>
#include <iostream>

using namespace std;

inline int read() {
	int num = 0;
	char c = getchar();
	while (c < '0' || c > '9') c = getchar();
	while (c >= '0' && c <= '9')
		num = num * 10 + c - '0', c = getchar();
	return num;
}

const int maxn = 1e5 + 5;

int head[maxn], eid;

struct Edge {
	int v, next;
} edge[2 * maxn];

inline void insert(int u, int v) {
	edge[++eid].v = v;
	edge[eid].next = head[u];
	head[u] = eid;
}

int n, m, r, mod, a[maxn];
int f[maxn], depth[maxn], size[maxn], best[maxn];
int seq[maxn], order[maxn], cnt, first[maxn];

void dfs1(int u, int fa) {
	f[u] = fa, depth[u] = depth[fa] + 1, size[u] = 1;
	for (int p = head[u]; p; p = edge[p].next) {
		int v = edge[p].v;
		if (v == fa) continue;
		dfs1(v, u);
		size[u] += size[v];
		if (size[v] > size[best[u]]) best[u] = v;
	}
}

void dfs2(int u, int h) {
	seq[++cnt] = u, order[u] = cnt, first[u] = h;
	if (!best[u]) return;
	dfs2(best[u], h);
	for (int p = head[u]; p; p = edge[p].next) {
		int v = edge[p].v;
		if (v == f[u] || v == best[u]) continue;
		dfs2(v, v);
	}
}

struct ST {
	int l, r, sum, add;
} t[4 * maxn];

inline void up(int p) {
	t[p].sum = (t[2 * p].sum + t[2 * p + 1].sum) % mod;
}

inline void mark(int p, int d) {
	t[p].sum = (t[p].sum + 1ll * (t[p].r - t[p].l + 1) * d % mod) % mod;
	t[p].add = (t[p].add + d) % mod;
}

inline void down(int p) {
	if (t[p].add) {
		mark(2 * p, t[p].add), mark(2 * p + 1, t[p].add);
		t[p].add = 0;
	}
}

void build(int p, int l, int r) {
	t[p].l = l, t[p].r = r, t[p].add = 0;
	if (l == r) {
		t[p].sum = a[seq[l]];
		return;
	}
	int mid = (l + r) >> 1;
	build(2 * p, l, mid);
	build(2 * p + 1, mid + 1, r);
	up(p);
}

void modify(int p, int x, int y, int d) {
	if (x <= t[p].l && t[p].r <= y) {
		mark(p, d);
		return;
	}
	down(p);
	int mid = (t[p].l + t[p].r) >> 1;
	if (x <= mid) modify(2 * p, x, y, d);
	if (y > mid) modify(2 * p + 1, x, y, d);
	up(p);
}

int query(int p, int x, int y) {
	if (x <= t[p].l && t[p].r <= y) return t[p].sum;
	down(p);
	int mid = (t[p].l + t[p].r) >> 1, ret = 0;
	if (x <= mid) ret = (ret + query(2 * p, x, y)) % mod;
	if (y > mid) ret = (ret + query(2 * p + 1, x, y)) % mod;
	return ret;
}

inline void modify0(int p1, int p2, int d) {
	while (first[p1] != first[p2]) {
		if (depth[first[p1]] < depth[first[p2]]) swap(p1, p2);
		modify(1, order[first[p1]], order[p1], d);
		p1 = f[first[p1]];
	}
	if (depth[p1] > depth[p2]) swap(p1, p2);
	modify(1, order[p1], order[p2], d);
}

inline int query0(int p1, int p2) {
	int ret = 0;
	while (first[p1] != first[p2]) {
		if (depth[first[p1]] < depth[first[p2]]) swap(p1, p2);
		ret = (ret + query(1, order[first[p1]], order[p1])) % mod;
		p1 = f[first[p1]];
	}
	if (depth[p1] > depth[p2]) swap(p1, p2);
	ret = (ret + query(1, order[p1], order[p2])) % mod;
	return ret;
}

int main() {
	n = read(), m = read(), r = read(), mod = read();
	for (int i = 1; i <= n; ++i) a[i] = read() % mod;
	for (int i = 1; i < n; ++i) {
		int u = read(), v = read();
		insert(u, v), insert(v, u);
	}
	dfs1(r, 0);
	dfs2(r, r);
	build(1, 1, n);
	while (m--) {
		int op = read(), x, y, z;
		switch(op) {
		case 1: x = read(), y = read(), z = read(), modify0(x, y, z);break;
		case 2: x = read(), y = read(), printf("%d\n", query0(x, y));break;
		case 3: x = read(), z = read(), modify(1, order[x], order[x] + size[x] - 1, z);break;
		case 4: x = read(), printf("%d\n", query(1, order[x], order[x] + size[x] - 1));break;
		}
	}
	return 0;
}
