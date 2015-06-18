#include <cstdio>
#include <algorithm>
#include <vector>
#include <set>
#include <cstring>
#include <string>
#include <iostream>

#define FOR(a,b,c) for (int c = (a), _for = (b); c < _for; ++c)
#define REP(n) FOR(0,n,_rep)
#define pb push_back
#define ll long long
#define x first
#define y second
#define pii pair < int, int >

using namespace std;

const int UND = 301;
const int BAD = 302;

int n;
char in[1005];
int a[1005];
vector < string > v;

string s0, s1, s2;

bool Good(string &a, string &b){
    if (a[0] != b[0] && a[0] != '?') return false;
    if (a[1] != b[1] && a[1] != '?') return false;
    return true;
}

ll memo[10][305][11][11];
bool bio[10][305][11][11];
ll dp(int p, int sm, int a1, int a2){ //printf(" -> %d %d %d %d\n", p, sm, a1, a2);
    if (sm == BAD || sm < 0) return 0;
    if (p == -1) return sm == 0 || sm == UND;
    if (sm == UND && a[p] != -1) sm = a[p];
    if (a[p] != -1 && sm != a[p]) return 0;

    ll &r = memo[p][sm][a1][a2];
    if (bio[p][sm][a1][a2]++) return r;
    r = 0;

    if (Good(v[p], s0)){
        int sm2 = sm - 10 - a1 - a2;
        if (sm == UND) sm2 = UND;
        else if (sm2 < 0) sm2 = BAD;
        r += dp(p - 1, sm2, 10, a1);
    }

    FOR(0, 10, t){
        s1[0] = t + '0';
        if (Good(v[p], s1)){
            int sm2 = sm - 10 - a1;
            if (sm == UND) sm2 = UND;
            else if (sm2 < 0) sm2 = BAD;
            r += dp(p - 1, sm2, t, 10 - t);
        }
    }

    FOR(0, 10, t1) FOR(0, 10 - t1, t2){
        s2[0] = t1 + '0';
        s2[1] = t2 + '0';
        if (Good(v[p], s2)){
            int sm2 = sm - t1 - t2;
            if (sm == UND) sm2 = UND;
            else if (sm2 < 0) sm2 = BAD;
            r += dp(p - 1, sm2, t1, t2);
        }
    }

    return r;

}

bool G2(string a, string b){
    if (a[0] != b[0] && a[0] != '?') return false;
    if (a[1] != b[1] && a[1] != '?') return false;
    if (a[2] != b[2] && a[2] != '?') return false;
    return true;
}

ll Res(string s){
    //cout << s << endl;
    ll r = 0;
    if (a[n - 1] == -1){
        if (G2(s, "xxx")) r += dp(n - 2, UND, 10, 10);
        string ss;
        ss = "xx0"; FOR(0, 10, t){
            ss[2] = t + '0';
            if (G2(s, ss)) r += dp(n - 2, UND, 10, 10);
        }
        ss = "x0/"; FOR(0, 10, t){
            ss[1] = t + '0';
            if (G2(s, ss)) r += dp(n - 2, UND, 10, t);
        }
        ss = "x00"; FOR(0, 10, t1) FOR(0, 10 - t1, t2){
            ss[1] = t1 + '0';
            ss[2] = t2 + '0';
            if (G2(s, ss)) r += dp(n - 2, UND, 10, t1);
        }
        ss = "0/x"; FOR(0, 10, t){
            ss[0] = t + '0';
            if (G2(s, ss)) r += dp(n - 2, UND, t, 10 - t);
        }
        ss = "0/0"; FOR(0, 10, t1) FOR(0, 10, t2){
            ss[0] = t1 + '0';
            ss[2] = t2 + '0';
            if (G2(s, ss)) r += dp(n - 2, UND, t1, 10 - t1);
        }
        ss = "00-"; FOR(0, 10, t1) FOR(0, 10 - t1, t2){
            ss[0] = t1 + '0';
            ss[1] = t2 + '0';
            if (G2(s, ss)) r += dp(n - 2, UND, t1, t2);
        }
    }
    else {
        int A = a[n - 1];
        if (G2(s, "xxx")) r += dp(n - 2, A - 30, 10, 10);
        string ss;
        ss = "xx0"; FOR(0, 10, t){
            ss[2] = t + '0';
            if (G2(s, ss)) r += dp(n - 2, A - 20 - t, 10, 10);
        }
        ss = "x0/"; FOR(0, 10, t){
            ss[1] = t + '0';
            if (G2(s, ss)) r += dp(n - 2, A - 20, 10, t);
        }
        ss = "x00"; FOR(0, 10, t1) FOR(0, 10 - t1, t2){
            ss[1] = t1 + '0';
            ss[2] = t2 + '0';
            if (G2(s, ss)) r += dp(n - 2, A - 10 - t1 - t2, 10, t1);
        }
        ss = "0/x"; FOR(0, 10, t){
            ss[0] = t + '0';
            if (G2(s, ss)) r += dp(n - 2, A - 20, t, 10 - t);
        }
        ss = "0/0"; FOR(0, 10, t1) FOR(0, 10, t2){
            ss[0] = t1 + '0';
            ss[2] = t2 + '0';
            if (G2(s, ss)) r += dp(n - 2, A - 10 - t2, t1, 10 - t1);
        }
        ss = "00-"; FOR(0, 10, t1) FOR(0, 10 - t1, t2){
            ss[0] = t1 + '0';
            ss[1] = t2 + '0';
            if (G2(s, ss)) r += dp(n - 2, A - t1 - t2, t1, t2);
        }
    }
    return r;
}

void Solve(){
    memset(bio, 0, sizeof bio);
    scanf("%d", &n);
    scanf("%s", in);
    v.clear();
    FOR(0,n,i) scanf("%d", a + i);
    FOR(0,n-1,i) v.pb(string(in + 2 * i, in + 2 * i + 2));
    printf("%lld\n", Res(string(in + 2 * n - 2, in + 2 * n + 1)));
}

int main(){
    s0 = "x-";
    s1 = "0/";
    s2 = "00";
    int q;
    scanf("%d", &q);
    REP(q) Solve();
    return 0;
}
