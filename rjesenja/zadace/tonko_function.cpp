#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <iostream>
#include <algorithm>
#include <vector>
using namespace std;

typedef long long llint;

typedef pair<int, int> point;
#define x first
#define y second

const int MAXN = 100100;

int N, Q, A[MAXN];

llint ccw(point a, point b, point c) {
  return (llint)a.x*(b.y-c.y) + (llint)b.x*(c.y-a.y) + (llint)c.x*(a.y-b.y);
}

struct tournament {
  static const int off = 1 << 17;

  int from, to;
  vector<point> tree[2*off];

  void build() {
    vector<point> vec;
    for (int i = off-1; i > 0; --i) {
      vector<point> &curr = tree[i];
      vector<point> &lhs = tree[i+i+0];
      vector<point> &rhs = tree[i+i+1];
      vec.clear();
      for (auto it: lhs) vec.push_back(it);
      for (auto it: rhs) vec.push_back(it);
      sort(vec.begin(), vec.end());
      for (auto it: vec) {
        while (curr.size() >= 2 && ccw(curr[curr.size()-2], curr.back(), it) <= 0)
          curr.pop_back();
        curr.push_back(it);
      }
    } 
  }

  int calc(point a) {
    return -a.x * from + a.y;
  }

  int _query(int node, int lo, int hi) {
    if (lo >= to || hi <= from) return 1e9;
    if (lo >= from && hi <= to) {
      if (tree[node].empty()) return 1e9;
      int l = 0, r = (int)tree[node].size()-1;
      while (l < r) {
        int m = (l + r) / 2;
        if (calc(tree[node][m]) > calc(tree[node][m+1])) l = m+1;
        else r = m;
      }
      return calc(tree[node][l]);
    }
    int mid = (lo + hi) / 2;
    return min(
      _query(2*node+0, lo, mid),
      _query(2*node+1, mid, hi)
    );
  }

  int query(int lo, int hi) {
    from = lo; to = hi;
    return _query(1, 0, off);
  }

} T;

int main(void) {
  scanf("%d", &N);
  for (int i = 0; i < N; ++i)
    scanf("%d", &A[i]);

  static int sum[MAXN];
  for (int i = 0; i < N; ++i) {
    sum[i] = (i > 0) ? sum[i-1] + A[i] : A[i];
    int a = A[i];
    int b = a*i + a - sum[i];
    T.tree[tournament::off + i].push_back({a, b});  
  }

  T.build();

  scanf("%d", &Q);
  while (Q--) {
    int a, b; scanf("%d%d", &a, &b);
    printf("%d\n", sum[b-1] + T.query(b - a, b));
  }


  return 0;
}

