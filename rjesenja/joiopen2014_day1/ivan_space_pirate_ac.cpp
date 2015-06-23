#include <cstdio>
#include <algorithm>
#include <vector>
#include <iostream>
#include <assert.h>
#include <stack>

#define FOR(a,b,c) for (int c = (int)(a); c < (int)(b); ++c)
#define REP(n,i) FOR(0,n,i)
#define pb push_back
#define pii pair < int, int >
#define x first
#define y second
#define ll long long
#define TRACE(x) cerr << #x << " = " << x << endl

using namespace std;

int n;
ll k;
int a[100005];
ll out[100005];

bool mb[100005];

void SolveCycle(){
    int t = 0;
    vector < int > MC;
    do {
        MC.pb(t);
        t = a[t];
    } while (t != 0);
    for (auto x : MC) mb[x] = true;
    FOR(0,n,i) if (!mb[i]) out[i] = MC.size();
    out[MC[k % MC.size()]] = (ll)n * (n - MC.size());
    FOR(1,MC.size()+1,l){ // cycle length
        int id = 0;
        int cm = k % l;
        out[MC[MC.size() - l + cm]] += cm;
        id = cm;
        for (; id < (int)MC.size(); id += l) out[MC[id]] += min(l, (int)MC.size() - id);
    }
    FOR(0,n,i) printf("%lld\n", out[i]);
}

int bio[3005];
bool cyc[3005];
void DFS(int id){
    stack < int > S;
    while (true){
        if (bio[id] == 2){
            while (S.size()) bio[S.top()] = 2, S.pop();
            return;
        }
        if (bio[id] == 1) break;
        S.push(id);
        bio[id] = 1;
        id = a[id];
    }
    while (S.top() != id) bio[S.top()] = 2, cyc[S.top()] = true, S.pop();
    bio[id] = 2, cyc[id] = true, S.pop();
    while (S.size()) bio[S.top()] = 2, S.pop();
}
pii CycPar[3005];
int LCA[65][3005];
void DFS2(int id){
    if (CycPar[id].x) return;
    if (cyc[id]){CycPar[id] = {id, 0}; LCA[0][id] = a[id]; return;}
    DFS2(a[id]);
    LCA[0][id] = a[id];
    CycPar[id] = {CycPar[a[id]].x, CycPar[a[id]].y + 1};
}
int Lift(int a, int d){
    REP(15,i) if ((d >> i) & 1) a = LCA[i][a];
    return a;
}
bool Disjoint(int a, int b){
    int del = CycPar[a].y - CycPar[b].y;
    if (del < 0) return true;
    a = Lift(a, del);
    return a != b;
}
vector < vector < int > > Cycles;
pii Loc[3005];
void BuildCycles(){
    static bool bio[3005];
    REP(n,i) if (cyc[i] && !bio[i]){
        int Color = Cycles.size();
        Cycles.pb({});
        vector < int > &cCycle = Cycles.back();
        int t = i;
        do {
            Loc[t] = {Color, cCycle.size()};
            cCycle.pb(t);
            t = a[t];
        } while (t != i);
    }
}

void SolvePhase3(int t, ll sh){
    sh -= CycPar[t].y;
    t = CycPar[t].x;
    sh %= Cycles[Loc[t].x].size();
    ++out[Lift(t, sh)];
}

void SolvePhase2(int a, int b, ll sh){
    if (!cyc[a]){
        if (Disjoint(b, a)) SolvePhase3(b, sh - 1);
        else {
            int Len = CycPar[b].y - CycPar[a].y + 1;
            --sh;
            sh %= Len;
            int t = Lift(b, sh);
            ++out[t];
            return;
        }
    }
    else {
        if (Loc[a].x != Loc[CycPar[b].x].x) SolvePhase3(b, sh - 1);
        else {
            --sh;
            int Len = CycPar[b].y;
            int t = CycPar[b].x;
            int Dis = Loc[a].y - Loc[t].y;
            if (Dis < 0) Dis += Cycles[Loc[a].x].size();
            Len += Dis + 1;
            sh %= Len;
            ++out[Lift(b, sh)];
        }
    }
}

void SolvePseudo(){
    assert(n <= 3000);
    REP(n,i) DFS(i);
    REP(n,i) DFS2(i);
    BuildCycles();
    FOR(1,60,i) REP(n,j) LCA[i][j] = LCA[i - 1][LCA[i - 1][j]];
    REP(n,a) REP(n,b){
        int c = 0;
        ll sh = k;
        if (Disjoint(c, a)) sh -= CycPar[c].y, c = CycPar[c].x;
        else {SolvePhase2(a, b, sh - CycPar[c].y + CycPar[a].y); continue;}

        if (!cyc[a] || cyc[a] && Loc[a].x != Loc[c].x){
            ++out[Cycles[Loc[c].x][(Loc[c].y + sh) % Cycles[Loc[c].x].size()]];
            continue;
        }

        int T = Loc[a].y - Loc[c].y;
        if (T < 0) T += Cycles[Loc[a].x].size();
        sh -= T;
        SolvePhase2(a, b, sh);
    }
    REP(n,i) printf("%lld\n", out[i]);
}

int main(){
    scanf("%d%lld", &n, &k);
    static bool Bio[100005];
    FOR(0,n,i) scanf("%d", a + i), --a[i], ++Bio[a[i]];
    int Cnt = 0;
    for (auto x : Bio) Cnt += x;
    if (Cnt == n) SolveCycle();
    else SolvePseudo();
    return 0;
}
