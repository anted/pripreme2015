#include <cstdio>
#include <string>
#include <fstream>
#include <iostream>
#include <list>
#include <vector>
#include <algorithm>
#include <cassert>
#include <set>

#define TRACE(x) cout << #x << " = " << x << endl
#define _ << " _ " <<

using namespace std;

typedef long long llint;
typedef pair<int, int> edge;

const int MAXN = 301;
const int MAXM = 301*301;
const int MAXK = 2001;

int n;
int m;
int k;
edge e[MAXM];
list<int> g[MAXN];
list<int> h[MAXN];
int X[MAXK];
int Y[MAXK];

void read(string filename) {
  ifstream in(filename);
  in >> n >> m;
  for (int i=0; i<m; i++) {
    int a, b;
    in >> a >> b;
    g[a-1].push_back(i);
    g[b-1].push_back(i);
    h[a-1].push_back(b-1);
    h[b-1].push_back(a-1);
    e[i] = {a-1, b-1};
  }
  in >> k;
  for (int i=0; i<k; i++) 
    in >> X[i] >> Y[i];
  TRACE(filename);
  TRACE(n _ m _ k);
}

int ccw(int a, int b, int c) {
  llint A = 
    llint(X[a])*(Y[b]-Y[c])+
    llint(X[b])*(Y[c]-Y[a])+
    llint(X[c])*(Y[a]-Y[b]);
  if (A < 0)
    return -1;
  if (A > 0)
    return 1;
  return 0;
}

int intersect(int a1, int a2, int b1, int b2) {
  if (a1 == b1 || a1 == b2)
    return 0 ;
  if (a2 == b1 || a2 == b2)
    return 0;
  return 
    ccw(a1, a2, b1)*ccw(a1, a2, b2) == -1 &&
    ccw(b1, b2, a1)*ccw(b1, b2, a2) == -1;
}

int intersect(const vector<int> &x, int e1, int e2) {
  return intersect(x[e[e1].first], x[e[e1].second], x[e[e2].first], x[e[e2].second]);
}

struct mapping {
  vector<int> x;
  vector<int> free;
  vector<set<int> > cuts;
  int total;

  void init() {
    assert((int)x.size() == n);
    free = vector<int>(k+1, 1);
    for (int i=0; i<n; i++) 
      free[x[i]] = 0;    
    cuts.clear();
    cuts.resize(m);
    total = 0;
    for (int i=0; i<m; i++) 
      for (int j=i+1; j<m; j++) 
        if (intersect(x, i, j))
          cuts[i].insert(j), cuts[j].insert(i), total++;
  }

  int move(int i, int p) {
    assert(0 <= i && i <= n);
    assert(free[p]);
    int oldtotal = total;
    free[x[i]] = 1;
    x[i] = p;
    free[x[i]] = 0;
    for (auto a : g[i]) {
      total -= (int)cuts[a].size();
      for (auto b : cuts[a])
        cuts[b].erase(a);
      cuts[a].clear();
    }
    for (auto a : g[i]) {
      for (int c=0; c<m; c++)
        if (a != c)
          if (intersect(x, a, c))
            cuts[a].insert(c), cuts[c].insert(a), total++;
    }
    return total-oldtotal;
  }

  int swp(int i, int j) {
    assert(0 <= i && i < n);
    assert(0 <= j && j < n);
    assert(i != j);
    int oldtotal = total;
    k++;
    free[k-1] = 1;
    int xi = x[i];
    int xj = x[j];
    move(i, k-1);
    move(j, xi);
    move(i, xj);
    k--;
    return total-oldtotal;
  }

  int move_random(int i) {
    assert(k != n);
    int p = rand()%k;
    while (!free[p])
      p = (p+1)%k;
    // TRACE("move" _ i _ p _ delta _ total);
    return total;
  }

  int swap_random_if_better() {
    int a = rand()%n;
    int b = (a+1+rand()%(n-1))%n;
    int delta = swp(a, b);
    if (delta > 0)
      swp(a, b);
    return total;
  }

  int swap_random() {
    int a = rand()%n;
    int b = (a+1+rand()%(n-1))%n;
    int delta = swp(a, b);
    return total;
  }

