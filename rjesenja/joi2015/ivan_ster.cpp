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

const int offset = (1 << 17);
class sparse{
public:
    class node{
    public:
        node *l, *r;
        int val;
        int Sval;
        node(int _val) : l(NULL), r(NULL), val(_val), Sval(_val){}
    } *Root = NULL;
    int Val(node *t){if (t == NULL) return 0; return t->Sval;}
    void Update(node *t){t->Sval = Val(t->l) + Val(t->r) + t->val;}
    void Set(node *&t, int lo, int hi, int p, int v){
        if (lo > p || hi < p) return;
        if (t == NULL) t = new node(0);
        if (lo == hi){
            t->val = t-> Sval = v;
            return;
        }
        int mid = (lo + hi) / 2;
        Set(t->l, lo, mid, p, v);
        Set(t->r, mid + 1, hi, p, v);
        Update(t);
    }
    int Query(node *t, int lo, int hi, int slo, int shi){
        if (t == NULL) return 0;
        if (lo > shi || hi < slo) return 0;
        if (lo >= slo && hi <= shi) return Val(t);
        int mid = (lo + hi) / 2;
        return Query(t->l, lo, mid, slo, shi) + Query(t->r, mid + 1, hi, slo, shi);
    }
    void Set(int p, int v){Set(Root, 0, offset - 1, p, v);}
    int Query(int lo, int hi){return Query(Root, 0, offset - 1, lo, hi);}
    void NSet(node *&t, int lo, int hi, int slo, int shi){
        if (lo > slo || hi < shi) return;
        if (t == NULL) t = new node(0);
        if (lo == slo && hi == shi) return;
        int mid = (lo + hi) / 2;
        NSet(t->l, lo, mid, slo, shi);
        NSet(t->r, mid + 1, hi, slo, shi);
    }
    void NSet(int lo, int hi){NSet(Root, 0, offset - 1, lo, hi);}

    void GetAll(node *&t, int lo, int hi, int slo, int shi, vector < pair < node*, pii > > &out){
        if (lo > shi || hi < slo) return;
        if (t == NULL) return;
        if (lo >= slo && hi <= shi){
            out.pb({t, {lo, hi}});
            return;
        }
        int mid = (lo + hi) / 2;
        GetAll(t->l, lo, mid, slo, shi, out);
        GetAll(t->r, mid + 1, hi, slo, shi, out);
    }
    void GetAll(int lo, int hi, vector < pair < node*, pii > > &out){GetAll(Root, 0, offset - 1, lo, hi, out);}
    void ForceUpdate(node *&t, int lo, int hi, int slo, int shi){
        if (lo > shi || hi < slo) return;
        if (lo >= slo && hi <= shi) return;
        if (t == NULL) return;
        int mid = (lo + hi) / 2;
        ForceUpdate(t->l, lo, mid, slo, shi);
        ForceUpdate(t->r, mid + 1, hi, slo, shi);
        Update(t);
    }
    void ForceUpdate(int lo, int hi){ForceUpdate(Root, 0, offset - 1, lo, hi);}
} T[50];

int Cnt = 0;

int n, q, k;

void Set(int p, int v){
    for (int i = Cnt; i < Cnt + 35; ++i, v /= k) T[i].Set(p, v % k);
}

ll Query(int lo, int hi){
    ll R = 0;
    ll Rc = 1;
    FOR(Cnt, Cnt + 35, i) R += Rc * T[i].Query(lo, hi), Rc *= k;
    return R;
}

void Del(sparse::node *t){
    if (t == NULL) return;
    Del(t->l), Del(t->r);
    delete t;
}

void Shift(int lo, int hi){
    vector < pair < sparse::node*, pii > > V;
    T[0].GetAll(lo, hi, V);
    for (auto x : V) FOR(0,35,i) T[i].NSet(x.y.x, x.y.y);
    vector < pair < sparse::node*, pii > > Vl[35];
    Vl[0] = V;
    FOR(1, 35, i) T[i].GetAll(lo, hi, Vl[i]);
    FOR(0, 34, i){
        FOR(0, V.size(), j)
            swap(*Vl[i][j].x, *Vl[i + 1][j].x);
    }
    FOR(0, V.size(), i){
        if (Vl[34][i].x == NULL) continue;
        Del(Vl[34][i].x->l);
        Del(Vl[34][i].x->r);
        *Vl[34][i].x = sparse::node(0);
    }
    FOR(0, 35, i) T[i].ForceUpdate(lo, hi);
}

ll loga[100005];
int a[100005];
void add(int p, int v){ ++p;
    for (; p < 100005; p += p & -p) loga[p] += v;
}
ll sum(int p){ ++p;
    ll r = 0;
    for (; p; p -= p & -p) r += loga[p];
    return r;
}
ll query(int lo, int hi){
    return sum(hi) - sum(lo - 1);
}

void Stupid(){
    FOR(0, n, i){
        scanf("%d", a + i);
        add(i, a[i]);
    }
    REP(q){
        int s, t, u;
        scanf("%d%d%d", &s, &t, &u);
        if (s == 1) add(t - 1, u - a[t - 1]), a[t - 1] = u;
        //if (s == 2) ++Cnt;
        if (s == 3) printf("%lld\n", query(t - 1, u - 1));
    }
}

int main(){
    scanf("%d%d%d", &n, &q, &k);
    if (k == 1){Stupid(); return 0;}
    FOR(0, n, i){
        int x;
        scanf("%d", &x);
        Set(i, x);
    } //printf(" -> %lld\n", Query(1, 3));
    REP(q){
        int s, t, u;
        scanf("%d%d%d", &s, &t, &u);
        if (s == 1) Set(t - 1, u);
        if (s == 2) Shift(t - 1, u - 1);
        if (s == 3) printf("%lld\n", Query(t - 1, u - 1));
    }
    return 0;
}
