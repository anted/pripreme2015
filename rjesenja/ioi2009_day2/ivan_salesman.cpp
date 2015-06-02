#include <cstdio>
#include <algorithm>
#include <vector>
#include <assert.h>
#include <set>

#define FOR(a,b,c) for (int c = (a), _for = (b); c < _for; ++c)
#define REP(n) for (int _rep = 0, _for = (n); _rep < _for; ++_rep)
#define pb push_back
#define x first
#define y second
#define ll long long
#define pii pair < int, int >

using namespace std;

const int oo = 1000000000;
const ll ool = (ll)oo * oo;

int n, u, d, s;
int t[500005], l[500005], m[500005];

int Cost(int a, int b){
	int d = l[b] - l[a];
	if (d >= 0) return d * ::d;
	else return -d * u;
}

bool lcmp(int a, int b){return l[a] < l[b];}

vector < int > TV[500005];
ll C[5005];
void SolveSq(){
	FOR(0, n + 1, i) TV[t[i]].pb(i);
	for (auto &v : TV) sort(v.begin(), v.end(), lcmp);
	C[0] = 0;
	FOR(1, 5005, i){
		if (TV[i].size() == 0) continue;
		for (auto x : TV[i]){
			C[x] = -ool;
			FOR(0, n + 1, j) if (t[j] < t[x]) C[x] = max(C[x], C[j] - Cost(j, x) + m[x]);
		}
		vector < ll > tmp;
		for (auto x : TV[i]) tmp.pb(C[x]);
		ll Best = tmp[0];
		FOR(1, TV[i].size(), j){
			C[TV[i][j]] = max(C[TV[i][j]], Best - Cost(TV[i][j - 1], TV[i][j]) + m[TV[i][j]]);
			Best = max(Best - Cost(TV[i][j - 1], TV[i][j]) + m[TV[i][j]], tmp[j]);
		}
		reverse(tmp.begin(), tmp.end());
		reverse(TV[i].begin(), TV[i].end());
		Best = tmp[0];
		FOR(1, TV[i].size(), j){
			C[TV[i][j]] = max(C[TV[i][j]], Best - Cost(TV[i][j - 1], TV[i][j]) + m[TV[i][j]]);
			Best = max(Best - Cost(TV[i][j - 1], TV[i][j]) + m[TV[i][j]], tmp[j]);
		}
		reverse(tmp.begin(), tmp.end());
		reverse(TV[i].begin(), TV[i].end());
	}
	ll R = 0;
	FOR(0, n + 1, i) R = max(R, C[i] - Cost(i, 0));
	printf("%lld\n", R);
}

bool tcmp(int a, int b){return t[a] < t[b];}

class Turnament{
public:
	static const int offset = (1 << 19);
	ll T[offset * 2];
	Turnament(){for (auto &x : T) x = -ool;}
	void Set(int id, int lo, int hi, int p, ll v){
		if (lo > p || hi < p) return;
		if (lo == hi){T[id] = v; return;}
		int mid = (lo + hi) / 2;
		Set(id * 2, lo, mid, p, v);
		Set(id * 2 + 1, mid + 1, hi, p, v);
		T[id] = max(T[id * 2], T[id * 2 + 1]);
	}
	void Set(int p, int v){Set(1, 0, offset - 1, p, v);}
	ll Query(int id, int lo, int hi, int slo, int shi){
		if (lo > shi || hi < slo) return -ool;
		if (lo >= slo && hi <= shi) return T[id];
		int mid = (lo + hi) / 2;
		return max(Query(id * 2, lo, mid, slo, shi), Query(id * 2 + 1, mid + 1, hi, slo, shi));
	}
	ll Query(int lo, int hi){return Query(1, 0, offset - 1, lo, hi);}
} T1, T2;

ll Out[500005];
vector < int > Seq;
void SolveDis(){
	FOR(0, n + 1, i) Seq.pb(i);
	sort(Seq.begin(), Seq.end(), tcmp);
	Out[0] = 0;
	for (auto x : Seq){
		ll &v = Out[x];
		int &p = l[x];
		ll R1 = T1.Query(0, p) - d * p, R2 = T2.Query(p, 500005) + u * p;
		if (x == 0) v = 0; else v = m[x] + max(R1, R2);
		T1.Set(p, v + d * p);
		T2.Set(p, v - u * p);
	}
	ll R = -ool;
	FOR(0, n + 1, i) R = max(R, Out[i] - (ll)Cost(i, 0));
	printf("%lld\n", R);
}

void SolveAll(){
	FOR(0, n + 1, i) TV[t[i]].pb(i);
	for (auto &v : TV) sort(v.begin(), v.end(), lcmp);
	
	for (auto &V : TV){
		if (V.size() == 0) continue;
		for (auto x : V){
			ll &v = Out[x];
			int &p = l[x];
			ll R1 = T1.Query(0, p) - d * p, R2 = T2.Query(p, 500005) + u * p;
			if (x == 0) v = 0; else v = m[x] + max(R1, R2);
		}
		
		vector < ll > tmp;
		for (auto x : V) tmp.pb(Out[x]);
		ll Best = tmp[0];
		FOR(1, V.size(), j){
			Out[V[j]] = max(Out[V[j]], Best - Cost(V[j - 1], V[j]) + m[V[j]]);
			Best = max(Best - Cost(V[j - 1], V[j]) + m[V[j]], tmp[j]);
		}
		reverse(tmp.begin(), tmp.end());
		reverse(V.begin(), V.end());
		Best = tmp[0];
		FOR(1, V.size(), j){
			Out[V[j]] = max(Out[V[j]], Best - Cost(V[j - 1], V[j]) + m[V[j]]);
			Best = max(Best - Cost(V[j - 1], V[j]) + m[V[j]], tmp[j]);
		}
		reverse(tmp.begin(), tmp.end());
		reverse(V.begin(), V.end());
		for (auto x : V){
			ll &v = Out[x];
			int &p = l[x];
			T1.Set(p, v + d * p);
			T2.Set(p, v - u * p);
		}
	}
	
	ll R = -ool;
	FOR(0, n + 1, i) R = max(R, Out[i] - (ll)Cost(i, 0));
	printf("%lld\n", R);
}

int main(){
	scanf("%d%d%d%d", &n, &u, &d, &s);
	FOR(1, n + 1, i) scanf("%d%d%d", t + i, l + i, m + i);
	l[0] = s;
	if (n <= 5000){SolveSq(); return 0;}
	if ((int)set < int >(t, t + n + 1).size() == n + 1){SolveDis(); return 0;}
	SolveAll();
	return 0;
}
