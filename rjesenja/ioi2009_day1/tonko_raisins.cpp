#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <iostream>
#include <algorithm>
#include <vector>
using namespace std;

typedef long long llint;

const int MAXN = 60;
const int inf = 1e9;

int N, M;
int A[MAXN][MAXN];
int sum[MAXN][MAXN];

int get(int x1, int y1, int x2, int y2) {
  return sum[x2][y2] + sum[x1][y1] - sum[x2][y1] - sum[x1][y2];
}

int dp[MAXN][MAXN][MAXN][MAXN];

int solve(int lx, int ly, int hx, int hy) {
  if (lx == hx-1 && ly == hy-1) return 0;
  int &ret = dp[lx][ly][hx][hy];
  if (ret != -1) return ret;
  ret = inf;
  for (int i = lx+1; i < hx; ++i) {
    ret = min(ret, solve(lx, ly, i, hy) + solve(i, ly, hx, hy));
  }
  for (int i = ly+1; i < hy; ++i) {
    ret = min(ret, solve(lx, ly, hx, i) + solve(lx, i, hx, hy));
  }
  ret += get(lx, ly, hx, hy);
  return ret;
} 

int main(void) {
 
  scanf("%d%d", &N, &M);
  for (int i = 0; i < N; ++i)
    for (int j = 0; j < M; ++j)
      scanf("%d", &A[i][j]);

  for (int i = 1; i <= N; ++i)
    for (int j = 1; j <= M; ++j)
      sum[i][j] = sum[i-1][j] + sum[i][j-1] - sum[i-1][j-1] + A[i-1][j-1];

  memset(dp, -1, sizeof dp);
  printf("%d\n", solve(0, 0, N, M));

  return 0;
}

