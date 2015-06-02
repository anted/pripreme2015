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

const int MAXN = 1010;

typedef pair<int, int> point;
#define x first
#define y second

const int dx[] = {1, 0, -1, 0};
const int dy[] = {0, 1, 0, -1};

char grid[MAXN][MAXN];

int N, K;
point S, E;
vector<point> H;

void load() {
  scanf("%d%d", &N, &K);
  for (int i = 0; i < N; ++i) {
    scanf("%s", grid[i]);
    for (int j = 0; j < N; ++j) {
      if (grid[i][j] == 'D') E = {i, j};
      if (grid[i][j] == 'M') S = {i, j};
      if (grid[i][j] == 'H') H.push_back({i, j});
    }
  }
}

int bees[MAXN][MAXN];

void bees_bfs() {
  queue<point> q;
  memset(bees, -1, sizeof bees);
  for (point pt: H) {
    q.push(pt);
    bees[pt.x][pt.y] = 0;
  }
  for (; !q.empty(); q.pop()) {
    point pt = q.front();
    for (int i = 0; i < 4; ++i) {
      int nx = pt.x + dx[i];
      int ny = pt.y + dy[i];
      if (nx < 0 || ny < 0 || nx >= N || ny >= N) continue;
      if (grid[nx][ny] == 'D' || grid[nx][ny] == 'T') continue;
      if (bees[nx][ny] != -1) continue;
      bees[nx][ny] = bees[pt.x][pt.y] + K;
      q.push({nx, ny});
    }
  }
  bees[E.x][E.y] = 2e9;
}

bool check(int k) {
  static int dp[MAXN][MAXN];
  if ((llint)k * K >= bees[S.x][S.y]) return false;  
  memset(dp, -1, sizeof dp);
  dp[S.x][S.y] = k*K;
  static queue<point> q;
  for (q.push(S); !q.empty(); q.pop()) {
    point pt = q.front();
    for (int i = 0; i < 4; ++i) {
      int nx = pt.x + dx[i];
      int ny = pt.y + dy[i];
      if (nx < 0 || ny < 0 || nx >= N || ny >= N) continue;
      if (grid[nx][ny] == 'H' || grid[nx][ny] == 'T') continue;
      if (dp[nx][ny] != -1) continue;
      if (bees[nx][ny] <= dp[pt.x][pt.y] + 1) continue;
      dp[nx][ny] = dp[pt.x][pt.y] + 1;
      q.push({nx, ny});
    }
  }
  return (dp[E.x][E.y] != -1);
}

void solve() {
  bees_bfs();
  int lo = 0, hi = 1e9;
  while (lo < hi) {
    int mid = (lo + hi + 1) / 2;
    if (check(mid)) lo = mid;
    else hi = mid - 1;
  }

  if (!check(lo)) printf("-1\n");
  else printf("%d\n", lo);
}

int main(void) {
  load();
  solve();
  return 0;
}

