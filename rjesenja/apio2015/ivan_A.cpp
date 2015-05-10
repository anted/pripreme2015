#include <cstdio>
#include <algorithm>
#include <vector>
#include <cstring>

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
const ll ool = (ll)oo * oo;

int n, a, b;
int y[2005];

ll GV;
bool good(ll x){
	return (GV & x) == x;
}

vector < pair < ll, int > > E[2005];
ll D1[2005], D2[2005]; // D1 = max, D2 = min

void Solve(){
	FOR(0, n, i){
		ll D = 0;
		FOR(i, n, j){
			D += y[j];
			E[i].pb({D, j + 1});
		}
	}
	ll R = (1LL << 60) - 1;
	for (int i = 59; i >= 0; --i){
		memset(D1, 0, sizeof D1);
		for (auto &x : D2) x = ool;
		GV = (R ^ (1LL << i));
		D1[0] = D2[0] = 0;
		FOR(0, n, i) for (auto x : E[i]){
			if (!good(x.x)) continue;
			D1[x.y] = max(D1[x.y], D1[i] + 1);
			D2[x.y] = min(D2[x.y], D2[i] + 1);
		}
		if (D2[n] > b || D1[n] < a) continue;
		R ^= (1LL << i);
	}
	printf("%lld\n", R);
}

int main(){
	scanf("%d%d%d", &n, &a, &b);
	FOR(0, n, i) scanf("%d", y + i);
	Solve();
	return 0;
}
