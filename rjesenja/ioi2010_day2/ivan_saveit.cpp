#include "grader.h"
#include "encoder.h"
#include "decoder.h"

#include <cstdio>
#include <vector>
#include <algorithm>
#include <string>
#include <cstring>
#include <iostream>
#include <unistd.h>
#include <queue>

#define FOR(a,b,c) for (int c = (a), _for (b); c < _for; ++c)
#define REP(n) FOR(0,n,_rep)
#define pb push_back
#define pii pair < int, int >
#define x first
#define y second
#define ll long long

using namespace std;

// No communication/shared data between two namespaces please.
namespace Encoder {
// Your encoder implementation goes here.
const int oo = 1000000000;

void WriteNode(int a){
    REP(10){
        encode_bit(a & 1);
        a >>= 1;
    }
}
void WriteEdge(int a, int b){
    WriteNode(a), WriteNode(b);
}
vector < int > E[1005];
vector < pii > Tree;
bool bio[1005];
int par[1005];
void DFS(int id, int p = 1005){
    par[id] = p;
    bio[id] = true;
    for (auto x : E[id]) if (!bio[x]){
        Tree.pb({id, x});
        DFS(x, id);
    }
}
int dis[1005];
void BFS(int id){
    for (auto &x : dis) x = oo;
    dis[id] = 0;
    queue < int > Q;
    Q.push(id);
    while (!Q.empty()){
        id = Q.front(); Q.pop();
        int d = dis[id];
        for (auto x : E[id]) if (dis[x] > d + 1){
            dis[x] = d + 1;
            Q.push(x);
        }
    }
}
void WriteEnc(int t){
    REP(8){
        encode_bit(t & 1);
        t >>= 1;
    }
}
bool cmp(pii a, pii b){return a.y < b.y;}
void encode(int nv, int nh, int ne, int *v1, int *v2){
    FOR(0, ne, i) E[v1[i]].pb(v2[i]), E[v2[i]].pb(v1[i]);
    DFS(0);
    //for (auto x : Tree) WriteEdge(x.x, x.y);
    sort(Tree.begin(), Tree.end(), cmp);
    FOR(0, nv, i) WriteNode(par[i]);
    FOR(0, nh, i){
        BFS(i);
        vector < int > CH;
        //if (i == 0) FOR(0, nv, j) printf("%d %d - %d\n", i, j, dis[j]);
        for (auto x : Tree) CH.pb(dis[x.x] - dis[x.y]);
        while (CH.size() % 5 != 0) CH.pb(0);
        int num = 0;
        FOR(0, CH.size(), ii){ //if (i == 0) printf("%d ", CH[ii]);
            num *= 3;
            num += CH[ii] + 1;
            if (ii % 5 == 4) WriteEnc(num), num = 0;
        } //if (i == 0) printf("\n");
    }
}
};

// No communication/shared data between two namespaces please.
namespace Decoder {
// Your decoder implementation goes here.
int GetNode(){
    int r = 0;
    FOR(0, 10, i) r |= (decode_bit() << i);
    return r;
}
pii GetEdge(){return {GetNode(), GetNode()};}
vector < int > GetEnc(){
    int t = 0;
    vector < int > r;
    FOR(0, 8, i) t |= (decode_bit() << i);
    REP(5){
        r.pb((t % 3) - 1);
        t /= 3;
    } reverse(r.begin(), r.end());
    return r;
}
vector < pii > Tree;
vector < pii > E[1005];
int H;
void DFS(int id, int p, int d){
    //printf(" -> %d %d - %d\n", H, id, d);
    hops(H, id, d);
    for (auto x : E[id]) if (x.x != p) DFS(x.x, id, d - x.y);
}
void decode(int nv, int nh) {
    FOR(0, nv, i){
        int x = GetNode();
        if (x < nv) Tree.pb({x, i});
    }
    FOR(0, nh, i){ H = i;
        for (auto &x : E) x.clear();
        vector < int > d;
        REP((Tree.size() + 4) / 5){
            vector < int > dd = GetEnc();
            for (auto x : dd) d.pb(x);//, printf("%d ", x);
        } //printf("\n");
        FOR(0, Tree.size(), i){
            int x = Tree[i].x, y = Tree[i].y, D = d[i];
            E[x].pb({y, D}), E[y].pb({x, -D});
        }
        DFS(i, i, 0);
    }
}
};

// Called by grader. Do not modify.
void encode(int nv, int nh, int ne, int *v1, int *v2) {
  Encoder::encode(nv, nh, ne, v1, v2);
}
void decode(int nv, int nh) {
  Decoder::decode(nv, nh);
}
