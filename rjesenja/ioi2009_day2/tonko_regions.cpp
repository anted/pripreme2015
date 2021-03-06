#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <iostream>
#include <algorithm>
#include <vector>
#include <map>
using namespace std;

typedef long long llint;

const int MAXN = 200100;
const int SQRT = 500;

int N, R, Q;
int col[MAXN];
int par[MAXN];

int lo[MAXN];
int hi[MAXN];

vector<int> V[MAXN];
vector<int> reg[MAXN];
vector<int> REG[MAXN];

vector<int> v;
int ptr[MAXN];


vector<int> ttt[MAXN];

int ANS[510][25100];


void load() {
  scanf("%d%d%d", &N, &R, &Q);
  scanf("%d", &col[0]);
  for (int i = 1; i < N; ++i) {
    scanf("%d%d", &par[i], &col[i]);
    --par[i];
    V[par[i]].push_back(i);
  }
  for (int i = 0; i < N; ++i) {
    reg[--col[i]].push_back(i);
  }
}

void dfs(int curr) {
  static int tm = 1;
  lo[curr] = tm++;
  for (int nxt: V[curr]) dfs(nxt);
  hi[curr] = tm++;
}

void init() {
  dfs(0); 
  for (int i = 0; i < R; ++i) {
    for (int it: reg[i])
      REG[i].push_back(lo[it]);
    sort(REG[i].begin(), REG[i].end());
  }
}

int CNT[MAXN];

void dfs_2(int curr) {
  static const int n = (int)v.size();
  
  bool flag = (reg[col[curr]].size() > SQRT);
  for (int i = 0; i < n; ++i)
    ANS[i][col[curr]] += CNT[i];
 
  if (flag) ++CNT[ptr[col[curr]]];
  for (int nxt: V[curr]) dfs_2(nxt);
  if (flag) --CNT[ptr[col[curr]]];
}


int solve_1(int r1, int r2) {
  int ans = 0;
  for (int it: reg[r1]) {
    ans += upper_bound(REG[r2].begin(), REG[r2].end(), hi[it])
         - lower_bound(REG[r2].begin(), REG[r2].end(), lo[it]); 
  }
  printf("%d\n", ans);
}

int solve_2(int r1, int r2) {
  printf("%d\n", ANS[ptr[r1]][r2]);
}


void solve() {
  for (int i = 0; i < R; ++i) {
    if (reg[i].size() > SQRT) {
      ptr[i] = (int)v.size();
      v.push_back(i);
    }
  }
  dfs_2(0);


  for (int i = 0; i < Q; ++i) {
    int r1, r2;
    scanf("%d%d", &r1, &r2);
    --r1; --r2;
    if (reg[r1].size() <= SQRT) {
      solve_1(r1, r2);
    }
    else  {
      solve_2(r1, r2);
    }
  }
}

int main(void) {
  load();
  init();
  solve();
  return 0;
}
