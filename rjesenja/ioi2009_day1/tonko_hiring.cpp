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

int N;
llint W;
pair<int, int> A[MAXN];
vector<pair<double, int>> vec;

int idx[MAXN];

llint data[MAXN];

struct fenwick {
  llint data[MAXN];
  fenwick() {
    memset(data, 0, sizeof data);
  }

  void update(int pos, int val) {
    for (; pos < MAXN; pos += pos & -pos)
      data[pos] += val;
  }

  llint query(int pos) {
    llint ret = 0;
    for (; pos > 0; pos -= pos & -pos)
      ret += data[pos];
    return ret;
  }
} F1, F2;


void load() {
  scanf("%d%lld", &N, &W);
  for (int i = 0; i < N; ++i)
    scanf("%d%d", &A[i].first, &A[i].second);
}

void compress() {
  static vector<pair<int, int>> tmp;
  for (int i = 0; i < N; ++i)
    tmp.push_back({A[i].second, i});
  sort(tmp.begin(), tmp.end());
  for (int i = 0; i < N; ++i)
    idx[tmp[i].second] = i;
  for (int i = 0; i < N; ++i)
    vec.push_back({(double)A[i].first / A[i].second, i});
  sort(vec.begin(), vec.end());
}

int get(llint sum) {
  int lo = 0, hi = N;
  while (lo < hi) {
    int mid = (lo + hi + 1) >> 1;
    if (F1.query(mid) <= sum) lo = mid;
    else hi = mid-1;
  }
  return lo;
}

void solve() {
  compress();
  int wh, ans = -1;
  double spend = 1e9 * 200.0;
  for (int i = 0; i < N; ++i) {
    int pos = idx[vec[i].second];
    double k = vec[i].first;
    F1.update(pos+1, A[vec[i].second].second);
    F2.update(pos+1, 1);
    int t = get((llint)(W / k));
    int cnt = F2.query(t);
    double kol = k * F1.query(t);

    if (cnt == ans && kol < spend) {
      spend = kol;
      wh = i; 
    } 
    if (cnt > ans) {
      ans = cnt;
      spend = kol;
      wh = i;
    }
  }
  static vector<pair<int, int>> tmp;
  for (int i = 0; i <= wh; ++i)
    tmp.push_back({A[vec[i].second].second, vec[i].second});
  sort(tmp.begin(), tmp.end());
  printf("%d\n", ans);
  for (int i = 0; i < ans; ++i)
    printf("%d\n", 1+tmp[i].second);
}

int main(void) {
  load();
  solve();
  return 0;
}

