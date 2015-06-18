#include "grader.h"
#include "memory.h"

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

void play(){
    vector < pii > loc(256, {-1, -1});
    FOR(1, 51, i){
        int x = faceup(i);
        loc[x].x = loc[x].y;
        loc[x].y = i;
    }
    FOR('A', 'Z', i) faceup(loc[i].x), faceup(loc[i].y);
}
