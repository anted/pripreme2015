#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <iostream>
#include <algorithm>
#include <vector>
#include <deque>
#include <complex>
using namespace std;

#define REP(i, n) for (int i = 0; i < (n); ++i)

typedef long long llint;

const int MAXN = 500100;

struct point { int x, y; };

int N;
point S, E;
point X[MAXN];
int Y[MAXN];

llint ccw(point a, point b, point c) {
  return (llint)a.x*(b.y-c.y) + (llint)b.x*(c.y-a.y) + (llint)c.x*(a.y-b.y);
}

long double sqr(int x) { return (long double)x*x; }

long double dist(point a, point b) {
  return sqrt(sqr(a.x-b.x) + sqr(a.y-b.y));
}

llint SQR(int x) { return (llint)x*x; }

void solve() {
  long double ans = 0;
  deque<point> dL, dR;
 
  static vector<llint> vec;

  for (int i = 0; i <= N+1; ++i) {
    int xl = X[i].x;
    int xr = X[i].y;

    while (dL.size() >= 2 && ccw(dL[dL.size()-2], dL.back(), {xl, Y[i]}) >= 0) dL.pop_back();
    while (dR.size() >= 2 && ccw(dR[dR.size()-2], dR.back(), {xr, Y[i]}) <= 0) dR.pop_back();
    dL.push_back({xl, Y[i]});
    dR.push_back({xr, Y[i]});
    
    while (dL.size() > 2 && ccw(dL[0], dL[1], dR.back()) <= 0) {
      //ans += dist(dL[0], dL[1]);
      vec.push_back(SQR(dL[0].x - dL[1].x) + SQR(dL[0].y - dL[1].y));
      dL.pop_front();
      dR.pop_front();
      dR.push_front(dL.front());
    }
    
    while (dR.size() > 2 && ccw(dR[0], dR[1], dL.back()) >= 0) {
      //ans += dist(dR[0], dR[1]);
      vec.push_back(SQR(dR[0].x - dR[1].x) + SQR(dR[0].y - dR[1].y));
      dR.pop_front();
      dL.pop_front();
      dL.push_front(dR.front());
    }
  }
 
  
  vec.push_back(SQR(dL[0].x - dL[1].x) + SQR(dL[0].y - dL[1].y));
  ans = 0;
  for (llint it: vec) ans += sqrt((double)it);

  //ans += dist(dL[0], dL[1]);
  printf("%.4Lf\n", ans);
}

void load() {
  scanf("%d", &N);
  scanf("%d%d", &S.x, &S.y);
  for (int i = 1; i <= N; ++i)
    scanf("%d%d%d", &X[i].x, &X[i].y, &Y[i]);
  scanf("%d%d", &E.x, &E.y);
  X[0].x = X[0].y = S.x;
  Y[0] = S.y;
  X[N+1].x = X[N+1].y = E.x;
  Y[N+1] = E.y;
}

int main(void) {
  load();
  solve();
  return 0;
}

