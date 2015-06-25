#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <iostream>
#include <algorithm>
#include <vector>
using namespace std;

typedef long long llint;

const int MAXN = 100100;

int N, K, Q;
int init[MAXN];

struct tournament {
  static const int off = 1 << 17;
  static const int LOG = 32;

  int from, to, val;
  llint data[2*off][LOG];
  int flag[2*off];

  void merge(int node) {
    if (node >= off) return;
    for (int i = 0; i < LOG; ++i)
      data[node][i] = data[2*node][i] + data[2*node+1][i];
  }

  void propagate(int node) {
    if (K == 1) return;
    if (node < off) {
      flag[2*node+0] += flag[node];
      flag[2*node+1] += flag[node];
    }
    int k = flag[node];
    flag[node] = 0;
    if (k >= LOG) {
      for (int i = 0; i < LOG; ++i)
        data[node][i] = 0;
      return;
    }
    // inace ako mogu napraviti promjenu
    for (int i = k; i < LOG; ++i)
      data[node][i-k] = data[node][i];
    for (int i = LOG-k; i < LOG; ++i)
      data[node][i] = 0;
  }

  void _set(int node, int lo, int hi) {
    propagate(node);
    if (lo >= to || hi <= from) return;
    if (lo >= from && hi <= to) {
      data[node][0] = val;
      for (int i = 1; i < LOG; ++i)
        data[node][i] = data[node][i-1] / K;
      return;
    }
    int mid = (lo + hi) / 2;
    _set(2*node+0, lo, mid);
    _set(2*node+1, mid, hi);
    merge(node);
  }

  void set(int p, int v) {
    from = p, to = p+1, val = v;
    _set(1, 0, off);
  }

  void _affect(int node, int lo, int hi) {
    propagate(node);
    if (lo >= to || hi <= from) return;
    if (lo >= from && hi <= to) {
      ++flag[node];
      propagate(node);
      return;
    }
    int mid = (lo + hi) / 2;
    _affect(2*node+0, lo, mid);
    _affect(2*node+1, mid, hi);
    merge(node);
  }

  void affect(int l, int r) {
    from = l, to = r+1;
    _affect(1, 0, off);
  }

  llint _query(int node, int lo, int hi) {
    propagate(node);
    if (lo >= to || hi <= from) return 0;
    if (lo >= from && hi <= to) return data[node][0];
    int mid = (lo + hi) / 2;
    return _query(2*node, lo, mid) + _query(2*node+1, mid, hi);
  }

  llint query(int l, int r) {
    from = l, to = r+1;
    return _query(1, 0, off);
  }

  void build() {
    memset(data, 0, sizeof data);
    memset(flag, 0, sizeof flag);
    for (int i = 0; i < N; ++i) {
      data[i+off][0] = init[i];
      for (int j = 1; j < LOG; ++j)
        data[i+off][j] = data[i+off][j-1] / K;
    }
    for (int i = off-1; i > 0; --i)
      for (int j = 0; j < LOG; ++j)
        data[i][j] = data[2*i][j] + data[2*i+1][j];
  }

} T;

int main(void) {
  
  scanf("%d%d%d", &N, &Q, &K);
  for (int i = 0; i < N; ++i)
    scanf("%d", &init[i]);

  T.build();

  while (Q--) {
    int cmd, a, b;
    scanf("%d%d%d", &cmd, &a, &b);
    if (cmd == 1) {
      T.set(a-1, b);
    }
    if (cmd == 2) {
      T.affect(a-1, b-1);
    }
    if (cmd == 3) {
      printf("%lld\n", T.query(a-1, b-1));
    }
  }

  return 0;
}

