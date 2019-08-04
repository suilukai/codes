#include <cstdio>
#include <cstdlib>

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

const int maxn = 1e5 + 5, inf = 0x3f3f3f3f;

struct Treap {
	int l, r, val, key, cnt, size;
	#define lc t[p].l
	#define rc t[p].r
} t[maxn];

int tot, root;

inline int add(int val) {
	t[++tot].val = val;
	t[tot].key = rand();
	t[tot].cnt = t[tot].size = 1;
	return tot;
}

inline void update(int p) {
	t[p].size = t[lc].size + t[rc].size + t[p].cnt;
}

inline void build() {
	add(-inf), add(inf);
	root = 1, t[1].r = 2;
	update(root);
}

inline void rturn(int& p) {
	int q = lc;
	lc = t[q].r, t[q].r = p, p = q;
	update(rc), update(p);
}

inline void lturn(int& p) {
	int q = rc;
	rc = t[q].l, t[q].l = p, p = q;
	update(lc), update(p);
}

void insert(int& p, int val) {
	if (!p) {
		p = add(val);
		return;
	}
	if (val == t[p].val) {
		++t[p].cnt, update(p);
		return;
	}
	if (val < t[p].val) {
		insert(lc, val);
		if (t[lc].key > t[p].key) rturn(p);
	}
	else {
		insert(rc, val);
		if (t[rc].key > t[p].key) lturn(p);
	}
	update(p);
}

void remove(int& p, int val) {
	if (!p) return;
	if (val == t[p].val) {
		if (t[p].cnt > 1) {
			--t[p].cnt, update(p);
			return;
		}
		if (lc || rc) {
			if (!rc || t[lc].key > t[rc].key) rturn(p), remove(rc, val);
			else lturn(p), remove(lc, val);
			update(p);
		} else p = 0;
	}
	if (val < t[p].val) remove(lc, val);
	else remove(rc, val);
	update(p);
}

int getrank(int p, int val) {
	if (val == t[p].val) return t[lc].size + 1;
	if (val < t[p].val) return getrank(lc, val);
	return getrank(rc, val) + t[lc].size + t[p].cnt;
}

int getval(int p, int rank) {
	if (rank <= t[lc].size) return getval(lc, rank);
	if (rank <= t[lc].size + t[p].cnt) return t[p].val;
	return getval(rc, rank - t[lc].size - t[p].cnt);
}

inline int getpre(int val) {
	int ans = 1, p = root;
	while (p) {
		if (val == t[p].val) {
			if (lc) {
				p = lc;
				while (rc) p = rc;
				ans = p;
			}
			break;
		}
		if (t[p].val < val && t[p].val > t[ans].val) ans = p;
		p = val < t[p].val ? lc : rc;
	}
	return t[ans].val;
}

inline int getnext(int val) {
	int ans = 2, p = root;
	while (p) {
		if (val == t[p].val) {
			if (rc) {
				p = rc;
				while (lc) p = lc;
				ans = p;
			}
			break;
		}
		if (t[p].val > val && t[p].val < t[ans].val) ans = p;
		p = val < t[p].val ? lc : rc;
	}
	return t[ans].val;
}

int main() {
	build();
	int n = get_num();
	for (int i = 1; i <= n; ++i) {
		int op = get_num(), x = get_num();
		if (op == 1) insert(root, x);
		else if (op == 2) remove(root, x);
		else if (op == 3) printf("%d\n", getrank(root, x) - 1);
		else if (op == 4) printf("%d\n", getval(root, x + 1));
		else if (op == 5) printf("%d\n", getpre(x));
		else printf("%d\n", getnext(x));
	}
	return 0;
}
