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

class vektor{
public:
	int* V;
	int sz;
	vektor() : sz(0){}
	int size(){return sz;}
	void resize(){V = new int[sz + 5], sz = 0;}
	void push_back(int x){V[sz++] = x;}
	void pop_back(){--sz;}
	int* begin(){return V;}
	int* end(){return V + sz;}
	int& operator[](int x){return V[x];}
};

const int oo = 1000000000;
const ll ool = (ll)oo * oo;

int n, m;
int b[30005], p[30005];
bool cmp(int a, int b){return p[a] < p[b];}

vektor DV[30005];
int dis[30005];
void dijkstra(){
	for (auto &x : dis) x = oo;
	int t = b[0];
	dis[t] = 0;
	set < pii > S;
	S.insert({0, t});
	while (!S.empty()){
		t = S.begin()->y;
		S.erase(S.begin());
		int prev = -1;
		for (auto x : DV[t]){
			if (p[x] == prev) continue;
			for (int pos = t + p[x], d = dis[t] + 1; pos < n; pos += p[x], ++d)
				if (dis[pos] > d){
					S.erase({dis[pos], pos});
					dis[pos] = d;
					S.insert({dis[pos], pos});
				}
			for (int pos = t - p[x], d = dis[t] + 1; pos >= 0; pos -= p[x], ++d)
				if (dis[pos] > d){
					S.erase({dis[pos], pos});
					dis[pos] = d;
					S.insert({dis[pos], pos});
				}
			prev = p[x];
		}
	}
}
void Solve1(){
	FOR(0, m, i) ++DV[b[i]].sz;
	FOR(0, n, i) DV[i].resize();
	FOR(0, m, i) DV[b[i]].pb(i);
	FOR(0, n, i) sort(all(DV[i]), cmp);
	dijkstra();
	if (dis[b[1]] == oo) printf("-1\n"); else printf("%d\n", dis[b[1]]);
}

int main(){
	scanf("%d%d", &n, &m);
	FOR(0, m, i) scanf("%d%d", b + i, p + i);
	Solve1();
	return 0;
}
