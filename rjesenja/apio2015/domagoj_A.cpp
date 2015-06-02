#include <cstdio>
#include <cstring>
#include <algorithm>

using namespace std;

const int MAX = 105, MAX1 = 2005, INF = 0x3f3f3f3f;

typedef long long ll;

ll sum[MAX1][MAX1];
int dp[MAX][MAX];
int dp1[MAX1];
int p[MAX1];
int n;
int a, b;

int Moze(ll rje)
{
  memset(dp, 0, sizeof dp);
  dp[0][1] = 1;

  for (int i=0; i<=n; i++)
    for (int j=1; j<=n; j++)
      if (dp[i][j])
        for (int k=j; k<=n; k++)
          if ((sum[j][k] | rje) == rje)
            dp[i+1][k+1] = 1;
  
  for (int i=a; i<=b; i++)
    if (dp[i][n+1])
      return 1;

  return 0;
}

int Moze1(ll rje)
{
  memset(dp1, INF, sizeof dp1);

  dp1[1] = 0;

  for (int i=1; i<=n; i++)
    for (int j=i; j<=n; j++)
      if ((sum[i][j] | rje) == rje)
        dp1[j+1] = min(dp1[j+1], dp1[i] + 1);

  return dp1[n+1] <= b;
}

int main()
{
  ll rje=0;

  scanf("%d%d%d", &n, &a, &b);

  for (int i=1; i<=n; i++)
    scanf("%d", &p[i]);

  for (int i=1; i<=n; i++)
    for (int j=i; j<=n; j++)
      sum[i][j] = sum[i][j-1] + p[j];
  
  for (int i=42; i>=0; i--) {
    ll tmp = rje | ((((ll) 1) << i) - 1);
    int st;

    if (a == 1)
      st = Moze1(tmp);
    else
      st = Moze(tmp);
    if (!st)
      rje |= ((ll) 1) << i;
  }

  printf("%lld\n", rje);

  return 0;
}
