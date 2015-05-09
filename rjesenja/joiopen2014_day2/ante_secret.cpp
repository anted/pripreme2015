#include "secret.h"
#include <cstring>
#include <map>

using namespace std;

typedef pair<int, int> P;

const int OFF = 1<<10;
const int SIZE = 1<<11;

int t[SIZE];
int n;
int a[SIZE];
map<P, int> Cache;

int mysecret(int x, int y) {
  if (x == -1 || y == -1)
    return -1;
  if (Cache.count({x, y}))
    return Cache[{x, y}];
  return Cache[{x, y}] = Secret(x, y);
}

int myquery(int L, int R, bool final) {
  int x = OFF+L;
  int y = OFF+R;
  int sx = t[x];
  int sy = t[y];
  if (x == y)
    return sx;
  for (; x/2 != y/2; x/=2, y/=2) {
    if (x%2 == 0)
      sx = mysecret(sx, t[2*(x/2)+1]);
    if (y%2 == 1)
      sy = mysecret(t[2*(y/2)], sy);
  }
  if (!final)
    return -1;
  return mysecret(sx, sy);
}

void Init(int N, int A[]) {
  memset(t, -1, sizeof t);
  n = N;
  for (int i=0; i<n; i++) 
    t[OFF+i] = A[i];
  for (int i=OFF-1; i>0; i--)
    t[i] = mysecret(t[2*i], t[2*i+1]);
  for (int i=0; i<n; i++) 
    myquery(i, n-1, false);
  for (int i=0; i<n; i++) 
    myquery(0, i, false);
}

int Query(int L, int R) {
  return myquery(L, R, true);
}
