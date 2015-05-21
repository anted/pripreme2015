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

int n, t, p;
vector < int > V[2005];
int Val[2005];
int Score[2005];

bool cmp(int a, int b){
	if (Score[a] != Score[b]) return Score[a] > Score[b];
	if (V[a].size() != V[b].size()) return V[a].size() > V[b].size();
	return a < b;
}

int main(){
	scanf("%d%d%d", &n, &t, &p), --p;
	FOR(0, n, i) FOR(0, t, j){
		int x;
		scanf("%d", &x);
		if (x) V[i].pb(j);
		else ++Val[j];
	}
	FOR(0, n, i) for (auto x : V[i]) Score[i] += Val[x];
	vector < int > Order;
	FOR(0, n, i) Order.pb(i);
	sort(all(Order), cmp);
	printf("%d ", Score[p]);
	FOR(0, n, i) if (Order[i] == p) printf("%d\n", i + 1);
	return 0;
}
