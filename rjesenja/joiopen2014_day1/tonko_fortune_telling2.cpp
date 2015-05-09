#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <iostream>
#include <algorithm>
#include <vector>
using namespace std;

#define ALL(x) x.begin(), x.end()

typedef long long llint;

const int MAXN = 200010;

int N, M;
int A[MAXN], B[MAXN];
int C[MAXN];
int mostR[MAXN];
int inv[MAXN];

vector<int> compressed;

inline int Max(int x, int y) { return x > y ? x : y; }

struct tournament {
  static const int off = 1 << 20;
  int tree[2*off];
  int from, to;

  tournament() {
    memset(tree, -1, sizeof tree);
  }

  void update(int pos, int val) {
    tree[pos += off] = val;
    for (pos >>= 1; pos > 0; pos >>= 1) {
      tree[pos] = Max(tree[2*pos], tree[2*pos+1]);
    }
  }

  int _query(int node, int lo, int hi) {
    if (lo >= to || hi <= from) return -1;
    if (lo >= from && hi <= to) return tree[node];
    int mid = (lo + hi) / 2;
    return max(_query(2*node, lo, mid), _query(2*node+1, mid, hi));
  }

  int query(int lo, int hi) {
    from = lo, to = hi;
    return _query(1, 0, off);
  }
} T;

struct fenwick {
  int loga[3*MAXN];
  int len;
  fenwick() {
    len = 3*MAXN-1;
    memset(loga, 0, sizeof loga);
  }

  void update(int pos) {
    for (++pos; pos < len; pos += pos&-pos)
      ++loga[pos];
  }

  int query(int pos) {
    int ret = 0;
    for (++pos; pos > 0; pos -= pos&-pos)
      ret += loga[pos];
    return ret;
  }
} F;

void load() {
  scanf("%d%d", &N, &M);
  for (int i = 0; i < N; ++i)
    scanf("%d%d", &A[i], &B[i]);
  for (int i = 0; i < M; ++i)
    scanf("%d", &C[i]);
}

void compress() {
  compressed.resize(2*N + M);
  for (int i = 0; i < N; ++i) {
    compressed[2*i+0] = A[i];
    compressed[2*i+1] = B[i];
  }
  for (int i = 0; i < M; ++i) {
    compressed[2*N+i] = C[i];
  }
  sort(ALL(compressed));
  compressed.resize(unique(ALL(compressed)) - compressed.begin());
  for (int i = 0; i < N; ++i) {
    A[i] = lower_bound(ALL(compressed), A[i]) - compressed.begin();
    B[i] = lower_bound(ALL(compressed), B[i]) - compressed.begin();
  }
  for (int i = 0; i < M; ++i) {
    C[i] = lower_bound(ALL(compressed), C[i]) - compressed.begin();
  }
}

vector<int> V[MAXN];

void solve() {
  compress();
  for (int i = 0; i < M; ++i) T.update(C[i], i);
  for (int i = 0; i < N; ++i) {
    int b = Max(A[i], B[i]);
    int a = A[i]+B[i] - b;
    mostR[i] = T.query(a, b);
    if (mostR[i] != -1) V[mostR[i]].push_back(i);
    if (mostR[i] != -1) {
      A[i] = b;
      B[i] = a;
    }
  }
  int len = (int)compressed.size();
  for (int i = M-1; i >= 0; --i) {
    for (int j: V[i]) {
      int a = Max(A[j], B[j]);
      int q = F.query(len-1) - F.query(a-1);
      inv[j] = q;
    }
    F.update(C[i]);
  }
  for (int i = 0; i < N; ++i) {
    if (mostR[i] != -1) continue;
    int a = Max(A[i], B[i]);
    int q = F.query(len-1) - F.query(a-1);
    inv[i] = q;
  }

  llint ans = 0;
  for (int i = 0; i < N; ++i) {
    if (inv[i] & 1) ans += compressed[B[i]];
    else ans += compressed[A[i]];
  }
  printf("%lld\n", ans);
}

int main(void) {
  load();
  solve();
  return 0;
}

