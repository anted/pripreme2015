#include <cstdio>
#include <algorithm>
#include <queue>

using namespace std;

const int MAX = 2005;

int par[MAX], tez[MAX];
int zauz[MAX], poz[MAX];
int n, m;
queue <int> Q;

int Parkiraj(int ind)
{
  for (int i=0; i<n; i++) {
    if (!zauz[i]) {
      zauz[i] = 1;
      poz[ind] = i;
      return tez[ind] * par[i];
    }
  }

  return -1;
}

int main()
{
  int rez = 0;

  scanf("%d%d", &n, &m);

  for (int i=0; i<n; i++)
    scanf("%d", &par[i]);

  for (int i=1; i<=m; i++)
    scanf("%d", &tez[i]);
  
  for (int i=0; i<2*m; i++) {
    int tmp;
    scanf("%d", &tmp);
    if (tmp > 0) {
      int a = Parkiraj(tmp);
      if (a == -1)
        Q.push(tmp);
      else
        rez += a;
    }
    else {
      tmp = -tmp;
      zauz[poz[tmp]] = 0;
      if (!Q.empty()) {
        rez += Parkiraj(Q.front());
        Q.pop();
      }
    }
  }

  printf("%d\n", rez);

  return 0;
}
