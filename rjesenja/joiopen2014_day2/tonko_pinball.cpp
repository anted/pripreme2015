
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
typedef pair<int, llint> State;

const int MAXN = 100100;
const int inf = 0x3f3f3f3f;
const llint infll = 1000000000LL * 10000000;

int N, M;
int L[MAXN], R[MAXN], P[MAXN];
llint A[MAXN];

vector<int> compressed;


const State infstate = {inf, infll};
struct tournament {
  static const int off = 1 << 19;
  int from, to;
  State tree[2*off];

  tournament() {
    for (int i = 0; i < 2*off; ++i) tree[i] = infstate;
  }

  void update(int pos, State val) {
    pos += off;
    tree[pos] = min(tree[pos], val);
    for (pos /= 2; pos > 0; pos /= 2)
      tree[pos] = min(tree[2*pos], tree[2*pos+1]);
  }

  State _query(int node, int lo, int hi) {
    if (lo >= to || hi <= from) return infstate;
    if (lo >= from && hi <= to) return tree[node];
    int mid = (lo + hi) / 2;
    return min(_query(2*node, lo, mid), _query(2*node+1, mid, hi));
  }

  State query(int lo, llint hi) {
    from = lo, to = hi;
    return _query(1, 0, off);
  }

} TL, TR;

void load() {
  scanf("%d%d", &N, &M);
  compressed.push_back(1);
  compressed.push_back(M);
  for (int i = 0; i < N; ++i) {
    scanf("%d%d%d%lld", &L[i], &R[i], &P[i], &A[i]);
    compressed.push_back(L[i]);
    compressed.push_back(R[i]);
    compressed.push_back(P[i]);
  }
}

void solve() {
  // compressing ...
  sort(ALL(compressed));
  compressed.resize(unique(ALL(compressed)) - compressed.begin());
  for (int i = 0; i < N; ++i) {
    L[i] = lower_bound(ALL(compressed), L[i]) - compressed.begin();
    R[i] = lower_bound(ALL(compressed), R[i]) - compressed.begin();
    P[i] = lower_bound(ALL(compressed), P[i]) - compressed.begin();
    ++R[i];
  }
  // end compressing ...
  int n = (int)compressed.size();
  llint ans = infll;
  for (int i = 0; i < N; ++i) {
    State l = TL.query(L[i], R[i]);
    State r = TR.query(L[i], R[i]);
    State la = {L[i], A[i]};
    State lb = {min(l.first, L[i]), A[i] + l.second};
    State ra = {n-R[i], A[i]};
    State rb = {min(r.first, n-R[i]), A[i] + r.second};
    l = min(la, lb);
    r = min(ra, rb);
    TL.update(P[i], l);
    TR.update(P[i], r);
    //printf("%d %lld - %d %lld\n", l.first, l.second, r.first, r.second);
    if (l.first == 0 && r.first == 0) {
      ans = min(ans, l.second + r.second - A[i]);
    }
  }
  if (ans == infll) printf("-1\n");
  else printf("%lld\n", ans);
}

int main(void) {
  load();
  solve();
  return 0;
}
