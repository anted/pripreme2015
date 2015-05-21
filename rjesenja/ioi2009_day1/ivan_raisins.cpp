#include <cstdio>
#include <algorithm>
#include <vector>

#define FOR(a,b,c) for (int c = (a), _for = (b); c < _for; ++c)
#define REP(n) for (int _rep = 0, _for = (n); _rep < _for; ++_rep)
#define pb push_back
#define x first
#define y second
#define ll long long
#define pii pair < int, int >
#define all(x) (x).begin(), (x).end()

using namespace std;

const int oo = 1000000000;

int n, m;
int in[55][55];

int Sum(int a, int b, int c, int d){
	return in[c][d] - in[a - 1][d] - in[c][b - 1] + in[a - 1][b - 1];
}

int memo[51][51][51][51];
void dp(int a, int b, int c, int d){
	int &r = memo[a][b][c][d];
	if (a == c && b == d){r = 0; return;}
	r = oo;
	FOR(a, c, k) r = min(r, memo[a][b][k][d] + memo[k + 1][b][c][d]);
	FOR(b, d, k) r = min(r, memo[a][b][c][k] + memo[a][k + 1][c][d]);
	r += Sum(a, b, c, d);
}

int main(){
	scanf("%d%d", &n, &m);
	FOR(1, n + 1, i) FOR(1, m + 1, j) scanf("%d", in[i] + j), in[i][j] += in[i - 1][j] + in[i][j - 1] - in[i - 1][j - 1];
	FOR(1, n + 1, l1) FOR(1, m + 1, l2) FOR(1, n + 2 - l1, i) FOR(1, m + 2 - l2, j) dp(i, j, i + l1 - 1, j + l2 - 1);
	printf("%d\n", memo[1][1][n][m]);
	return 0;
}
