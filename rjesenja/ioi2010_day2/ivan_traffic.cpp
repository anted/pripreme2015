#include "traffic.h"

#include <cstdio>
#include <vector>
#include <algorithm>

#define FOR(a,b,c) for (int c = (a), _for (b); c < _for; ++c)
#define REP(n) FOR(0,n,_rep)
#define pb push_back
#define pii pair < int, int >
#define x first
#define y second
#define ll long long

using namespace std;

vector < int > E[1000005];
int n;
ll w[1000005];

ll sz[1000005];
ll msz[1000005];
void DFS(int id, int p){
    sz[id] = w[id];
    for (auto x : E[id]) if (x != p){
        DFS(x, id);
        sz[id] += sz[x];
        msz[id] = max(msz[id], sz[x]);
    }
}

int GetCentroid(int id){
    DFS(id, id);
    FOR(0, n, i) msz[i] = max(msz[i], sz[id] - sz[i]);
    int r = id;
    FOR(0, n, i) if (msz[i] < msz[r]) r = i;
    return r;
}

int LocateCentre(int N, int pp[], int S[], int D[]) {
   n = N;
   FOR(0, n - 1, i) E[S[i]].pb(D[i]), E[D[i]].pb(S[i]);
   FOR(0, n, i) w[i] = pp[i];
   return GetCentroid(0);
}
