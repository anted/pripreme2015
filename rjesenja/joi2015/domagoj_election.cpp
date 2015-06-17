#include <cstdio>
#include <algorithm>
#include <vector>

using namespace std;

const int MAX = 100050;
int DEB = 0;

typedef pair<int, int> P;
#define X first
#define Y second

vector <int> V[MAX];
vector <int> Q[MAX];
int fench[MAX], fenja[MAX];
int sub[MAX], par[MAX], poz[MAX], dub[MAX], root[MAX], slj[MAX];
int ukpoz=0;
int a[MAX], b[MAX], c[MAX], w[MAX], lc[MAX];
int dp[MAX];
P sorted[MAX];

void Stavi(int fen[MAX], int pos, int val)
{
  for (pos++; pos < MAX; pos += pos & -pos)
    fen[pos] += val;
}

int Vrati(int fen[MAX], int pos)
{
  int ret=0;
  for (pos++; pos; pos -= pos & -pos)
    ret += fen[pos];
  return ret;
}

int DfsSub(int node, int prosli)
{
  int ret = 1;
  
  if (prosli != -1) {
    par[node] = prosli;
    dub[node] = dub[prosli] + 1;
  }

  for (int i=0; i<(int) V[node].size(); i++) {
    int nn = V[node][i];
    if (nn != prosli)
      ret += DfsSub(nn, node);
  }

  return sub[node] = ret;
}

void Decompose(int node, int kor)
{
  int maxx = -1, ind = -1;

  poz[node] = ukpoz++;
  root[node] = kor;

  for (int i=0; i<(int) V[node].size(); i++) {
    int nn = V[node][i];
    if (nn != par[node] && sub[nn] > maxx) {
      maxx = sub[nn];
      ind = nn;
    }
  }

  if (ind != -1)
    Decompose(ind, kor);
  slj[node] = ind;

  for (int i=0; i<(int) V[node].size(); i++) {
    int nn = V[node][i];
    if (nn != par[node] && nn != ind)
      Decompose(nn, nn);
  }
}

int Popnise(int dol, int gor) //prima poz
{
  return Vrati(fench, dol) - Vrati(fench, gor - 1) - (Vrati(fenja, dol) - Vrati(fenja, gor - 1)) + dp[gor];
}

P Lca(int aa, int bb, int st)
{
  int val = 0;

  if (aa == bb)
    return P(aa, 0);

  for (;;) {
    if (root[aa] == root[bb]) {
      if (dub[aa] > dub[bb])
        swap(aa, bb);

      if (st)
        val += Popnise(poz[bb], poz[aa]);
      return P(aa, val);
    }

    if (dub[root[aa]] > dub[root[bb]])
      swap(aa, bb);

    if (st)
      val += Popnise(poz[bb], poz[root[bb]]) - dp[poz[root[bb]]];
    bb = par[root[bb]];
  }
}

int main()
{
  int n, q;

  scanf("%d", &n);

  for (int i=0; i<n-1; i++) {
    int aa, bb;
    scanf("%d%d", &aa, &bb); aa--; bb--;
    V[aa].push_back(bb);
    V[bb].push_back(aa);
  }

  DfsSub(0, -1);
  Decompose(0, 0);
  scanf("%d", &q);

  for (int i=0; i<q; i++) {
    scanf("%d%d%d", &a[i], &b[i], &w[i]); a[i]--; b[i]--;
    lc[i] = Lca(a[i], b[i], 0).X;
    Q[lc[i]].push_back(i);
  }

  for (int i=0; i<n; i++)
    sorted[i] = P(dub[i], i);

  sort(sorted, sorted + n, greater<P>());

  for (int i=0; i<n; i++) {
    int tmp = sorted[i].Y;    

    int nov=0;
    for (int j=0; j<(int) V[tmp].size(); j++)
      if (V[tmp][j] != par[tmp])
        nov += dp[poz[V[tmp][j]]];

    for (int j=0; j<(int) Q[tmp].size(); j++) {
      int ind = Q[tmp][j];
      nov = max(nov, w[ind] + Lca(a[ind], b[ind], 1).Y);
    }

    dp[poz[tmp]] = nov;
    Stavi(fenja, poz[tmp], nov);
    if (tmp)
      Stavi(fench, poz[par[tmp]], nov);
  }

  printf("%d\n", dp[0]);

  return 0;
}
