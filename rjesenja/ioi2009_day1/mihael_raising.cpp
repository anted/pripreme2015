#include <algorithm>
#include <cstdio>

using namespace std;

const int MAXN = 55;

int n, m;
int mat[MAXN][MAXN];

int bio[MAXN][MAXN][MAXN][MAXN];
int dp[MAXN][MAXN][MAXN][MAXN];

int pom_dp[MAXN][MAXN];

int sum(int x, int y, int r, int c) {
  int rez = pom_dp[x+r-1][y+c-1];
  if (x != 0) rez -= pom_dp[x-1][y+c-1];
  if (y != 0) rez -= pom_dp[x+r-1][y-1];
  if (x != 0 && y != 0) rez += pom_dp[x-1][y-1];
  return rez;
}

int rek(int x, int y, int r, int c) {
  if (r == 1 && c == 1) return 0;
  if (bio[x][y][r][c]) return dp[x][y][r][c];
  bio[x][y][r][c] = 1;
  int best = 1e9;
  for (int i = 1; i < r; i++) {
    best = min(best, rek(x, y, i, c) + rek(x+i, y, r-i, c));
  }
  for (int i = 1; i < c; i++) {
    best = min(best, rek(x, y, r, i) + rek(x, y+i, r, c-i));
  }
  return dp[x][y][r][c] = best + sum(x, y, r, c);
}

int main() {
  scanf("%d%d", &n, &m);

  for (int i = 0; i < n; i++) {
    for (int j = 0; j < m; j++) {
      scanf("%d", &mat[i][j]);
      pom_dp[i][j] = mat[i][j];
      if (i != 0) pom_dp[i][j] += pom_dp[i-1][j];
      if (j != 0) pom_dp[i][j] += pom_dp[i][j-1];
      if (i != 0 && j != 0) pom_dp[i][j] -= pom_dp[i-1][j-1];
    }
  }

  printf("%d\n", rek(0, 0, n, m));

  return 0;
}
