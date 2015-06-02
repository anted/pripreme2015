#include <algorithm>
#include <cstdio>
#include <vector>

using namespace std;

const int MAXN = 500010;

int n;
int a[MAXN];
int pol[1<<20];
int off = (1<<19);

int query(int lo, int hi, int l = 0, int r = off, int curr = 1) {
//  printf("%d %d %d %d %d\n", lo, hi, l, r, curr);
  if (r <= lo || hi <= l) return 1000000000;
  if (l >= lo && r <= hi) return pol[curr];
  int mid = (l + r) / 2;
  return min(query(lo, hi, l, mid, curr*2), query(lo, hi, mid, r, curr*2+1));
}

int main() {
  for (int i = off; i < (1<<20); i++) pol[i] = 1000000000;

  scanf("%d", &n);

  for (int i = 0; i < n; i++) {
    scanf("%d", a + i);
  }

  int take = (n+1)/2;
  int sum = 0;

  for (int i = 0; i < take; i++) {
    sum += a[i];
  }

  pol[off+0] = sum;
  for (int i = 1; i < n; i++) {
    sum += a[(i+take-1)%n];
    sum -= a[i-1];
    pol[off+i] = sum;
  }
  for (int pot = 18; pot >= 0; pot--) {
    for (int i = (1<<pot); i < (1<<(pot+1)); i++) {
      pol[i] = min(pol[i*2], pol[i*2+1]);
    }
  }

  int best = 0;
  for (int i = 0; i < n; i++) {
    int lo = (i-take+1);
    int hi = i;
    int tmp = 0;
    if (lo < 0) {
      tmp = min( query(0, hi+1), query((lo+n)%n, n) );
    } else {
      tmp = query(lo, hi+1);
    }
    if (tmp > best) best = tmp;
  }

  printf("%d\n", best);

  return 0;
}
