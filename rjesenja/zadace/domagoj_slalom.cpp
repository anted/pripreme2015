#include <cstdio>
#include <algorithm>
#include <deque>
#include <cmath>

using namespace std; 

typedef pair<int, int> P;
typedef long long ll; 

#define X first
#define Y second

int Ccw(P a, P b, P c)
{
  ll tmp = (ll) a.X * (b.Y - c.Y) + (ll) b.X * (c.Y - a.Y) + (ll) c.X * (a.Y - b.Y);
  if (tmp > 0)
    return 1;
  if (tmp < 0)
    return -1; 
  return 0;
}

ll Sqr(int a)
{
  return (ll) a * a;
}

double Dist(P a, P b)
{
  return sqrt(Sqr(a.X - b.X) + Sqr(a.Y - b.Y));
}

deque <P> L, R;

void Stavi(deque <P> &Q, P toc, int nebris)
{
  for (; Q.size() > 1; ) { 
    P pred = Q[Q.size()-2];
    P zad = Q.back();
    if (Ccw(pred, zad, toc) == nebris)
      break;
    Q.pop_back();
  }

  Q.push_back(toc);
}

double Naprijed()
{
  double ret=0;

  for (; L.size() > 1 && R.size() > 1; ) {
    if (Ccw(L[0], R[1], L[1]) != -1) {
      if (L[1].Y < R[1].Y) {
        ret += Dist(R[0], R[1]);
        R.pop_front();
        L.pop_front();
        L.push_front(R[0]);
      }
      else {
        ret += Dist(L[0], L[1]);
        L.pop_front();
        R.pop_front();
        R.push_front(L[0]);
      }
    }
    else
      break;
  }

  return ret;
}

int main()
{
  int n;

  scanf("%d", &n); n += 2;

  int x, y;
  scanf("%d%d", &x, &y);
  L.push_back(P(x, y));
  R.push_back(P(x, y));
  double rje=0;

  for (int i=1; i<n; i++) {
    P l, r;
    if (i == n - 1) {
      scanf("%d%d", &x, &y);
      l = r = P(x, y);
    }
    else {
      int x1, x2, y1;
      scanf("%d%d%d", &x1, &x2, &y1);
      l = P(x1, y1);
      r = P(x2, y1);
    }

    Stavi(L, l, -1);
    Stavi(R, r, 1);
    rje += Naprijed();
  }

  printf("%.4lf\n", rje);

  return 0;
}
