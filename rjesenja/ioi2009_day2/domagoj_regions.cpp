#include <cstdio>
#include <algorithm>
#include <vector>

using namespace std;

const int KOR = 450, REG = 25010 + KOR, MAX = 200010;

int p[MAX];
int vel[REG];
int velike[KOR][REG];
int disc[MAX], fin[MAX];
int ind[REG];
int vr;
int sad;
vector <int> V[MAX];
vector <int> R[REG], S[REG];

void DfsPoc(int node)
{
  disc[node] = vr++;
  S[ind[p[node]]].push_back(disc[node]);
  R[ind[p[node]]].push_back(node);

  for (int i=0; i<(int) V[node].size(); i++)
    DfsPoc(V[node][i]);

  fin[node] = vr++;
}

void DfsVel(int reg, int node)
{
  if (p[node] == reg)
    sad++;
  velike[ind[reg]][ind[p[node]]] += sad;

  for (int i=0; i<(int) V[node].size(); i++)
    DfsVel(reg, V[node][i]);

  if (p[node] == reg)
    sad--;
}

int QMali(int a, int b)
{
  int ret = 0;

  for (int i=0; i<(int) R[a].size(); i++)
    ret += lower_bound(S[b].begin(), S[b].end(), fin[R[a][i]]) -
           lower_bound(S[b].begin(), S[b].end(), disc[R[a][i]]);

  return ret;
}

int Query(int a, int b)
{
  a--;
  b--;

  if (vel[a] > KOR)
    return velike[ind[a]][ind[b]];
  return QMali(ind[a], ind[b]);
}

int main()
{
  int n, r, q;

  scanf("%d%d%d", &n, &r, &q);

  for (int i=0; i<n; i++) {
    int par;
    if (i) {
      scanf("%d", &par); par--;
      V[par].push_back(i);
    }
    scanf("%d", &p[i]); p[i]--;
    vel[p[i]]++;
  }
  
  int inddol = 0, indgor = KOR;
  for (int i=0; i<r; i++) {
    if (vel[i] > KOR)
      ind[i] = inddol++;
    else
      ind[i] = indgor++;
  }
  
  DfsPoc(0);
  for (int i=0; i<r; i++)
    if (vel[i] > KOR)
      DfsVel(i, 0);
  
/*  for (int i=KOR; i<indgor; i++) {
    for (int j=0; j<(int) R[i].size(); j++)
      printf("%d ", R[i][j]);
    printf("\n");
  }*/

  for (int i=0; i<q; i++) {
    int a, b;
    scanf("%d%d", &a, &b);
    printf("%d\n", Query(a, b));
    fflush(stdout);
  }

  return 0;
}
