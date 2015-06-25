#include "traffic.h"

#include <vector>
#include <algorithm>
using namespace std;

const int MAXN = 1000010;

int N;
int A[MAXN];
int sum[MAXN];

vector<int> V[MAXN];

int dfs(int x, int prev) {
  sum[x] = A[x];
  for (int y: V[x]) {
    if (y == prev) continue;
    sum[x] += dfs(y, x);
  }
  return sum[x];
}

int LocateCentre(int n, int pp[], int S[], int D[]) {
  N = n;
  for (int i = 0; i < N; ++i) A[i] = pp[i];
  for (int i = 0; i < N-1; ++i) {
    V[S[i]].push_back(D[i]);
    V[D[i]].push_back(S[i]);
  }
  dfs(0, 0);
 
  int best = 0;
  static int mx[MAXN];

  for (int x = 0; x < N; ++x) {
    mx[x] = 0;
    for (int y: V[x]) {
      if (sum[y] < sum[x]) mx[x] = max(mx[x], sum[y]);
      else mx[x] = max(mx[x], sum[0] - sum[x]);
    }
    if (mx[x] < mx[best])
      best = x;
  }
  return best;
}
