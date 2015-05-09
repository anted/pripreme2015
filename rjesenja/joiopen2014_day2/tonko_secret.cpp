#include "secret.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <algorithm>
using namespace std;

const int MAXN = 2000;

int N;
int A[MAXN];

int tree[4000];
int L[30][4000];
int R[30][4000];
int lo, hi;

int get(int dep, int l, int r, int qL, int qR) {
  if (qL == qR - 1) return A[qL];
  if (qL == qR - 2) return Secret(A[qL], A[qL+1]);

  const int mid = (l + r) / 2;
  if (r - l < 2) return A[l];
  if (mid > qL && mid < qR) return Secret(L[dep][qL], R[dep][qR]);
  if (l == qL && r == qR) return Secret(L[dep][l], R[dep][r]);
  if (qR == mid) return L[dep][qL];
  if (qL == mid) return R[dep][qR];
 
  
  if (qL > mid) return get(dep+1, mid, r, qL, qR);
  if (qR < mid) return get(dep+1, l, mid, qL, qR);
}

void build(int dep, int l, int r) {
  const int mid = (l + r) / 2;
  if (l + 2 == r) {
    L[dep][l] = A[l];
    R[dep][r] = A[r-1];
    return;
  }
  build(dep+1, l, mid);
  build(dep+1, mid, r);
  for (int i = l; i < mid; ++i)
    L[dep][i] = get(dep+1, l, mid, i, mid);
  for (int i = mid; i < r; ++i)
    R[dep][i+1] = get(dep+1, mid, r, mid, i+1);
}

void Init(int n, int a[]) {
  N = n;
  for (int i = 0; i < N; ++i) A[i] = a[i];
  
  build(0, 0, 1024);
}

int Query(int l, int r) {
  return get(0, 0, 1024, l, r+1);
}
