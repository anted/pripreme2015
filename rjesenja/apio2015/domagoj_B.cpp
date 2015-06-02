#include <cstdio>
#include <cstring>
#include <algorithm>
#include <set>
#include <vector>
#include <deque>

#define X first
#define Y second

using namespace std;

typedef pair<int, int> P;
typedef unsigned long long ull;

const int MAX = 30005, INF = 0x3f3f3f3f;

struct str {
  int poz, pom, putt;
  str (int poz, int pom, int putt) : poz(poz), pom(pom), putt(putt) {}
  str () {}
};

int n, m;
ull bio[MAX][MAX/64+2];
int minput[MAX];
int done[MAX];
int poc=-1, kraj=-1;
vector <int> V[MAX];
deque <str> Q;

void StaviBio(int poz, int pom)
{
  bio[poz][pom/64] |= ((ull) 1) << (pom % 64);
}

int JelBio(int poz, int pom)
{
  return (bio[poz][pom/64] & (((ull) 1) << (pom % 64))) > 0;
}

int Unutar(int x)
{
  return x >= 0 && x < m;
}

int Bfs()
{
  memset(minput, INF, sizeof minput);
  minput[poc] = 0;
  Q.push_front(str(poc, 0, 0));

  for (; !Q.empty(); ) {
    str tmp = Q.front();
    Q.pop_front();

    if (JelBio(tmp.poz, tmp.pom))
      continue;
    StaviBio(tmp.poz, tmp.pom);

    if (tmp.poz == kraj)
      return tmp.putt;

    if (!done[tmp.poz]) {
      done[tmp.poz] = 1;
      for (int i=0; i<(int) V[tmp.poz].size(); i++) {
        int tp = V[tmp.poz][i];
        Q.push_front(str(tmp.poz, tp, tmp.putt));
      }
    }

    if (!tmp.pom)
      continue;
  
    for (int mnoz=-1; mnoz<=1; mnoz+=2) {
      int nn = tmp.poz + tmp.pom * mnoz;
      if (Unutar(nn)) {
        if (minput[nn] == INF) {
          minput[nn] = tmp.putt + 1;
          Q.push_back(str(nn, 0, tmp.putt + 1));
        }

        Q.push_back(str(nn, tmp.pom, tmp.putt + 1));
      }
    }
  }

  return -1;
}

int main()
{
  scanf("%d%d", &m, &n);

  for (int i=0; i<n; i++) {
    int poz, pom;
    scanf("%d%d", &poz, &pom);

    if (!i)
      poc = poz;
    if (i == 1)
      kraj = poz;

    V[poz].push_back(pom);
  }

  printf("%d\n", Bfs());

  return 0;
}
