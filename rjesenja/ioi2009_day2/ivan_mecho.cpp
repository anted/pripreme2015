#include <cstdio>
#include <algorithm>
#include <vector>
#include <queue>
#include <cstring>

#define FOR(a,b,c) for (int c = (a), _for = (b); c < _for; ++c)
#define REP(n) for (int _rep = 0, _for = (n); _rep < _for; ++_rep)
#define pb push_back
#define x first
#define y second
#define ll long long
#define pii pair < int, int >

using namespace std;

pii operator+(pii a, pii b){return {a.x + b.x, a.y + b.y};}
void operator+=(pii &a, pii b){a = a + b;}
const pii dt[] = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};

int n, s;
char mat[805][805];
pii Start, End;

vector < pii > Bees[805 * 805];
bool Taken[805][805];

void BuildBees(){
	memset(Taken, 0, sizeof Taken);
	FOR(0, n, i) FOR(0, n, j) if (mat[i][j] == 'T' || mat[i][j] == 'D') Taken[i][j] = true;
	queue < pii > Q;
	for (auto x : Bees[0]) Taken[x.x][x.y] = true, Q.push(x);
	int Counter = 0;
	while (!Q.empty()){ ++Counter;
		queue < pii > Q2;
		while (!Q.empty()){
			pii t = Q.front(); Q.pop();
			for (auto dt : ::dt){
				pii nt = t + dt;
				if (nt.x < 0 || nt.y < 0 || nt.x >= n || nt.y >= n) continue;
				if (Taken[nt.x][nt.y]++) continue;
				Q2.push(nt), Bees[Counter].pb(nt);
			}
		}
		Q = Q2;
	}
}

bool T2[805][805];
bool Query(int Time){
	memset(Taken, 0, sizeof Taken);
	memset(T2, 0, sizeof T2);
	FOR(0, n, i) FOR(0, n, j) if (mat[i][j] == 'T') Taken[i][j] = true;
	FOR(0, Time + 1, i) for (auto x : Bees[i]) Taken[x.x][x.y] = true;
	queue < pii > Q;
	Q.push(Start);
	FOR(Time + 1, n * n + 1, i){
		REP(s){ // s iteracija bfs sirenja
			if (Q.empty()) return false;
			queue < pii > Q2;
			while (!Q.empty()){
				pii t = Q.front(); Q.pop();
				if (Taken[t.x][t.y]++) continue;
				for (auto dt : ::dt){
					pii nt = t + dt;
					if (nt.x < 0 || nt.y < 0 || nt.x >= n || nt.y >= n) continue;
					if (Taken[nt.x][nt.y] || T2[nt.x][nt.y]++) continue;
					if (nt == End) return true; // postoji put
					Q2.push(nt);
				}
			} Q = Q2;
		}
		for (auto x : Bees[i]) Taken[x.x][x.y] = true;
	} return false; // nije nista nasao
}

int main(){
	scanf("%d%d", &n, &s);
	FOR(0, n, i) scanf("%s", mat[i]);
	FOR(0, n, i) FOR(0, n, j){
		if (mat[i][j] == 'M') mat[i][j] = 'G', Start = {i, j};
		if (mat[i][j] == 'D') End = {i, j};
		if (mat[i][j] == 'H') Bees[0].pb({i, j});
	}
	
	BuildBees();
	
	if (!Query(0)){printf("-1\n"); return 0;}
	
	int lo = 0, hi = n * n, mid = (lo + hi + 1) / 2;
	while (lo != hi){
		if (Query(mid)) lo = mid;
		else hi = mid - 1;
		mid = (lo + hi + 1) / 2;
	} printf("%d\n", lo);
	
	return 0;
}
