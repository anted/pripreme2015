#include "grader.h"
#include "cluedo.h"

#include <vector>
#include <cstdio>

#define FOR(a,b,c) for (int c = (a), _for = (b); c < _for; ++c)
#define REP(n) FOR(0, n, _rep)
#define pb push_back

using namespace std;

void Solve(){
	vector < int > V[3];
	FOR(1, 7, i) V[0].pb(i);
	FOR(1, 11, i) V[1].pb(i);
	FOR(1, 7, i) V[2].pb(i);
	REP(6 + 10 + 6 - 3 + 1){
		int x = Theory(V[0].back(), V[1].back(), V[2].back()) - 1;
		if (x == -1) return;
		V[x].pop_back();
	}
}
