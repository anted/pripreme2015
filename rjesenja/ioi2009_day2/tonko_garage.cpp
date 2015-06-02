#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <iostream>
#include <algorithm>
#include <vector>
#include <set>
#include <queue>
using namespace std;

typedef long long llint;

const int MAXN = 10000;

int N, M;
set<int> S;
queue<int> Q;
int A[MAXN], B[MAXN];
int park[MAXN];

void load() {
  scanf("%d%d", &N, &M);
  for (int i = 0; i < N; ++i) scanf("%d", A+i);
  for (int i = 0; i < M; ++i) scanf("%d", B+i);
}

void solve() {
  llint ans = 0;
  for (int i = 0; i < N; ++i) S.insert(i);
  for (int i = 0; i < 2*M; ++i) {
    int x; scanf("%d", &x);
    if (x > 0) {
      --x;
      Q.push(x);
    }
    else {
      x = -x;
      --x;
      S.insert(park[x]);
    }
    while (!S.empty() && !Q.empty()) {
      int x = Q.front();
      Q.pop();
      set<int>::iterator it = S.begin();
      park[x] = *it;
      ans += (llint)A[*it] * B[x];
      S.erase(it);
    }
  }
  printf("%lld\n", ans);
}

int main(void) {
  load(); 
  solve();
  return 0;
}

