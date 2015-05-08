#include <cstdio>
#include <algorithm>
#include <vector>
#include <cstring>
#include <iostream>
#include <ctime>

#define FOR(a,b,c) for (int c = (a), _for = (b); c < _for; ++c)
#define REP(n) for (int _rep = 0, _for = (n); _rep < _for; ++_rep)
#define pb push_back
#define x first
#define y second
#define ll long long
#define pii pair < int, int >
#define all(x) (x).begin(), (x).end()

using namespace std;

ll ccw(pii a, pii b, pii c){
	return (ll)a.x * (b.y - c.y) + (ll)b.x * (c.y - a.y) + (ll)c.x * (a.y - b.y);
}

int sccw(pii a, pii b, pii c){
	ll T = ccw(a, b, c);
	if (T < 0) return -1;
	if (T > 0) return 1;
	return 0;
}

int n, m;
vector < int > EL[505];
vector < pii > E;
int l;
pii p[505];

bool cross(int a, int b, int c, int d){
	return sccw(p[a], p[b], p[c]) * sccw(p[a], p[b], p[d]) == -1 && sccw(p[c], p[d], p[a]) * sccw(p[c], p[d], p[b]) == -1;
}
int CN[505];
int Count(vector < int > &v){ // O(E^2)
	int R = 0;
	memset(CN, 0, sizeof CN);
	for (auto x : E)
		for (auto y : E)
			if (cross(v[x.x], v[x.y], v[y.x], v[y.y]))
				++R, ++CN[v[x.x]], ++CN[v[x.y]];
	return R / 2;
}
int CountNode(vector < int > &v, int id){ // O(deg * E) ~ O(NE)
	int R = 0;
	for (auto x : EL[id])
		for (auto y : E)
			if (cross(v[id], v[x], v[y.x], v[y.y]))
				++R;
	return R;
}

void Set(vector < int > &v, int &c, int p, int pp){
	c -= CountNode(v, p);
	v[p] = pp;
	c += CountNode(v, p);
}
void Swap(vector < int > &v, int &c, int a, int b){
	int A = v[a], B = v[b];
	Set(v, c, a, l);
	Set(v, c, b, A);
	Set(v, c, a, B);
}

int main(){
	srand(time(0));
	scanf("%d%d", &n, &m);
	REP(m){
		int a, b;
		scanf("%d%d", &a, &b), --a, --b;
		E.pb({a, b});
		EL[a].pb(b);
		EL[b].pb(a);
	}
	scanf("%d", &l);
	FOR(0, l, i) scanf("%d%d", &p[i].x, &p[i].y);
	p[l] = {200000, 200000};
	
	vector < int > OptV;
	int OC = 1000000000;
	vector < int > SV;
	int SC;
	FOR(0, n, i) SV.pb(i);
	SC = Count(SV);
	
	FILE* F = fopen("mig2temp", "r");
	int T;
	while (fscanf(F, "%d", &T) == 1) OptV.pb(T - 1);
	if ((int)OptV.size() == n){
		OC = Count(OptV);
		SC = OC, SV = OptV;
	}
	
	const int Limit = 110000;
	
	while (OC > Limit){
		REP(rand() & 1){
			int T = rand() % n;
			int A = SV[T], B = rand() % (l - 1);
			if (B == A) B = l - 1;
			int T2 = -1;
			FOR(0, SV.size(), i) if (SV[i] == B) T2 = i;
			if (T2 == -1){
				Set(SV, SC, T, B);
			}
			else {
				Swap(SV, SC, T, T2);
			}
		} // razmucka
		
		REP(300){
			vector < int > V = SV;
			int C = SC;
			int T = rand() % n;
			int A = SV[T], B = rand() % (l - 1);
			if (B == A) B = l - 1;
			int T2 = -1;
			FOR(0, SV.size(), i) if (SV[i] == B) T2 = i;
			if (T2 == -1){
				Set(V, C, T, B);
			}
			else {
				Swap(V, C, T, T2);
			}
			if (C < SC) SC = C, SV = V;
		} // optimizira
		
		REP(50){
			int E1 = rand() % E.size(), E2 = rand() % (E.size() - 1);
			if (E1 == E2) E2 = E.size() - 1;
			if (!cross(SV[E[E1].x], SV[E[E1].y], SV[E[E2].x], SV[E[E2].y])){--_rep; continue;}
			vector < int > V = SV;
			int C = SC;
			Swap(V, C, E[E1].x, E[E2].x);
			if (C < SC) SC = C, SV = V;
			V = SV;
			C = SC;
			Swap(V, C, E[E1].x, E[E2].y);
			if (C < SC) SC = C, SV = V;
		} // cross optimizacija
		
		if (SC < OC) OC = SC, OptV = SV;
		SC = OC, SV = OptV;
		
		cerr << OC << endl;
		
		// temp storage
		FILE* F = fopen("mig2temp", "w");
		for (auto x : OptV) fprintf(F, "%d\n", x + 1);
		fclose(F);
		
	}
	
	for (auto x : OptV) printf("%d\n", x + 1);
	
	return 0;
}
