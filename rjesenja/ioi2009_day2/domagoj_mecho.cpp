#include <cstdio>
#include <cstring>
#include <algorithm>
#include <queue>

using namespace std;

#define X first
#define Y second

typedef pair<int, int> P;

const int MAX = 805, INF = 0x3f3f3f3f;

char p[MAX][MAX];
int pc[MAX][MAX];
int bio[MAX][MAX];
int n;
int brkor;
int pr[4] = {1, 0, -1, 0}, ps[4] = {0, 1, 0, -1};
queue <P> Q;

int Vani(int r, int s)
{
  return r < 0 || s < 0 || r >= n || s >= n;
}

int MozePc(int r, int s)
{
  return !Vani(r, s) && p[r][s] != 'T' && p[r][s] != 'D';
}

int MozeMed(int r, int s)
{
  return !Vani(r, s) && p[r][s] != 'T';
}

void BfsPcele()
{
  memset(pc, INF, sizeof pc);
  
  for (int i=0; i<n; i++)
    for (int j=0; j<n; j++)
      if (p[i][j] == 'H') {
        pc[i][j] = 0;
        Q.push(P(i, j));
      }

  for (; !Q.empty(); ) {
    P tmp = Q.front();
    Q.pop();
    for (int i=0; i<4; i++) {
      int nr = tmp.X + pr[i];
      int ns = tmp.Y + ps[i];
      if (MozePc(nr, ns) && pc[nr][ns] == INF) {
        pc[nr][ns] = pc[tmp.X][tmp.Y] + 1;
        Q.push(P(nr, ns));
      }
    }
  }
}

int BfsMedo(int t)
{
  memset(bio, INF, sizeof bio);
  for (; !Q.empty(); Q.pop());

  for (int i=0; i<n; i++)
    for (int j=0; j<n; j++)
      if (p[i][j] == 'M') {
        bio[i][j] = brkor * t;
        Q.push(P(i, j));
        if (pc[i][j] <= t)
          return 0;
      }

  for (; !Q.empty();) {
    P tmp = Q.front();
    Q.pop();

    if (p[tmp.X][tmp.Y] == 'D')
      return 1;
  
    for (int i=0; i<4; i++) {
      int nr = tmp.X + pr[i];
      int ns = tmp.Y + ps[i];
      int tt = bio[tmp.X][tmp.Y];
      int sek = (tt + 1) / brkor;
      if (MozeMed(nr, ns) && bio[nr][ns] == INF && sek < pc[nr][ns]) {        
        bio[nr][ns] = tt + 1;
        Q.push(P(nr, ns));
      }
    }
  }

  return 0;
}

int main()
{
  scanf("%d%d", &n, &brkor);

  for (int i=0; i<n; i++)
    scanf("%s", p[i]);

  BfsPcele();

  int lo = 0, hi = MAX * MAX;
  for (; lo < hi; ) {
    int mid = (lo + hi + 1) / 2;
    if (BfsMedo(mid))
      lo = mid;
    else
      hi = mid - 1;
  }

/*  for (int i=0; i<n; i++) {
    for (int j=0; j<n; j++) {
      if (pc[i][j] < INF)
        printf("%2d ", pc[i][j]);
      else
        printf("-1 ");
    }

    printf("\n");
  }

  printf("KKK %d\n", lo);*/

  if (BfsMedo(lo))
    printf("%d\n", lo);
  else
    printf("-1\n");

  return 0;
}
