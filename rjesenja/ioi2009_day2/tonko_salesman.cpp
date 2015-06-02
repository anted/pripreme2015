#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <iostream>
#include <algorithm>
#include <vector>
using namespace std;

typedef long long llint;

const int MAXN = 500100;
const int inf = 2e9;

int N, U, D, S;

vector<pair<int, int>> list[MAXN];

struct tournament {
  static const int off = 1 << 19;
  llint u[2*off];
  llint d[2*off];
  llint best[2*off];

  int from, to;

  tournament() {
    for (int i = 0; i < 2*off; ++i) {
      u[i] = -inf;
      d[i] = -inf;
    }
  }

  void set(int pos, int val) {
    llint v1 = val - U*pos;
    llint v2 = val + D*pos;
    pos += off;
    u[pos] = v1;
    d[pos] = v2;
    for (pos /= 2; pos > 0; pos /= 2) {
      u[pos] = max(u[pos+pos], u[pos+pos+1]);
      d[pos] = max(d[pos+pos], d[pos+pos+1]);
    }
  }

  llint query_d(int pos, int lo, int hi) {
    if (lo >= from && hi <= to) return d[pos];
    if (lo >= to || hi <= from) return -inf;
    int mid = (lo + hi) / 2;
    return max(
      query_d(pos+pos+0, lo, mid),
      query_d(pos+pos+1, mid, hi)
    );
  }

  llint query_u(int pos, int lo, int hi) {
    if (lo >= from && hi <= to) return u[pos];
    if (lo >= to || hi <= from) return -inf;
    int mid = (lo + hi) / 2;
    return max(
      query_u(pos+pos+0, lo, mid),
      query_u(pos+pos+1, mid, hi)
    );
  }

  llint opt(int pos) {
    llint ret;
    from = 0, to = pos;
    ret = -D*pos + query_d(1, 0, off);
    from = pos, to = off-100;
    ret = max(ret, U*pos + query_u(1, 0, off));
    return ret;
  }

} T;

void load() {
  scanf("%d%d%d%d", &N, &U, &D, &S);
  for (int i = 0; i < N; ++i) {
    int a, b, c;
    scanf("%d%d%d", &a, &b, &c);
    list[a].push_back({b, c});
  }
}

void solve() {
  static llint best[MAXN];
  memset(best, 0x80, sizeof best);

  T.set(S, 0);
  llint ans = 0;
  for (int i = 0; i <= 500001; ++i) {
    if (list[i].empty()) continue;
    sort(list[i].begin(), list[i].end());
    for (auto item: list[i]) {
      llint calc = item.second + T.opt(item.first);
      best[item.first] = max(best[item.first], calc);
      if (item.first < S) ans = max(ans, calc - D*(S-item.first));
      else ans = max(ans, calc - U*(item.first - S));
      T.set(item.first, calc);
    }
    for (auto item: list[i]) {
      T.set(item.first, -inf);
    }
    reverse(list[i].begin(), list[i].end());
    for (auto item: list[i]) {
      llint calc = item.second + T.opt(item.first);
      best[item.first] = max(best[item.first], calc);
      if (item.first < S) ans = max(ans, calc - D*(S - item.first));
      else ans = max(ans, calc - U*(item.first - S));
      T.set(item.first, calc);
    }
    for (auto item: list[i]) {
      T.set(item.first, best[item.first]);
    }
  }
  printf("%lld\n", ans);
}

int main(void) {
  load();
  solve();
  return 0;
}

