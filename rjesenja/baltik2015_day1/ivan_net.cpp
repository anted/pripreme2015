#include <cstdio>
#include <algorithm>
#include <vector>
#include <set>

#define FOR(a,b,c) for (int c = (a), _for = (b); c < _for; ++c)
#define REP(n) FOR(0,n,_rep)
#define pb push_back
#define ll long long
#define x first
#define y second
#define pii pair < int, int >

using namespace std;

int loga[1000005];
void Add(int p, int v){ ++p;
    for (; p < 1000005; p += p & -p)
        loga[p] += v;
}
int Sum(int p){ ++p;
    int r = 0;
    for (; p; p -= p & -p)
        r += loga[p];
    return r;
}
int Sum(int lo, int hi){return Sum(hi) - Sum(lo - 1);}

int n;
vector < int > E[500005];
int dep[500005], dis[500005], fin[500005];
int dinv[500005];
int Root;
vector < int > E2[500005];
set < int > Alive;

void Rootaj(int id, int p){
    static int cnt = 0;
    dinv[cnt] = id;
    dis[id] = cnt++;
    dep[id] = dep[p] + 1;
    for (auto x : E[id]) if (x != p) Rootaj(x, id), E2[id].pb(x);
    fin[id] = cnt - 1;
}

void GenSz(){
    FOR(1, n + 1, i) if (E2[i].size() == 0) Add(dis[i], 1);
}

int Size(int id){return Sum(dis[id], fin[id]);}

void Att(int a, int b){
    auto it1 = Alive.lower_bound(dis[a]), it2 = Alive.lower_bound(dis[b]);
    printf("%d %d\n", dinv[*it1], dinv[*it2]);
    Alive.erase(it1);
    Alive.erase(it2);
}

void Solver(int id){
    int csz = Size(id);
    if (csz == 0) return;
    vector < pii > V;
    for (auto x : E2[id]) V.pb({Size(x), x});
    sort(V.begin(), V.end());
    reverse(V.begin(), V.end());
    while (V.size() > 0 && V.back().x == 0) V.pop_back();
    while (V.size() > 2 + (id == Root)){
        int a = V.back().y, b = V[V.size() - 2].y;
        Att(a, b);
        --V.back().x;
        --V[V.size() - 2].x;
        while (V.size() > 0 && V.back().x == 0) V.pop_back();
    }
    for (int i = 0; i < (int)E2[id].size(); ++i){
        int &x = E2[id][i];
        Solver(x);
        if (Size(x) == 0){
            swap(x, E2[id].back());
            E2[id].pop_back();
            --i;
        }
    }
}

int main(){
    scanf("%d", &n);
    REP(n - 1){
        int a, b;
        scanf("%d%d", &a, &b);
        E[a].pb(b), E[b].pb(a);
    }
    FOR(1, n + 1, i) if (E[i].size() > 1){Rootaj(i, i); Root = i; break;}
    int k = 0;
    FOR(1, n + 1, i) if (E[i].size() == 1) ++k;
    printf("%d\n", (k + 1) / 2);
    //GenSz();
    FOR(1, n + 1, i) if (E2[i].size() == 0) Alive.insert(dis[i]);
    //Solver(Root);

    vector < int > V;
    for (auto x : Alive) V.pb(x);// printf("%d\n", dinv[x]);

    FOR(0, k / 2, i) printf("%d %d\n", dinv[V[i]], dinv[V[i + (k + 1) / 2]]);
    if (k & 1) printf("%d %d\n", Root, dinv[V[k / 2]]);

    return 0;
}
