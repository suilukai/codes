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

int a[maxn];

void qsort(int l, int r) {
	int i = l, j = r, mid = a[(l + r) / 2];
	while (i <= j) {
		while (a[i] < mid) ++i;
		while (a[j] > mid) --j;
		if (i <= j) swap(a[i++], a[j--]);
	}
	if (i < r) qsort(i, r);
	if (j > l) qsort(l, j);
}

int main() {
	int n = get_num();
	for (int i = 1; i <= n; ++i) a[i] = get_num();
	qsort(1, n);
	for (int i = 1; i <= n; ++i) printf("%d ", a[i]);
	return 0;
}
