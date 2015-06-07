#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>
using namespace std;

typedef long long llint;

const int MAXN = 500010;

int N, K;

int dir[MAXN];
int bio[MAXN];
int covered[MAXN];
vector<int> covered_root[MAXN];

int comp_cnt;
int comp[MAXN];
int flag[MAXN];

vector<int> cyc;
vector<int> vec;

vector<int> V[MAXN];
vector<int> roots[MAXN];

int sweep[2*MAXN];

int ans = 0;

int dfs_1(int x) {
  if (bio[x]  > 1) return comp[x];
  if (bio[x] == 1) {
    cyc.push_back(x);
    for (int y = dir[x]; y != x; y = dir[y])
      cyc.push_back(y);
    roots[comp_cnt] = cyc;
    return comp[x] = comp_cnt++;
  }
  vec.push_back(x);
  bio[x] = 1;
  return comp[x] = dfs_1(dir[x]);
}

void find_components() {
  memset(bio, 0, sizeof bio);
  for (int i = 0; i < N; ++i) {
    if (bio[i]) continue;
    dfs_1(i);
    for (int x: vec) bio[x] = 2;
    for (int x: cyc) bio[x] = 3;
    cyc.clear();
    vec.clear();
  }
  for (int i = 0; i < N; ++i) {
    if (bio[i] == 3) continue;
    V[dir[i]].push_back(i);
  }
}

void init_covering() {
  int x = 0;
  for (int i = 0; i <= K; ++i) {
    covered[x] = 1;
    x = dir[x];
  }
}

int dfs_2(int x) {
  int mx = 0;
  for (int y: V[x]) mx = max(mx, dfs_2(y));
  if (covered[x]) return mx - 1;
  if (mx <= 0) {
    flag[x] = 1;
    return K-1;
  }
  return mx - 1;
}

void tree_covering() {
  for (int i = 0; i < comp_cnt; ++i) {
    for (int x: roots[i]) {
      dfs_2(x);
      flag[x] = 0;
    }
  }
  
  static int dist[MAXN];
  static queue<int> q;
  memset(dist, 0x3f, sizeof dist);
  
  for (int i = 0; i < N; ++i)
    if (flag[i]) {
      q.push(i);
      dist[i] = 0;
    }

  ans = (int)q.size();

  for (; !q.empty(); q.pop()) {
    int x = q.front();
    if (dist[dir[x]] < MAXN) continue;
    dist[dir[x]] = dist[x] + 1;
    q.push(dir[x]);
  }
  
  for (int i = 0; i < N; ++i) {
    if (dist[i] < K)
      covered[i] = 1;
  }
}

void cycle_covering(int cyc) {
  static int nxt[2*MAXN];
  int n = (int)roots[cyc].size();
  nxt[2*n] = 0;
  for (int i = 2*n-1; i >= 0; --i) {
    if (covered[roots[cyc][i%n]]) nxt[i] = nxt[i+1] + 1;
    else nxt[i] = 0;
  }
  int best = 10000000;
  for (int i = 0; i <= min(n, K); ++i) {
    int cnt = 0;
    for (int j = i+nxt[i]; j < i+n; j += nxt[j+K] + K)
      ++cnt;
    best = min(best, cnt);
  }
  ans += best;
}

void solve() {
  find_components(); 
  init_covering();
  tree_covering();
  for (int i = 0; i < comp_cnt; ++i) {
    cycle_covering(i);
  }
  printf("%d\n", ans);
}

void load() {
  scanf("%d%d", &N, &K);
  for (int i = 0; i < N; ++i) {
    int u, v;
    scanf("%d%d", &u, &v);
    --u; --v;
    dir[u] = v;
  }
}


int main(void) {
  load();
  solve();
  return 0;
}

