#include <cstdio>
#include <algorithm>
#include <vector>
#include <map>
#include <set>
#include <cstring>

#include <ctime>

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

class Basic{
public:
	bool t;
	ll p;
	int id;
}; // false = +, true = -
bool operator<(Basic a, Basic b){
	if (a.t != b.t) return a.t < b.t;
	if (a.p != b.p) return a.p < b.p;
	return a.id < b.id;
}

class Turnament{
public:
	int offset;
	int size;
	pair < ll, int > *T;
	ll *P;
	void init(int s){
		size = s;
		offset = 1;
		while (offset < size) offset *= 2;
		T = new pair < ll, int >[offset * 2];
		P = new ll[offset * 2];
		FOR(0, offset * 2, i) T[i] = {0, 0}, P[i] = 0;
	}
	void update(int id){
		T[id] = min(T[id * 2], T[id * 2 + 1]);
		T[id].x += P[id];
	}
	void setid(int id, int lo, int hi, int p, int v){
		if (lo > p || hi < p) return;
		if (lo == hi){T[id].y = v; return;}
		int mid = (lo + hi) / 2;
		setid(id * 2, lo, mid, p, v);
		setid(id * 2 + 1, mid + 1, hi, p, v);
		update(id);
	}
	void setid(int p, int v){setid(1, 0, offset - 1, p, v);}
	void add(int id, int lo, int hi, int slo, int shi, ll v){
		if (lo > shi || hi < slo) return;
		if (lo >= slo && hi <= shi){
			P[id] += v;
			T[id].x += v;
			return;
		}
		int mid = (lo + hi) / 2;
		add(id * 2, lo, mid, slo, shi, v);
		add(id * 2 + 1, mid + 1, hi, slo, shi, v);
		update(id);
	}
	void add(int lo, int hi, ll v){if (lo > hi) return; add(1, 0, offset - 1, lo, hi, v);}
	pair < ll, int > mini(int id, int lo, int hi, int slo, int shi){
		if (lo > shi || hi < slo) return {ool, oo};
		if (lo >= slo && hi <= shi) return T[id];
		int mid = (lo + hi) / 2;
		auto a = mini(id * 2, lo, mid, slo, shi);
		auto b = mini(id * 2 + 1, mid + 1, hi, slo, shi);
		a = min(a, b);
		a.x += P[id];
		return a;
	}
	pair < ll, int > mini(int lo, int hi){return mini(1, 0, offset - 1, lo, hi);}
};

class Magija{ // pravac
public:
	vector < Basic > V;
	set < Basic > Active;
	Turnament T;
	Magija(){
		V.pb({false, -ool, -1});
		V.pb({true, ool, -1});
		V.pb({false, ool / 10, -1});
		V.pb({true, -ool / 10, -1});
	}
	ll MinDis(Basic t){
		if (!t.t){
			auto it = Active.lower_bound(Basic{true, t.p, -1});
			return it->p - t.p;
		}
		else {
			auto it = Active.upper_bound(Basic{false, t.p, -1});
			--it;
			return t.p - it->p;
		}
	}
	pair < ll, int > MinDisOuter(Basic t){
		if (!t.t){
			auto it = Active.lower_bound(Basic{true, t.p, -1});
			return {it->p - t.p, it->id};
		}
		else {
			auto it = Active.upper_bound(Basic{false, t.p, -1});
			--it;
			return {t.p - it->p, it->id};
		}
	}
	void init(){
		for (auto x : V) Active.insert(x);
		sort(V.begin(), V.end());
		T.init(V.size());
		FOR(0, V.size(), i){
			T.setid(i, V[i].id);
			T.add(i, i, MinDis(V[i]));
		}
	}
	pair < ll, int > StrMin(){
		return T.mini(0, V.size() - 1);
	}
	void kill(Basic t){
		int p = lower_bound(V.begin(), V.end(), t) - V.begin();
		T.add(p, p, oo);
		if (!t.t){
			auto it = Active.lower_bound(t);
			--it;
			Basic nt = *it;
			++it, ++it;
			Basic tt = *it;
			ll delta = t.p - nt.p;
			int lo = lower_bound(V.begin(), V.end(), Basic{true, t.p, -1}) - V.begin();
			int hi = lower_bound(V.begin(), V.end(), Basic{true, tt.p, -1}) - V.begin() - 1;
			T.add(lo, hi, delta);
		}
		else {
			auto it = Active.lower_bound(t);
			++it;
			Basic nt = *it;
			--it, --it;
			Basic tt = *it;
			ll delta = nt.p - t.p;
			int lo = upper_bound(V.begin(), V.end(), Basic{false, tt.p, -1}) - V.begin();
			int hi = upper_bound(V.begin(), V.end(), Basic{false, t.p, -1}) - V.begin() - 1;
			T.add(lo, hi, delta);
		}
		Active.erase(t);
	}
};

