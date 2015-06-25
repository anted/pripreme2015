#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <iostream>
#include <algorithm>
#include <vector>
using namespace std;

typedef long long llint;

const int MAXN = 100010;
const int LOG = 18;

int N, M;

pair<int, int> A[MAXN];
int C[MAXN];

vector<int> V[MAXN];
vector<int> paths[MAXN];

int dad[MAXN][LOG];
int dep[MAXN];

int dp[MAXN];
llint sum[MAXN];

int hlcol[MAXN];
int hlcnt;

int sz[MAXN]; // velicina podstabla
int hldad[MAXN]; // koji cvor je otac HL
int hlpos[MAXN];  // pozicija u HL

struct fenwick {
  vector<llint> data;
  int n;
  fenwick() {
    data.push_back(0);
    data.push_back(0);
    n = 2;
  }
  void inc() {
    data.push_back(0);
    ++n;
  }

  void update(int p, int val) {
    for (++p; p < n; p += p & -p)
      data[p] += val;
  }

  llint query(int p) {
    llint ret = 0;
    for (++p; p > 0; p -= p & -p)
      ret += data[p];
    return ret;
  }
};

fenwick F1[MAXN];
fenwick F2[MAXN];

void build_lca(int curr, int prev) {
  if (curr != 0) dep[curr] = dep[prev] + 1;
  dad[curr][0] = prev;
  sz[curr] = 1;
  for (int i = 1; i <= 16; ++i)
    dad[curr][i] = dad[dad[curr][i-1]][i-1];
  for (int nxt: V[curr]) {
    if (nxt == prev) continue;
    build_lca(nxt, curr);
    sz[curr] += sz[nxt];
  }
}

void build_hl(int curr, int prev, int col, int cpos = 0) {
  hlcol[curr] = col;
  hlpos[curr] = cpos;
  F1[col].inc();
  F2[col].inc();

  int best = -1;
  int mx = 0;
  for (int nxt: V[curr]) {
    if (prev == nxt) continue;
    if (sz[nxt] > mx) {
      mx = sz[nxt];
      best = nxt;
    } 
  }
  if (best != -1)
    build_hl(best, curr, col, cpos+1);
  for (int nxt: V[curr]) {
    if (prev == nxt) continue;
    if (nxt == best) continue;
    hldad[hlcnt] = curr;
    build_hl(nxt, curr, hlcnt++);
  }

}

int lca(int u, int v) {
  if (dep[u] < dep[v]) swap(u, v);
  // u je veci
  for (int i = 16; i >= 0; --i)
    if (dep[u] - (1 << i) >= dep[v])
      u = dad[u][i];
  for (int i = 16; i >= 0; --i) {
    if (dad[u][i] == dad[v][i]) continue;
    u = dad[u][i];
    v = dad[v][i];
  }
  if (u == v) return u;
  return dad[u][0];
}

inline llint get(fenwick *F, int from, int to) {
  if (dep[to] < dep[from]) {  return 0; }
  llint ret = 0;
  for (int p = to; ; p = hldad[hlcol[p]]) {
    if (hlcol[p] != hlcol[from]) ret += F[hlcol[p]].query(hlpos[p]);
    else {
      ret += F[hlcol[p]].query(hlpos[p]) - F[hlcol[p]].query(hlpos[from] - 1);
      break;
    }
  }
  return ret;
}

void dfs(int x, int prev = -1) {
  int ret = 0;
  // trivijalan slucaj u slucaju da preskacem sebe
  for (int y: V[x]) {
    if (y == prev) continue;
    dfs(y, x);
    ret += dp[y];
  }

  // ima ceste preko mene
  for (int i: paths[x]) {
    int u = A[i].first;
    int v = A[i].second;
    llint tmp = C[i];
    tmp += get(F1, x, u);
    tmp -= get(F2, x, u);
    tmp += get(F1, x, v);
    tmp -= get(F2, x, v);
    tmp -= F1[hlcol[x]].query(hlpos[x]) - F1[hlcol[x]].query(hlpos[x] - 1);
    ret = max(ret, (int)tmp);
  }

  // recimo da sam izracunao ret
  // F1 mi je za parente
  int par = dad[x][0];
  F1[hlcol[par]].update(hlpos[par], ret);
  // F2 sam ja
  F2[hlcol[x]].update(hlpos[x], ret);
  // postavim sebe
  dp[x] = ret;
}

void solve() {
  hldad[0] = -1;
  build_hl(0, 0, hlcnt++);
  dfs(0);
  printf("%d\n", dp[0]);
}

void load() {
  scanf("%d", &N);
  for (int i = 0; i < N-1; ++i) {
    int u, v; scanf("%d%d", &u, &v);
    --u; --v;
    V[u].push_back(v);
    V[v].push_back(u);
  }
  build_lca(0, 0);
  scanf("%d", &M);
  for (int i = 0; i < M; ++i) {
    int u, v; scanf("%d%d%d", &u, &v, &C[i]);
    --u; --v;
    A[i] = {u, v};
    int LCA = lca(u, v);
    paths[LCA].push_back(i);
  }
}

int main(void) {
  load();
  solve();
  return 0;
}

