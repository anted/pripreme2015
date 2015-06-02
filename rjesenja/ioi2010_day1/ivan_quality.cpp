#include "quality.h"

#include <vector>
#include <cstdio>
#include <algorithm>
#include <cstring>

#define FOR(a,b,c) for (int c = (a), _for = (b); c < _for; ++c)
#define REP(n) FOR(0, n, _rep)
#define pb push_back
#define pii pair < int, int >
#define x first
#define y second
#define ll long long

using namespace std;

int r, c, h, w;
int mat[3005][3005];

int m2[3005][3005];
int target;

bool Query(int limit){ // postoji li medijan >= limit
	memset(m2, 0, sizeof m2);
	FOR(1, r + 1, i) FOR(1, c + 1, j) m2[i][j] = (mat[i][j] <= limit) + m2[i - 1][j] + m2[i][j - 1] - m2[i - 1][j - 1];
	FOR(h, r + 1, i) FOR(w, c + 1, j) if (m2[i][j] - m2[i - h][j] - m2[i][j - w] + m2[i - h][j - w] >= target) return true;
	return false;
}

int rectangle(int R, int C, int H, int W, int Q[3001][3001]) {
	memset(mat, 0, sizeof mat);
	r = R, c = C, h = H, w = W;
	FOR(1, r + 1, i) FOR(1, c + 1, j) mat[i][j] = Q[i - 1][j - 1];
	target = (h * w + 1) / 2;
	
	int lo = 0, hi = r * c, mid = (lo + hi) / 2;
	while (lo != hi){
		if (Query(mid)) hi = mid;
		else lo = mid + 1;
		mid = (lo + hi) / 2;
	} return lo;
	
}
