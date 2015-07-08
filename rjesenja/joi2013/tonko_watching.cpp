#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <iostream>
#include <algorithm>
#include <vector>
using namespace std;

typedef long long llint;

const int MAXN = 2010;

int N, P, Q;
int A[MAXN];

inline int Min(int x, int y) { return x < y ? x : y; }

void load() {
  scanf("%d%d%d", &N, &P, &Q);
  for (int i = 0; i < N; ++i) scanf("%d", &A[i]);
  sort(A, A + N);
}

short dp[MAXN][MAXN];
int W;

int nxt[MAXN][2];

short rec(int pos, short q) {
  if (pos == N) return 0;

  short &ret = dp[pos][q];
  if (ret != -1) return ret;
  
  ret = 1e4;

  // slucaj kad dodajem +1
  ret = Min(ret, 1 + rec(nxt[pos][0], q));
  // slucaj kad povecam q
  if (q < Q) {
    ret = Min(ret, rec(nxt[pos][1], q+1));
  }

  return ret;
}

bool check(int w) {
  W = w;
  memset(dp, -1, sizeof dp);
  
  for (int i = 0; i < N; ++i) {
    nxt[i][0] = lower_bound(A, A+N, A[i]+w) - A;
    nxt[i][1] = lower_bound(A, A+N, A[i]+w+w) - A;
  }

  int ret = rec(0, 0);
  return ret <= P;
}

void solve() {
  if (P+Q >= N) {
    printf("1\n");
    return;
  }
  int lo = 1, hi = 1e9;
  while (lo < hi) {
    int mid = (lo + hi) / 2;
    if (!check(mid)) lo = mid+1;
    else hi = mid;
  }
  printf("%d\n", lo);
}

int main(void) {
  load();
  solve();
  return 0;
}

