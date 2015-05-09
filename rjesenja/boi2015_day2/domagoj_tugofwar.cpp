#include <cstdio>
#include <cstring>
#include <algorithm>
#include <vector>
#include <queue>

#define X first
#define Y second

using namespace std;

typedef pair<int, int> P;
typedef unsigned long long ull;

const int MAX = 60005, POL = 600100, VEL = (2 * POL) / 64 + 5;

vector <P> V[MAX];
queue <int> Q;
int l[MAX], r[MAX], s[MAX];
int brnode, bredge;
int deg[MAX];
int bio[MAX], bioedge[MAX], cik[MAX];
int n;
int brcik=0;
int valc[MAX][2];
int minn=POL, maxx=POL;
ull dp[2][VEL];

void DfsPoc(int node)
{
  bio[node] = 1;
  brnode++;
  bredge += (int) V[node].size();

  for (int i=0; i<(int) V[node].size(); i++)
    if (!bio[V[node][i].X])
      DfsPoc(V[node][i].X);
}

int Prov()
{
  for (int i=0; i<2*n; i++) {
    if (!bio[i]) {
      brnode = bredge = 0;
      DfsPoc(i);
      if (brnode * 2 != bredge)
        return 0;
    }
  }

  return 1;
}

void Stabla()
{
  memset(bio, 0, sizeof bio);

  for (int i=0; i<2*n; i++)
    if (deg[i] == 1)
      Q.push(i);

  for (; !Q.empty(); ) {
    int tmp = Q.front();
    bio[tmp] = 1;
    Q.pop();
    for (int i=0; i<(int) V[tmp].size(); i++) {
      int nn = V[tmp][i].X;
      deg[nn]--;
      if (deg[nn] == 1)
        Q.push(nn);
    }
  }
}

int Dfs(int node, int prosli)
{
  int ret = 0;

  for (int i=0; i<(int) V[node].size(); i++) {
    int nn = V[node][i].X, e = V[node][i].Y;
    if (!cik[nn] && nn != prosli) {
      if (nn < n)
        ret += s[e];
      else
        ret -= s[e];

      ret += Dfs(nn, node);
    }
  }

  return ret;
}

void Ciklusi()
{
  vector <int> T;
  
  for (int i=0; i<2*n; i++) {
    if (!bio[i]) {
      T.clear();
      int tmpval = 0;
      for (int kol=0, j=i, sz=0, st=1; j != i || !sz; sz++, st *= -1, kol++) {
        bio[j] = cik[j] = 1;        
        T.push_back(j);
        for (int k=0; k<(int) V[j].size(); k++) {
          int nn = V[j][k].X, e = V[j][k].Y;
          if (!bio[nn] || (cik[nn] && !bioedge[e])) {
            bioedge[e] = 1;
            j = nn;
            tmpval += st * s[e];
            break;
          }
        }
      }

      int ost=0;
      for (int i=0; i<(int) T.size(); i++)
        ost += Dfs(T[i], -1);

      valc[brcik][0] = tmpval + ost;
      valc[brcik][1] = -tmpval + ost;
      brcik++;
    }    
  }
}

void Stavi(int ind, int val)
{
  int pom, ost;
  if (val >= 0) {
    pom = val / 64;
    ost = val % 64;
  }
  else {
    pom = (val - 63) / 64;
    ost = (val % 64 + 64) % 64;
  }

  for (int i=minn/64; i*64 <= maxx; i++) {
    dp[1^ind][i+pom] |= dp[ind][i] << ost;
    dp[1^ind][i+pom+1] |= dp[ind][i] >> (64 - ost);
  }
}

void Ispis(int ind)
{
  for (int i=0; i<2*POL; i++)
    if (dp[ind][i/64] & (((ull) 1) << (i % 64)))
      printf("%d ", i - POL);
  printf("\n");
}

int main()
{
  int raz;

  scanf("%d%d", &n, &raz);

  for (int i=0; i<2*n; i++) {
    scanf("%d%d%d", &l[i], &r[i], &s[i]); l[i]--; r[i]--;
    V[l[i]].push_back(P(r[i] + n, i));
    V[r[i] + n].push_back(P(l[i], i));
    deg[l[i]]++;
    deg[r[i] + n]++;
  }

  if (!Prov())
    printf("NO\n");
  else {
    Stabla();
    Ciklusi();

    dp[0][POL/64] |= ((ull) 1) << (POL % 64);
    int ind=0;
    for (int i=0; i<brcik; i++, ind ^= 1) {
      for (int j=0; j<2; j++)
        Stavi(ind, valc[i][j]);

      for (int i=minn/64; (i-1)*64<=maxx; i++)
        dp[ind][i] = 0;

      maxx += max(valc[i][0], valc[i][1]);
      minn += min(valc[i][0], valc[i][1]);
    }
  
    int da=0;
    for (int i=POL-raz; i<=POL+raz; i++)
      if (dp[ind][i/64] & (((ull) 1) << (i % 64)))
        da = 1;

    if (da)
      printf("YES\n");
    else
      printf("NO\n");
  }

  return 0;
}

