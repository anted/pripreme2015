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

typedef tuple<int, int, int> State;

const int MAXN = 30010;
const int SQRT = 180;
const int inf = 0x3f3f3f3f;

int N, M;
int A[MAXN], B[MAXN];

int dist[MAXN][SQRT+10];

vector<State> V[MAXN][SQRT+10];
vector<int> vec[MAXN];

void load() {
  scanf("%d%d", &N, &M);
  for (int i = 0; i < M; ++i) {
    scanf("%d%d", &A[i], &B[i]);
    vec[A[i]].push_back(B[i]);
  }
}

void solve() {
  static bool bio[MAXN][SQRT+10];
  memset(bio, 0, sizeof bio);
  memset(dist, inf, sizeof dist);
  dist[A[0]][0] = 0;
  priority_queue<State, vector<State>, greater<State>> pq;
  pq.push(State(0, A[0], 0));
  
  while (!pq.empty()) {
    int d, x, y;
    tie(d, x, y) = pq.top(); pq.pop();
    if (bio[x][y]) continue;
    bio[x][y] = 1;
   
    auto func = [&](int nx, int ny, int val) {
      if (dist[x][y] + val < dist[nx][ny]) {
        dist[nx][ny] = dist[x][y] + val;
        if (!bio[nx][ny])
          pq.push(State(dist[nx][ny], nx, ny));
      }
    };

    if (y == 0) {
      for (int it: vec[x]) {
        if (it < SQRT) func(x, it, 0);
        if (it >= SQRT) {
          for (int j = 1; j < SQRT; ++j) {
            if (x + j*it >= N) break;
            func(x + j*it, 0, j);
          }
          for (int j = 1; j < SQRT; ++j) {
            if (x - j*it < 0) break;
            func(x - j*it, 0, j);
          }
        }
      }
    }
    else {
      if (x+y < N) {
        func(x+y, y, 1);
        func(x+y, 0, 1);
      }
      if (x-y >= 0) {
        func(x-y, y, 1);
        func(x-y, 0, 1);
      }
    } 
  }

  int ans = inf;
  for (int i = 0; i < SQRT; ++i) {
    ans = min(ans, dist[A[1]][i]);
  }
  if (ans == inf) printf("-1\n");
  else printf("%d\n", ans);
}

int main(void) {
  load();
  solve();
  return 0;
}

