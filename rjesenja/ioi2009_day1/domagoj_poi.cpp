#include <cstdio>
#include <algorithm>

using namespace std;

const int MAX = 2005;

int p[MAX][MAX];
int val[MAX];
int sum[MAX], rijes[MAX];
int sorted[MAX];

bool Manji(int a, int b)
{
  if (sum[a] != sum[b])
    return sum[a] > sum[b];
  if (rijes[a] != rijes[b])
    return rijes[a] > rijes[b];
  return a < b;
}

int main()
{
  int n, brt, ja;

  scanf("%d%d%d", &n, &brt, &ja);

  for (int i=0; i<brt; i++)
    val[i] = n;

  for (int i=0; i<n; i++) {
    for (int j=0; j<brt; j++) {
      scanf("%d", &p[i][j]);
      val[j] -= p[i][j];
      rijes[i] += p[i][j];
    }
  }

  for (int i=0; i<n; i++)
    for (int j=0; j<brt; j++)
      sum[i] += val[j] * p[i][j];

  for (int i=0; i<n; i++)
    sorted[i] = i;

  sort(sorted, sorted + n, Manji);

  printf("%d %d\n", sum[ja-1], (int) (lower_bound(sorted, sorted + n, ja - 1, Manji) - sorted + 1));

  return 0;
}
