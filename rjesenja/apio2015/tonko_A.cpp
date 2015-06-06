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
const llint inf = 1e9 * 3000LL;

int N, L, R;
int A[MAXN];
llint sum[MAXN];

int dp[250][250];
int dp2[MAXN];

void load() {
  scanf("%d%d%d", &N, &L, &R);
  for (int i = 0; i < N; ++i) {
    scanf("%d", A+i);
    sum[i+1] = sum[i] + A[i];
  }
}

llint lim;

int rec(int x, int y) {
  if (x == N) return (int)(y >= L && y <= R);
  if (y > R) return 0;
  int &ret = dp[x][y];
  if (ret != -1) return ret;
  ret = 0;
  for (int i = x; i < N; ++i) {
    llint a = lim | (sum[i+1] - sum[x]);
    if (a == lim && rec(i+1, y+1))
      ret = 1;
  }
  return ret;
}

int rec2(int x) {
  if (x == N) return 0;
  int &ret = dp2[x];
  if (ret != -1) return ret;
  ret = 10000;
  for (int i = x; i < N; ++i) {
    llint a = lim | (sum[i+1] - sum[x]);
    if (a == lim)
      ret = min(ret, 1 + rec2(i+1));
  }
  return ret;
}

void solve() {
  llint ans = 0;
  for (int i = 45; i >= 0; --i) {
    lim = ans + (1LL << i) - 1;
    
    if (N <= 200) {  
      memset(dp, -1, sizeof dp); 
      if (!rec(0, 0))
        ans |= (1LL << i);
    }
    else {
      memset(dp2, -1, sizeof dp2);
      if (rec2(0) > R)
        ans |= (1LL << i);
    }
  
  }
  printf("%lld\n", ans);
}

int main(void) {
  load();
  solve();
  return 0;
}

