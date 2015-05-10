#include <cstdio>
#include <iostream>
#include <vector>
#include <algorithm>
#include <set>

#define TRACE(x) cerr << #x << " = " << x << endl
#define _ << " _ " <<

using namespace std;
typedef long long L;

const int MAX = 100000;

int k;
int n = 0;
L a[MAX];
L b[MAX];
L total = 0;

L cntmed(const vector<L> &x) {
  if ((int)x.size() == 0)
    return 0;
  L med = x[(int)x.size()/2];
  L cnt = 0;
  //TRACE(med);
  for (auto y : x)
    cnt += abs(med-y);
  return cnt;
}

L solve1() {
  vector<L> x;
  for (int i=0; i<n; i++) {
    x.push_back(a[i]);
    x.push_back(b[i]);
  }
  sort(x.begin(), x.end());
  return n+cntmed(x);
}

typedef pair<int, int> P;

struct cmp {
  bool operator() (const P& a, const P& b) {
    return a.first+a.second < b.first+b.second;
  }
};

struct medset {
  multiset<L> s;
  L t;
  multiset<L>::iterator m;
  medset() {
    t = 0;
    m = s.begin();
  };
  L cnt() {
    if (s.empty())
      return 0;
    return t;
  }
  void add(L x) {
    L med = *m;
    int size = (int)s.size();
    if (size == 0) {
      s.insert(x);
      m = s.begin();
      t = 0;
      return ;
    }
    int left = (size+1)/2;
    int right = size-left;
    if (x >= med) {
      t += abs(med-x);
      s.insert(x);
      right++;
      if (right > left) {
        ++m;
        L move = *m-med;
        t += left*move;
        t -= right*move;
      }
    } else {
      t += abs(med-x);
      s.insert(x);
      left++;
      if (left > right+1) {
        --m;
        L move = med-*m;
        t += (right+1)*move;
        t -= (left-1)*move;
      }
    }
  }
};

L solve2() {
  vector<P> x(n);
  for (int i=0; i<n; i++) 
    x[i] = {a[i], b[i]};
  sort(x.begin(), x.end(), cmp());
  L sol = 1LL<<60;
  for (int i=0; i<=n; i++) {
    vector<L> left;
    vector<L> right;
    for (int j=0; j<n; j++) 
      if (j < i)
        left.push_back(x[j].first), left.push_back(x[j].second);
      else
        right.push_back(x[j].first), right.push_back(x[j].second);
    sort(left.begin(), left.end());
    sort(right.begin(), right.end());
    sol = min(sol, n+cntmed(left)+cntmed(right));
  }
  return sol;
}

L solve3() {
  vector<P> x(n);
  for (int i=0; i<n; i++) 
    x[i] = {a[i], b[i]};
  sort(x.begin(), x.end(), cmp());
  L sol = 1LL<<60;
  medset s;
  vector<L> l(n+1, 0);
  for (int i=1; i<=n; i++) {
    s.add(x[i-1].first);
    s.add(x[i-1].second);
    l[i] = s.cnt();
  }
  vector<L> R(n+1, 0);
  s = medset();
  for (int i=n-1; i>=0; i--) {
    s.add(x[i].first);
    s.add(x[i].second);
    R[i] = s.cnt();
  }
  for (int i=0; i<=n; i++) 
    sol = min(sol, n+l[i]+R[i]);
  return sol;
}

int main() {
  vector<int> T = {4,3, 5, 5, 6,45,3, 1, 2,3 ,45};
  medset M;
  //TRACE(M.cnt());
  for (auto x : T) {
    M.add(x);
    //TRACE(x _ *M.m _ M.cnt());
  }

  int m;
  scanf("%d%d", &k, &m);
  for (int i=0; i<m; i++) {
    char p, q;
    int s, t;
    scanf(" %c%d %c%d", &p, &s, &q, &t);
    if (p == q) {
      total += abs(s-t);
      continue ;
    }
    a[n] = min(s, t);
    b[n] = max(s, t);
    n++;
  }

  //for (int i=0; i<n; i++) 
  //  TRACE(a[i] _ b[i]);
  
  if (k == 1)
    printf("%lld\n", total+solve1());
  else
    printf("%lld\n", total+solve3());
  return 0;
}
