#include <cstdio>
#include <algorithm>
#include <vector>
#include <set>

#define FOR(a,b,c) for (int c = (a), _for = (b); c < _for; ++c)
#define REP(n) for (int _rep = 0, _for = (n); _rep < _for; ++_rep)
#define pb push_back
#define x first
#define y second
#define ll long long
#define pii pair < int, int >
#define all(x) (x).begin(), (x).end()

using namespace std;

pair < int, double > merge(pair < int, double > A, pair < int, double > B){return {A.x + B.x, A.y + B.y};}

const double ep = 1e-9;

const int offset = (1 << 19);
pair < int, double > T[offset * 2];
void Set(int id, int lo, int hi, int pos, pair < int, double > val){
	if (lo > pos || hi < pos) return;
	if (lo == hi){T[id] = val; return;}
	int mid = (lo + hi) / 2;
	Set(id * 2, lo, mid, pos, val);
	Set(id * 2 + 1, mid + 1, hi, pos, val);
	T[id] = merge(T[id * 2], T[id * 2 + 1]);
}
void Set(int pos, pair < int, double > val){Set(1, 0, offset - 1, pos, val);}
pair < int, double > Query(int id, int lo, int hi, double Cost){
	if (lo == hi){
		if (Cost + ep > T[id].y) return T[id];
		else return {0, 0.0};
	}
	int mid = (lo + hi) / 2;
	if (Cost + ep < T[id * 2].y) return Query(id * 2, lo, mid, Cost);
	else return merge(T[id * 2], Query(id * 2 + 1, mid + 1, hi, Cost - T[id * 2].y));
}
pair < int, double > Query(double Cost){return Query(1, 0, offset - 1, Cost);}

int n;
ll w;
pii in[500005]; // {s, q}
int P[500005];

vector < pair < int, int > > S;

int main(){
	scanf("%d%lld", &n, &w);
	FOR(0, n, i) scanf("%d%d", &in[i].x, &in[i].y);
	
	vector < pair < double, int > > Sweep;
	FOR(0, n, i) Sweep.pb({(double)in[i].x / (double)in[i].y, i});
	sort(all(Sweep));
	
	int Best = -1, BestPos = -1;
	double BestCost;
	for (auto x : Sweep) S.pb({in[x.y].y, x.y});
	int CnS = 0; sort(all(S));
	for (auto x : S) P[x.y] = CnS++;
	
	FOR(0, Sweep.size(), i){
		double x = Sweep[i].x;
		int id = Sweep[i].y;
		Set(P[id], {1, (double)in[id].y});
		auto T = Query(w / x);
		T.y *= x;
		if (T.x > Best || T.x == Best && T.y < BestCost) Best = T.x, BestPos = i, BestCost = T.y;
	}
	
	printf("%d\n", Best);
	S.clear();
	FOR(0, BestPos + 1, i) S.pb({in[Sweep[i].y].y, Sweep[i].y}); sort(all(S));
	double x = Sweep[BestPos].x, Cost = 0;
	for (auto u : S){
		if (Cost + x * u.x > w + ep) break;
		Cost += x * u.x;
		printf("%d\n", u.y + 1);
	}
	return 0;
}
