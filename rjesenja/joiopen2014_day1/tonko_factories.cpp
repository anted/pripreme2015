#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <algorithm>
#include <vector>
#include <stack>
using namespace std;

#include "factories.h"

typedef long long llint;

const int MAXN = 500010;
const int LOGMAXN = 20;
const llint inf = 2000000000LL * 1000000000;

int N;
vector<pair<int, int>> V[MAXN];

int dad[MAXN][LOGMAXN];
int dep[MAXN];
int dtm[MAXN];
int ftm[MAXN];
llint sum[MAXN];

llint dp[MAXN];
bool bio[MAXN];

int col[MAXN];

/// BEGIN LCA
void build_lca(int curr, int prev, int s) {
  static int disc_time = 0;
  dtm[curr] = disc_time; ++disc_time;
  dep[curr] = dep[prev] + 1;
  sum[curr] = sum[prev] + s;
  dad[curr][0] = prev;
  for (int i = 1; i < 19; ++i)
    dad[curr][i] = dad[dad[curr][i-1]][i-1];
  for (auto nxt: V[curr]) {
    if (nxt.first == prev) continue;
    build_lca(nxt.first, curr, nxt.second);
  }
  ftm[curr] = disc_time;
}

int lca(int u, int v) {
  if (dep[u] < dep[v]) swap(u, v);
  for (int i = 18; i >= 0; --i)
    if (dep[u] - (1 << i) >= dep[v])
      u = dad[u][i];
  if (u == v) return u;
  for (int i = 18; i >= 0; --i) {
    if (dad[u][i] == dad[v][i]) continue;
    u = dad[u][i];
    v = dad[v][i];
  }
  return dad[u][0];
}

inline llint dist(int u, int v) {
  return sum[u] + sum[v] - 2*sum[lca(u, v)];
}
/// END LCA



void Init(int n, int a[], int b[], int d[]) {
  N = n;
  for (int i = 0; i < N-1; ++i) {
    V[a[i]].push_back({b[i], d[i]});
    V[b[i]].push_back({a[i], d[i]});
  }
  dep[0] = -1;
  build_lca(0, 0, 0);
}

llint Query(int S, int X[], int T, int Y[]) {
  static vector<int> vec;

  llint ans = inf;
 
  for (int i = 0; i < S; ++i) vec.push_back(X[i]);
  for (int i = 0; i < T; ++i) vec.push_back(Y[i]);
  const int n = (int)vec.size();

  for (int i = 0; i < S; ++i) col[X[i]] = 1;
  for (int i = 0; i < T; ++i) col[Y[i]] = 2;

  sort(vec.begin(), vec.end(), [](int a, int b) {
    return dtm[a] < dtm[b];
  });

  static pair<llint, llint> val[MAXN];

  auto below = [](int u, int v) {
    return dtm[u] > dtm[v] && ftm[u] <= ftm[v];
  };

  auto func = [&](int u, int v) {
    // u mi je iznad v
    val[u].first  = min(val[u].first,  val[v].first  + sum[v]-sum[u]);
    val[u].second = min(val[u].second, val[v].second + sum[v]-sum[u]);
    ans = min(ans, val[u].first + val[u].second);
  };

  stack<int> st;

  val[N] = {inf, inf};
  sum[N] = inf;
  for (int i = 0; i < n; ++i) {
    int x = vec[i];
    val[x] = {inf, inf};
    if (col[x] == 1) val[x].first = 0;
    if (col[x] == 2) val[x].second = 0;
    int last = N;
    while (!st.empty()) {
      int y = st.top();
      func(y, last);
      last = y;
      if (below(x, y)) {
        func(y, x);
        break; 
      }
      int LCA = lca(x, y);
      st.pop();
      if (st.empty() || below(LCA, st.top())) {
        val[LCA] = {inf, inf};
        func(LCA, y);
        st.push(LCA);
      }
    }
    st.push(x);
  }
  
  int last = N;
  while (!st.empty()) {
    func(st.top(), last);
    last = st.top();
    st.pop();
  }

  // cleaning
  for (int it: vec) {
    col[it] = 0;
    val[it] = {inf, inf};
  }
  vec.clear();
  
  return ans;
}

