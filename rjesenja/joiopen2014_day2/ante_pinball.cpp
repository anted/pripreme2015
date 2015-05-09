#include <iostream>
#include <algorithm>
#include <vector>
#include <map>

#define TRACE(x) cerr << #x << " = " << x << endl
#define _ << " _ " <<

using namespace std;

typedef long long llint;

const int MAXM = 100000;
const int OFF = 1<<19;
const int TSIZE = 1<<20;
const llint INF = 1LL<<59;

int m;
int n;
int a[MAXM];
int b[MAXM];
int c[MAXM];
llint d[MAXM];

void compress() {
  static int x[3*MAXM+2];
  int k = 0;
  x[k++] = 1;
  for (int i=0; i<m; i++) {
    x[k++] = a[i];
    x[k++] = b[i];
    x[k++] = c[i];
  }
  x[k++] = n;
  sort(x, x+k);
  k = unique(x, x+k)-x;
  for (int i=0; i<m; i++) {
    a[i] = lower_bound(x, x+k, a[i])-x;
    b[i] = lower_bound(x, x+k, b[i])-x;
    c[i] = lower_bound(x, x+k, c[i])-x;
  }
  n = lower_bound(x, x+k, n)-x;
}

struct brute {
  map<int, llint> M;
  void put(int x, llint v) {
    if (!M.count(x))
      M[x] = INF;
    M[x] = min(M[x], v); 
  }
  llint query(int x, int y) {
    llint s = INF;
    for (int i=x; i<=y; i++)
      if (M.count(i))
        s = min(s, M[i]);
    return s;
  }
};

struct tournament {
  llint t[TSIZE];
  tournament() {
    for (int i=0; i<TSIZE; i++)
      t[i] = INF;
  }
  void put(int x, llint v) {
    x += OFF;
    t[x] = min(t[x], v);
    for (x/=2; x>0; x/=2)
      t[x] = min(t[2*x], t[2*x+1]);
    
  }
  llint query(int x, int y) {
    int xx = x, yy = y;
    x += OFF, y += OFF;
    llint s = min(t[x], t[y]);
    for (; x/2!=y/2; x/=2, y/=2) {
      if (x%2 == 0)
        s = min(s, t[2*(x/2)+1]);
      if (y%2 == 1)
        s = min(s, t[2*(y/2)]);
    }
    return s;
  }
};

int main() {
  cin >> m >> n;
  for (int i=0; i<m; i++) 
  cin >> a[i] >> b[i] >> c[i] >> d[i];
  compress();
  static tournament t[2];
  t[0].put(0, 0);
  t[1].put(n, 0);
  llint sol = INF;
  for (int i=0; i<m; i++) {
    llint q[2] = {INF, INF};
    for (int j=0; j<2; j++) {
      q[j] = t[j].query(a[i], b[i])+d[i];
      if (q[j] < INF)
        t[j].put(c[i], q[j]);
    }
    //TRACE(i _ a[i] _ b[i] _ c[i] _ d[i] _ q[0] _ q[1]);
    sol = min(sol, q[0]+q[1]-d[i]);
  }
  if (sol >= INF)
    sol = -1;
  cout << sol << endl;
  return 0;
}
