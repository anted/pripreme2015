#include <cstdio>
#include <cstring>
#include <algorithm>

using namespace std;

const int MAX = 55, INF = 0x3f3f3f3f;

int pref[MAX][MAX];
int dp[MAX][MAX][MAX][MAX];

int Fui(int l, int r, int u, int d)
{
  return pref[r][d] - pref[l-1][d] - pref[r][u-1] + pref[l-1][u-1];
}

int Rek(int l, int r, int u, int d)
{
  if (dp[l][r][u][d] != -1)
    return dp[l][r][u][d];

  if (l == r && u == d)
    return dp[l][r][u][d] = 0;

  int ret = INF;
  for (int i=l; i<r; i++)
    ret = min(ret, Rek(l, i, u, d) + Rek(i + 1, r, u, d));

  for (int i=u; i<d; i++)
    ret = min(ret, Rek(l, r, u, i) + Rek(l, r, i + 1, d));

  return dp[l][r][u][d] = ret + Fui(l, r, u, d);
}

int main()
{
  int brred, brstup;

  scanf("%d%d", &brred, &brstup);

  for (int i=1; i<=brred; i++) {
    int sum = 0;
    for (int j=1; j<=brstup; j++) {
      int tmp;
      scanf("%d", &tmp);
      sum += tmp;
      pref[i][j] = pref[i-1][j] + sum;
    }
  }

  memset(dp, -1, sizeof dp);
  printf("%d\n", Rek(1, brred, 1, brstup));

  return 0;
}