vector < Magija > MagV;
vector < pair < int, Basic > > Elem[500005]; // {magija, oblik unutar}
multiset < pair < ll, int > > MagS;

int n;
pii pt[500005];
int dir[500005]; // 0=gore, 1=desno, 2=dolje, 3=lijevo

void init(){
	map < int, int > M;
	FOR(0, n, i){ // 0-2
		if (dir[i] != 0 && dir[i] != 2) continue;
		int Val = pt[i].x, Pos = pt[i].y;
		if (!M.count(Val)){
			M[Val] = MagV.size();
			MagV.pb(Magija());
		}
		int magid = M[Val];
		Basic t = {dir[i] == 2, Pos, i};
		MagV[magid].V.pb(t);
		Elem[i].pb({magid, t});
	} M.clear();
	FOR(0, n, i){ // 1-3
		if (dir[i] != 1 && dir[i] != 3) continue;
		int Val = pt[i].y, Pos = pt[i].x;
		if (!M.count(Val)){
			M[Val] = MagV.size();
			MagV.pb(Magija());
		}
		int magid = M[Val];
		Basic t = {dir[i] == 3, Pos, i};
		MagV[magid].V.pb(t);
		Elem[i].pb({magid, t});
	} M.clear();
	FOR(0, n, i){ // 0-1
		if (dir[i] != 0 && dir[i] != 1) continue;
		int Val = pt[i].x + pt[i].y, Pos = pt[i].x - pt[i].y;
		if (!M.count(Val)){
			M[Val] = MagV.size();
			MagV.pb(Magija());
		}
		int magid = M[Val];
		Basic t = {dir[i] == 0, Pos, i};
		MagV[magid].V.pb(t);
		Elem[i].pb({magid, t});
	} M.clear();
	FOR(0, n, i){ // 0-3
		if (dir[i] != 0 && dir[i] != 3) continue;
		int Val = pt[i].x - pt[i].y, Pos = pt[i].x + pt[i].y;
		if (!M.count(Val)){
			M[Val] = MagV.size();
			MagV.pb(Magija());
		}
		int magid = M[Val];
		Basic t = {dir[i] == 3, Pos, i};
		MagV[magid].V.pb(t);
		Elem[i].pb({magid, t});
	} M.clear();
	FOR(0, n, i){ // 1-2
		if (dir[i] != 1 && dir[i] != 2) continue;
		int Val = pt[i].x - pt[i].y, Pos = pt[i].x + pt[i].y;
		if (!M.count(Val)){
			M[Val] = MagV.size();
			MagV.pb(Magija());
		}
		int magid = M[Val];
		Basic t = {dir[i] == 2, Pos, i};
		MagV[magid].V.pb(t);
		Elem[i].pb({magid, t});
	} M.clear();
	FOR(0, n, i){ // 2-3
		if (dir[i] != 2 && dir[i] != 3) continue;
		int Val = pt[i].x + pt[i].y, Pos = pt[i].x - pt[i].y;
		if (!M.count(Val)){
			M[Val] = MagV.size();
			MagV.pb(Magija());
		}
		int magid = M[Val];
		Basic t = {dir[i] == 3, Pos, i};
		MagV[magid].V.pb(t);
		Elem[i].pb({magid, t});
	} M.clear();
	for (auto &m : MagV){
		m.init();
		MagS.insert(m.StrMin());
	}
}

bool dead[500005];

void KillExt(int mid, Basic t){
	MagS.erase(MagS.find(MagV[mid].StrMin()));
	MagV[mid].kill(t);
	MagS.insert(MagV[mid].StrMin());
}

void kill(int id){
	dead[id] = true;
	for (auto x : Elem[id]) KillExt(x.x, x.y);
}

char input[10];
int main(){
	
	//clock_t start = clock();
	
	scanf("%d", &n);
	FOR(0, n, i){
		scanf("%d%d%s", &pt[i].x, &pt[i].y, input);
		if (input[2] == 'r') dir[i] = 0;
		if (input[2] == 's') dir[i] = 1;
		if (input[2] == 'l') dir[i] = 2;
		if (input[2] == 'j') dir[i] = 3;
	}
	
	init();
	
	while (MagS.begin()->x < oo){
		ll D = MagS.begin()->x;
		int id = MagS.begin()->y;
		vector < int > Neigh;
		for (auto t : Elem[id]){
			pair < ll, int > T = MagV[t.x].MinDisOuter(t.y);
			if (T.x != D) continue;
			Neigh.pb(T.y);
		}
		kill(id);
		for (auto x : Neigh) kill(x);
	}
	
	int Cnt = 0;
	FOR(0, n, i) if (!dead[i]){
		printf("%d\n", i + 1);
		++Cnt;
	} if (!Cnt) printf("nema\n");
	
	//printf(" # Time: %lf\n", (double)(clock() - start) / CLOCKS_PER_SEC);
	
	return 0;
}
