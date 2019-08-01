#include <cstdio>
#include <iostream>

using namespace std;

inline int get_num() {
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
int f[maxn], depth[maxn], size[maxn];
int order[maxn], dfs_order[maxn], cnt, chain[maxn], first[maxn], tot;

int dfs1(int u, int fa) {
	f[u] = fa;
	depth[u] = depth[fa] + 1;
	size[u] = 1;
	for (int p = head[u]; p; p = edge[p].next) {
		int v = edge[p].v;
		if (v != fa) size[u] += dfs1(v, u);
	}
	return size[u];
}

void dfs2(int u, int fa, int ch) {
	order[u] = ++cnt;
	dfs_order[cnt] = u;
	int id = 0;
	for (int p = head[u]; p; p = edge[p].next) {
		int v = edge[p].v;
		if (v != fa && size[v] > size[id]) id = v;
	}
	if (!id) return;
	chain[id] = ch, first[id] = first[u];
	dfs2(id, u, ch);
	for (int p = head[u]; p; p = edge[p].next) {
		int v = edge[p].v;
		if (v == fa || v == id) continue;
		chain[v] = ++tot, first[v] = v;
		dfs2(v, u, tot);
	}
}

struct ST {
	int l, r, sum, add;
} t[4 * maxn];

inline void up(int p) {
	t[p].sum = (t[2 * p].sum + t[2 * p + 1].sum) % mod;
}

inline void mark(int p, int d) {
	t[p].sum = (t[p].sum + 1ll * d * (t[p].r - t[p].l + 1) % mod) % mod;
	t[p].add = (t[p].add + d) % mod;
}

inline void down(int p) {
	if (t[p].add) {
		mark(2 * p, t[p].add);
		mark(2 * p + 1, t[p].add);
		t[p].add = 0;
	}
}

void build(int p, int l, int r) {
	t[p].l = l, t[p].r = r;
	t[p].add = 0;
	if (l == r) {
		t[p].sum = a[dfs_order[l]];
		return;
	}
	int mid = (l + r) / 2;
	build(2 * p, l, mid);
	build(2 * p + 1, mid + 1, r);
	up(p);
}

void modify0(int p, int l, int r, int d) {
	if (l <= t[p].l && t[p].r <= r) {
		mark(p, d);
		return;
	}
	down(p);
	int mid = (t[p].l + t[p].r) / 2;
	if (l <= mid) modify0(2 * p, l, r, d);
	if (r > mid) modify0(2 * p + 1, l, r, d);
	up(p);
}

int query0(int p, int l, int r) {
	if (l <= t[p].l && t[p].r <= r) return t[p].sum;
	down(p);
	int mid = (t[p].l + t[p].r) / 2, sum = 0;
	if (l <= mid) sum = (sum + query0(2 * p, l, r)) % mod;
	if (r > mid) sum = (sum + query0(2 * p + 1, l, r)) % mod;
	return sum;
}

inline void modify(int p1, int p2, int d) {
	while (chain[p1] != chain[p2]) {
		if (depth[first[p1]] < depth[first[p2]]) swap(p1, p2);
		int p = first[p1];
		modify0(1, order[p], order[p1], d);
		p1 = f[p];
	}
	if (depth[p1] > depth[p2]) swap(p1, p2);
	modify0(1, order[p1], order[p2], d);
}

inline int query(int p1, int p2) {
	int sum = 0;
	while (chain[p1] != chain[p2]) {
		if (depth[first[p1]] < depth[first[p2]]) swap(p1, p2);
		int p = first[p1];
		sum = (sum + query0(1, order[p], order[p1])) % mod;
		p1 = f[p];
	}
	if (depth[p1] > depth[p2]) swap(p1, p2);
	sum = (sum + query0(1, order[p1], order[p2])) % mod;
	return sum;
}

int main() {
	n = get_num(), m = get_num(), r = get_num(), mod = get_num();
	for (int i = 1; i <= n; ++i) a[i] = get_num() % mod;
	for (int i = 1; i <= n - 1; ++i) {
		int u = get_num(), v = get_num();
		insert(u, v);
		insert(v, u);
	}
	dfs1(r, 0);
	chain[r] = ++tot, first[r] = r;
	dfs2(r, 0, tot);
	build(1, 1, n);
	for (int i = 1; i <= m; ++i) {
		 int op = get_num();
		 if (op == 1) {
		 	int x = get_num(), y = get_num(), z = get_num();
			modify(x, y, z);
		 }
		 else if (op == 2) {
		 	int x = get_num(), y = get_num();
			printf("%d\n", query(x, y));
		 }
		 else if (op == 3) {
		 	int x = get_num(), z = get_num();
			modify0(1, order[x], order[x] + size[x] - 1, z);
		 }
		 else {
		 	int x = get_num();
			printf("%d\n", query0(1, order[x], order[x] + size[x] - 1));
		 }
	}
	return 0;
}
