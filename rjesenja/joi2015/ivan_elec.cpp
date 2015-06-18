#include <cstdio>
#include <vector>
#include <algorithm>
#include <unistd.h>

#define FOR(a,b,c) for (int c = (a), _for = (b); c < _for; ++c)
#define REP(n) FOR(0,n,_rep)
#define pb push_back
#define ll long long
#define pii pair < int, int >
#define x first
#define y second

using namespace std;

int n;
vector < int > E[100005];
int m;
vector < pair < pii, int > > V;
vector < int > E2[100005];
int dis[100005], fin[100005];
vector < pair < pii, int > > VE[100005];
int dep[100005];
int LCA[20][100005];
int lca(int a, int b){
    if (dep[a] < dep[b]) swap(a, b);
    int t = dep[a] - dep[b];
    FOR(0,20,i) if ((t >> i) & 1) a = LCA[i][a];
    for (int i = 19; i >= 0; --i) if (LCA[i][a] != LCA[i][b]) a = LCA[i][a], b = LCA[i][b];
    if (a != b) return LCA[0][a];
    return a;
}

void Rootaj(int id, int p){
    LCA[0][id] = p;
    static int Cnt = 0;
    dis[id] = Cnt++;
    for (auto x : E[id]) if (x != p){
        E2[id].pb(x);
        dep[x] = dep[id] + 1;
        Rootaj(x, id);
    } fin[id] = Cnt - 1;
}

ll loga[100005];
void add(int p, int v){ ++p;
    for (; p < 100005; p += p & -p)
        loga[p] += v;
}
void add(int lo, int hi, int v){add(lo, v); add(hi + 1, -v);}
ll sum(int p){ ++p;
    ll r = 0;
    for (; p; p -= p & -p)
        r += loga[p];
    return r;
}

ll Val[100005];

void DFS(int id){
    for (auto x : E2[id]) DFS(x);
    for (auto x : E2[id]){
        add(dis[id], fin[id], Val[x]);
        add(dis[x], fin[x], -Val[x]);
    }
    Val[id] = sum(dis[id]);
    for (auto e : VE[id]){
        int x = e.x.x, y = e.x.y;
        ll c = e.y;
        Val[id] = max(Val[id], sum(dis[x]) + sum(dis[y]) - sum(dis[id]) + c);
    }
}

int main(){
    scanf("%d", &n);
    REP(n - 1){
        int x, y;
        scanf("%d%d", &x, &y), --x, --y;
        E[x].pb(y), E[y].pb(x);
    }
    scanf("%d", &m);
    REP(m){
        int a, b, c;
        scanf("%d%d%d", &a, &b, &c), --a, --b;
        V.pb({{a, b}, c});
    }
    Rootaj(0, 0);
    FOR(1,20,x) FOR(0,n,i) LCA[x][i] = LCA[x - 1][LCA[x - 1][i]];
    for (auto x : V) VE[lca(x.x.x, x.x.y)].pb(x);
    DFS(0);
    printf("%lld\n", Val[0]);
    //FOR(0,n-1,i) printf(" -> %lld (%lld)\n", Val[i], sum(dis[i]));
    //for (auto x : VE[4]) printf("%d %d %d\n", x.x.x, x.x.y, x.y);
    //printf(" LCA(%d,%d) = %d\n", 4, 6, lca(4, 6));
    return 0;
}
