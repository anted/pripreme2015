#include <cstdio>
#include <algorithm>
#include <vector>
#include <map>
#include <cstring>

#define FOR(a,b,c) for (int c = (a), _for = (b); c < _for; ++c)
#define REP(n) for (int _rep = 0, _for = (n); _rep < _for; ++_rep)
#define pb push_back
#define x first
#define y second
#define ll long long
#define pii pair < int, int >

using namespace std;

int loga[300005];
void add(int p, int v){ ++p;
	for (; p < 300005; p += p & -p)
		loga[p] += v;
}
ll sum(int p){ ++p;
	ll r = 0;
	for (; p; p &= (p - 1))
		r += loga[p];
	return r;
}
void AddInter(int lo, int hi, int v){add(lo, v); add(hi + 1, -v);}

int n, r, q;
int S[200005], H[200005];
vector < int > E[200005];
vector < int > Reg[200005];

void operator+=(vector < int > &a, vector < int > b){FOR(0, r, i) a[i] += b[i];}

ll S1Out[505][505];
vector < int > S1DFS(int id){
	vector < int > R(r, 0);
	for (auto x : E[id]) R += S1DFS(x);
	FOR(0, r, i) S1Out[H[id]][i] += R[i];
	++R[H[id]];
	return R;
}
void Solve1(){
	S1DFS(0);
	REP(q){
		int a, b;
		scanf("%d%d", &a, &b), --a, --b;
		printf("%lld\n", S1Out[a][b]), fflush(stdout);
	}
}

int Disc[200005], Fin[200005]; // x-to podstablo <=> [Disc[x], Fin[x]]
int Inv[200005];
void S2DFS(int id){
	static int DT = 0;
	Disc[id] = DT++;
	Inv[Disc[id]] = id;
	for (auto x : E[id]) S2DFS(x);
	Fin[id] = DT - 1;
}
vector < int > DR[200005];
void Solve2(){
	S2DFS(0);
	FOR(0, r, i){
		for (auto x : Reg[i]) DR[i].pb(Disc[x]);
		sort(DR[i].begin(), DR[i].end());
	}
	REP(q){
		int a, b;
		scanf("%d%d", &a, &b), --a, --b;
		ll R = 0;
		for (auto x : Reg[a]) R += upper_bound(DR[b].begin(), DR[b].end(), Fin[x]) - lower_bound(DR[b].begin(), DR[b].end(), Disc[x]);
		printf("%lld\n", R);
	}
}

bool Huge[200005];
vector < int > HV;
int HPrep[50][25005];
map < int, int > M;

void Solve3(){
	S2DFS(0);
	FOR(0, r, i){
		for (auto x : Reg[i]) DR[i].pb(Disc[x]);
		sort(DR[i].begin(), DR[i].end());
		if (DR[i].size() > 500) Huge[i] = true, M[i] = HV.size(), HV.pb(i);
	}
	FOR(0, HV.size(), i){
		int t = HV[i];
		memset(loga, 0, sizeof loga);
		for (auto x : Reg[t]) AddInter(Disc[x], Fin[x], 1);
		FOR(0, r, j){
			int &R = HPrep[i][j];
			for (auto y : Reg[j]) R += sum(Disc[y]);
		}
	}
	REP(q){
		int a, b;
		scanf("%d%d", &a, &b), --a, --b;
		if (!Huge[a]){
			ll R = 0;
			for (auto x : Reg[a]) R += upper_bound(DR[b].begin(), DR[b].end(), Fin[x]) - lower_bound(DR[b].begin(), DR[b].end(), Disc[x]);
			printf("%lld\n", R);
		}
		else { // masivan, ima ih max 50
			printf("%d\n", HPrep[M[a]][b]);
		}
	}
}

int main(){
	scanf("%d%d%d", &n, &r, &q);
	scanf("%d", H), --H[0];
	FOR(1, n, i) scanf("%d%d", S + i, H + i), --S[i], --H[i]; // 0-indexirano
	
	FOR(1, n, i) E[S[i]].pb(i);
	FOR(0, n, i) Reg[H[i]].pb(i); Solve3(); return 0;
	
	if (r <= 500){Solve1(); return 0;}
	FOR(0, r, i) if (Reg[i].size() > 500){Solve3(); return 0;}
	Solve2();
	
	return 0;
}