  int best_swap_with_random() {
    int a = rand()%n;
    int best_delta = 0;
    int c = -1;
    for (int b=0; b<n; b++)
      if (b != a) {
        int delta = swp(a, b);
        swp(a, b);
        if (delta < best_delta) {
          best_delta = delta;
          c = b;
        }
      }
    if (c != -1)
      swp(a, c);
    return total;
  }

  int best_cross(int a) {
    int best_cross = 0;
    int c = -1;
    set<int> r = cuts[a];
    for (auto b : r) {
      if (e[a].second == e[b].second)
        continue ;
      int delta = swp(e[a].second, e[b].second);
      swp(e[a].second, e[b].second);
      if (delta < best_cross) {
        best_cross = delta;
        c = b;
      }
    }
    if (c != -1)
      swp(e[a].second, e[c].second);
    // TRACE(best_cross);
    return total;
  }

  int move_random_if_better(int i) {
    assert(k != n);
    int old = x[i];
    int p = rand()%k;
    while (!free[p])
      p = (p+1)%k;
    int delta = move(i, p);
    if (delta > 0)
      move(i, old);
    return total;
  }

  mapping(const vector<int> &_x) : x(_x) {
    init();
  }

  int cnt() const {
    return total;
  }
};

vector<int> rands() {
  vector<int> x(k);
  for (int i=0; i<k; i++) 
    x[i] = i;
  random_shuffle(x.begin(), x.end());
  x.resize(n);
  return x;

}
int cnt(const vector<int> &x, int a1, int a2) {
  int c = 0;
  for (int i=0; i<n; i++) 
    for(auto j : h[i])
      if (i < j)
        c += intersect(x[a1], x[a2], x[i], x[j]);
  return c;
}

int score(const vector<int> &x) {
  int s = 0;
  for (int i=0; i<n; i++) 
    for(auto j : h[i])
      if (i < j)
        s += cnt(x, i, j);
  return s/2 ;
}

void out(const vector<int> &x, string filename) {
  TRACE(filename _ score(x));
  ofstream out(filename);
  for (auto a : x)
    out << a+1 << endl;
  out.close();
}

vector<int> load(string filename) {
  ifstream in(filename);
  vector<int> x(n);
  for (int i=0; i<n; i++) {
    in >> x[i];
    TRACE(x[i]);
    x[i]--;
    assert(0 <= x[i] && x[i] < k);
  }
  return x;
}

int main(int argc, char *argv[]) {
  read(argv[1]);
  vector<int> sol = rands();
  if (argc >= 3) {
    sol = load(argv[2]);
    TRACE("load" _ argv[2]);
  }
  out(sol, string(argv[1]) + ".out");
  mapping curr = mapping(sol);
  int best = curr.cnt();
  // Ovo se mijenjalo cijelo vrijeme tak da ne znam vise sta je tocno najbolje za koji file :)
  for (int i=0; i<10000000; i++) {
   // for (int i=0; i<10; i++) {
   //    int temp = curr.best_swap_with_random();
   //    if (temp < best) {
   //      best = temp;
   //      out(curr.x, string(argv[1]) + ".out");
   //    }
   // }
   // TRACE("switch");
    TRACE(0);
    for (int i=0; i<2000; i++) {
      int temp = curr.move_random_if_better(rand()%n);
      if (temp < best) {
        best = temp;
        out(curr.x, string(argv[1]) + ".out");
      }
    }
      
    TRACE(1);
   for (int i=0; i<m; i++) {
      int temp = curr.best_cross(i);
      TRACE(temp);
      if (temp < best) {
        best = temp;
        out(curr.x, string(argv[1]) + ".out");
      }
   }
   TRACE(3);
   for (int i=0; i<2; i++)
     curr.swap_random();
   TRACE(2);
   for (int i=0; i<1000; i++) {
     int temp = curr.best_swap_with_random();
     TRACE(temp);
     if (temp < best) {
       best = temp;
       out(curr.x, string(argv[1]) + ".out");
     }
   }
  }
    
  return 0;
}
