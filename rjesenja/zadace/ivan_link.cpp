#include <cstdio>
#include <vector>
#include <algorithm>

#define FOR(a,b,c) for (int c = (a), _for = (b); c < _for; ++c)
#define REP(n) FOR(0, n, _rep)
#define pb push_back

using namespace std;

const int oo = 1000000000;

int n, k;
int nxt[500005];
vector < int > Einv[500005];

bool Processed[500005];
int Sol = 0;

int DFS(int id){
    Processed[id] = true;
    int t = k + 1;
    for (auto x : Einv[id]) t = min(t, 1 + DFS(x));
    if (id == 0) t = 0;
    if (t == k + 1) ++Sol, t = 1;
    return t;
}

int Simuliraj(int id, vector < int > &V, int tg){
    id = *lower_bound(V.begin(), V.end(), id);
    int R = 0;
    while (id < tg){
        ++R;
        id = *lower_bound(V.begin(), V.end(), id + k);
    } return R;
}

void Process(int id){
    static bool CycBio[500005];
    while (!CycBio[id]++) id = nxt[id];
    // id je sada u ciklusu
    int pid = id;
    vector < int > Cyc;
    do {
        Cyc.pb(pid);
        pid = nxt[pid];
    } while (pid != id);
    // Cyc ima sve elemente ciklusa, prvi je id, po redu su
    static bool CycElem[500005];
    for (auto x : Cyc) CycElem[x] = true;
    vector < int > Dis;
    for (auto x : Cyc){
        Processed[x] = true;
        int t = k + 1;
        for (auto y : Einv[x]) if (!CycElem[y]) t = min(t, 1 + DFS(y));
        if (x == 0) t = 0;
        Dis.pb(t);
    } // Dis ima udaljenosti do 0
    if ((int)Cyc.size() == 1){
        if (Cyc[0] == k + 1) ++Sol;
        return;
    }
    int a = 0, b = 1;
    REP(3 * Cyc.size()){
        Dis[b] = min(Dis[b], 1 + Dis[a]);
        if (++a == (int)Cyc.size()) a = 0;
        if (++b == (int)Cyc.size()) b = 0;
    }
    vector < int > V;
    FOR(0, Cyc.size(), i) if (Dis[i] == k + 1) V.pb(i), V.pb(i + Cyc.size());
    V.pb(oo);
    sort(V.begin(), V.end());
    int R = oo;
    FOR(0, min(k, (int)Cyc.size()), i) R = min(R, Simuliraj(i, V, i + Cyc.size()));
    Sol += R;
}

int main(){
    scanf("%d%d", &n, &k);
    REP(n){
        int a, b;
        scanf("%d%d", &a, &b), --a, --b;
        nxt[a] = b;
        Einv[b].pb(a);
    }
    FOR(0, n, i) if (!Processed[i]) Process(i);
    printf("%d\n", Sol);
    return 0;
}
