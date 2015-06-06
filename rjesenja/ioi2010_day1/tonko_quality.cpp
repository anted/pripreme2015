#include "quality.h"

#include <cstdio>
#include <algorithm>
using namespace std;

const int MAXN = 3001;

int R, C, H, W, Q[MAXN][MAXN];

int xpos[MAXN*MAXN], ypos[MAXN*MAXN];



bool check(int k) {
  static int w[3100][3100];

  int lim = (H*W + 1)/2;
  int r = R - H + 1;
  int c = C - W + 1;

  for (int i = 0; i <= R+1; ++i)
    for (int j = 0; j <= C+1; ++j)
      w[i][j] = 0;

  for (int i = 1; i <= k; ++i) {
    int lx = xpos[i] - H + 1;
    int hx = xpos[i];// + W - 1;
    int ly = ypos[i] - W + 1;
    int hy = ypos[i];// + H - 1;
    
    lx = 1 + max(lx, 0);
    ly = 1 + max(ly, 0);
    hx = 2 + min(hx, R - H);
    hy = 2 + min(hy, C - W);

    ++w[lx][ly];
    --w[lx][hy];
    --w[hx][ly];
    ++w[hx][hy];

  }
  
  for (int i = 1; i <= r; ++i) {
    for (int j = 1; j <= c; ++j) {
      w[i][j] +=  w[i-1][j] + w[i][j-1] - w[i-1][j-1];
      if (w[i][j] >= lim) return true;
    }
  }
 
  return false;
}

int rectangle(int r, int c, int h, int w, int q[3001][3001]) {
  R = r; C = c;
  W = w; H = h;
  for (int i = 0; i < R; ++i)
    for (int j = 0; j < C; ++j)
      Q[i][j] = q[i][j];

  for (int i = 0; i < R; ++i)
    for (int j = 0; j < C; ++j) {
      xpos[Q[i][j]] = i;
      ypos[Q[i][j]] = j;
    }

  int lo = 1, hi = R*C;
  while (lo < hi) {
    int mid = (lo + hi) / 2;
    if (check(mid)) hi = mid;
    else lo = mid + 1;
  }

	return lo;
}
