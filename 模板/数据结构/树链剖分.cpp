#include <cstdio>
#include <iostream>
#include <algorithm>

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

int n, m, r, mod, a[maxn], size[maxn], depth[maxn], f[maxn];
int chain[maxn], first[maxn], tot = 1;
int order[maxn], dfs_order[maxn], cnt;

int dfs0(int u, int fa) {
    size[u] = 1;
    depth[u] = depth[fa] + 1;
    f[u] = fa;
    for (int p = head[u]; p; p = edge[p].next) {
        int v = edge[p].v;
        if (v != fa) size[u] += dfs0(v, u);
    }
    return size[u];
}

void dfs(int u, int fa, int ch) {
    chain[u] = ch;
    order[u] = ++cnt;
    dfs_order[cnt] = u;
    int id = 0;
    for (int p = head[u]; p; p = edge[p].next) {
        int v = edge[p].v;
        if (v != fa && size[v] > size[id]) id = v;
    }
    if (!id) return;
    dfs(id, u, ch);
    for (int p = head[u]; p; p = edge[p].next) {
        int v = edge[p].v;
        if (v != fa && v != id) {
            first[++tot] = v;
            dfs(v, u, tot);
        }
    }
}

struct ST {
    int l, r, sum, tag;
} st[4 * maxn];

inline void up(int p) {
    st[p].sum = (st[2 * p].sum + st[2 * p + 1].sum) % mod;
}

inline void mark(int p, int d) {
    st[p].sum = (st[p].sum + d * (st[p].r - st[p].l + 1)) % mod;
    st[p].tag = (st[p].tag + d) % mod;
}

inline void down(int p) {
    if (st[p].tag) {
        mark(2 * p, st[p].tag);
        mark(2 * p + 1, st[p].tag);
        st[p].tag = 0;
    }
}

void build(int p, int l, int r) {
    st[p].l = l, st[p].r = r;
    if (l == r) {
        st[p].sum = a[dfs_order[l]] % mod;
        return;
    }
    int mid = (l + r) / 2;
    build(2 * p, l, mid);
    build(2 * p + 1, mid + 1, r);
    up(p);
}

int query0(int p, int l, int r) {
    if (l <= st[p].l && st[p].r <= r) return st[p].sum;
    down(p);
    int mid = (st[p].l + st[p].r) / 2, ret = 0;
    if (l <= mid) ret = (ret + query0(2 * p, l, r)) % mod;
    if (r > mid) ret = (ret + query0(2 * p + 1, l, r)) % mod;
    return ret;
}

void modify0(int p, int l, int r, int d) {
    if (l <= st[p].l && st[p].r <= r) {
        mark(p, d);
        return;
    }
    down(p);
    int mid = (st[p].l + st[p].r) / 2;
    if (l <= mid) modify0(2 * p, l, r, d);
    if (r > mid) modify0(2 * p + 1, l, r, d);
    up(p);
}

inline int query(int p1, int p2) {
    int ans = 0;
    while (chain[p1] != chain[p2]) {
        if (depth[first[chain[p1]]] < depth[first[chain[p2]]]) swap(p1, p2);
        int p = first[chain[p1]];
        ans = (ans + query0(1, order[p], order[p1])) % mod;
        p1 = f[p];
    }
    if (depth[p1] > depth[p2]) swap(p1, p2);
    ans = (ans + query0(1, order[p1], order[p2])) % mod;
    return ans;
}

inline void modify(int p1, int p2, int d) {
    while (chain[p1] != chain[p2]) {
        if (depth[first[chain[p1]]] < depth[first[chain[p2]]]) swap(p1, p2);
        int p = first[chain[p1]];
        modify0(1, order[p], order[p1], d);
        p1 = f[p];
    }
    if (depth[p1] > depth[p2]) swap(p1, p2);
    modify0(1, order[p1], order[p2], d);
}

int main() {
    n = get_num(), m = get_num(), r = get_num(), mod = get_num();
    for (int i = 1; i <= n; ++i) a[i] = get_num();
    for (int i = 1; i <= n - 1; ++i) {
        int x = get_num(), y = get_num();
        insert(x, y);
        insert(y, x);
    }
    dfs0(r, 0);
    first[tot] = r;
    dfs(r, 0, tot);
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
            int x = get_num(), y = get_num();
            modify0(1, order[x], order[x] + size[x] - 1, y);
        }
        else {
            int x = get_num();
            printf("%d\n", query0(1, order[x], order[x] + size[x] - 1));
        }
    }
    return 0;
}
