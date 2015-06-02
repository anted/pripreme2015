#include <cstdio>
#include <algorithm>
#include <vector>
#include <set>
#include <queue>

#define FOR(a,b,c) for (int c = (a), _for = (b); c < _for; ++c)
#define REP(n) for (int _rep = 0, _for = (n); _rep < _for; ++_rep)
#define pb push_back
#define x first
#define y second
#define ll long long
#define pii pair < int, int >

using namespace std;

int n, m;
int r[105];
int w[2005];

int pos[2005];

int main(){
	scanf("%d%d", &n, &m);
	FOR(0, n, i) scanf("%d", r + i);
	FOR(1, m + 1, i) scanf("%d", w + i);
	
	set < int > S;
	FOR(0, n, i) S.insert(i);
	queue < int > Q;
	
	ll R = 0;
	REP(m * 2){
		int t;
		scanf("%d", &t);
		if (t > 0) Q.push(t);
		else S.insert(pos[-t]);
		while (Q.size() && S.size()){
			int a = Q.front(), b = *S.begin();
			Q.pop(), S.erase(S.begin());
			pos[a] = b;
			R += w[a] * r[b];
		}
	} printf("%lld\n", R);
	
	return 0;
}
