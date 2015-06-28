#include <cstdio>
#include <algorithm>
#include <vector>
#include <iostream>
#include <assert.h>
#include <stack>
#include <unistd.h>
#include <bitset>
#include <set>
#include <cstring>
#include <map>

#define FOR(a,b,c) for (int c = (int)(a); c < (int)(b); ++c)
#define REP(n,i) FOR(0,n,i)
#define pb push_back
#define pii pair < int, int >
#define x first
#define y second
#define ll long long
#define TRACE(x) cerr << #x << " = " << x << endl
#define _ << " _ " <<
#define _T1 id * 2, lo, mid
#define _T2 id * 2 + 1, mid + 1, hi
#define pll pair < ll, ll >

using namespace std;

const int dx[] = {1, 0, -1, 0};
const int dy[] = {0, 1, 0, -1};

vector < string > V;
vector < pii > Path;
int r, s;

void BuildPath(int x, int y){
    static bool bio[205][205];
    if (bio[x][y]++) return;
    Path.pb({x, y});
    REP(4,k){
        int nx = x + dx[k], ny = y + dy[k];
        if (nx < 0 || ny < 0 || nx >= r || ny >= s) continue;
        if (V[nx][ny] != '.') continue;
        BuildPath(nx, ny);
    }
}

vector < pii > Border;

bool bio[205][205];
int num[205][205];
void M2(pii pt){
    if (pt.x < 0 || pt.y < 0 || pt.x >= r || pt.y >= s) return;
    bio[pt.x][pt.y] = true;
}
void Mark(pii pt){
    M2(pt);
    REP(4,k) M2({pt.x + dx[k], pt.y + dy[k]});
}
void SetNeigh(pii pt, int v){
    REP(4,k){
        pii t = {pt.x + dx[k], pt.y + dy[k]};
        if (t.x < 0 || t.y < 0 || t.x >= r || t.y >= s) continue;
        num[t.x][t.y] = v;
    }
}

bool Bad(vector < pii > V){
    FOR(1,V.size(),i){
        int a = V[i].x - V[i - 1].x, b = V[i].y - V[i - 1].y;
        if (abs(a) + abs(b) != 1) return TRACE(i);
    } return false;
}

bool OptDFS(pii t, int id, bool zajeb = false){
    vector < pii > tmp;
    vector < int > dir = {0, 1, 2, 3};
    bool ch = true;
    int v = Path.size() + 1;
    while (ch){ch = false;
        tmp.pb(t);
        if (num[t.x][t.y]){v = num[t.x][t.y]; break;}
        bio[t.x][t.y] = true;
        random_shuffle(dir.begin(), dir.end());
        pii tt;
        for (auto k : dir){
            int x = t.x + dx[k], y = t.y + dy[k];
            if (x < 0 || y < 0 || x >= r || y >= s) continue;
            if (bio[x][y]++) continue;
            if (!ch){
                ch = true;
                tt = {x, y};
            }
        } t = tt;
    }
    //int Len = id + 1 + tmp.size() + Path.size() - v; //TRACE(Len);
    //if (Len <= (int)Path.size()) return false;
    vector < pii > New;
    REP(id,i) New.pb(Path[i]);
    for (auto x : tmp) New.pb(x);
    FOR(v-1,Path.size(),i) New.pb(Path[i]);
    //TRACE(New.size());
    if (Bad(New)){
        TRACE(id _ v _ tmp.size()); TRACE(tmp.back().x _ tmp.back().y);
        for (auto x : Path) TRACE(x.x _ x.y);
        printf("\n");
        for (auto x : New) TRACE(x.x _ x.y);
        exit(0);
    }
    if (New.size() <= Path.size() && !zajeb || zajeb && New.size() <= Path.size() * 0.9) return false;
    Path = New; //TRACE(id _ v);
    return true;
}

char in[205];
int main(int argc, char* argv[]){ TRACE(argc); assert(argc == 3);
    srand(getpid() * time(0));
    FILE *f = fopen(argv[1], "r");
    while (fscanf(f, "%s", in) == 1){
        int l = 0;
        for (; in[l] != '\0'; ++l);
        V.pb(string(in, in + l));
    } fclose(f);

    for (int i = 0; argv[2][i] != '\0'; ++i) in[i] = argv[2][i], in[i + 1] = '$', in[i + 2] = '\0';

    r = V.size(), s = V.front().size();
    REP(r,i){
        if (V[i][0] != 'X') Border.pb({i, 0});
        if (V[i][s - 1] != 'X') Border.pb({i, s - 1});
    }
    FOR(1,s-1,i){
        if (V[0][i] != 'X') Border.pb({0, i});
        if (V[r - 1][i] != 'X') Border.pb({r - 1, i});
    }
    for (auto x : Border) if (V[x.x][x.y] == '.') BuildPath(x.x, x.y); TRACE(Path.size());
    int CNT = 0;
    vector < pii > Opt;
    while (true){
        memset(bio, 0, sizeof bio);
        memset(num, 0, sizeof num);
        int id = rand() % (Path.size() + 1) - 1;
        REP(id,i) Mark(Path[i]);
        FOR(id+2,Path.size(),i) SetNeigh(Path[i], i + 1);
        REP(r,i) REP(s,j) if (V[i][j] == 'X') bio[i][j] = true;
        bool t;
        for (auto x : Border) bio[x.x][x.y] = true;
        if (id == -1) t = OptDFS(Border[rand() % Border.size()], id);
        else t = OptDFS(Path[id], id);

        if (Opt.size() < Path.size()){ TRACE(Path.size()); CNT = 0; Opt = Path;
            for (auto &x : V) for (auto &y : x) if (y == '.') y = '#';
            for (auto x : Path) V[x.x][x.y] = '.';

            f = fopen(argv[2], "w");
            REP(r,i){
                REP(s,j) fprintf(f, "%c", V[i][j]);
                fprintf(f, "\n");
            } fclose(f); //for (auto x : Path) TRACE(x.x _ x.y); static int DEBUG = 0; if (++DEBUG == 2) return 0;

            f = fopen(in, "w");
            REP(r,i){
                REP(s,j) fprintf(f, "%c", V[i][j]);
                fprintf(f, "\n");
            } fclose(f);
        } else ++CNT;

        if (CNT >= 10000){ CNT = 0; cerr << "New attempt\n"; Path.clear(); continue;
            /*cerr << "worsening ...\n";
            CNT = 0;
            REP(50,iii){
                memset(bio, 0, sizeof bio);
                memset(num, 0, sizeof num);
                int id = rand() % (Path.size() + 1) - 1;
                REP(id,i) Mark(Path[i]);
                FOR(id+2,Path.size(),i) SetNeigh(Path[i], i + 1);
                REP(r,i) REP(s,j) if (V[i][j] == 'X') bio[i][j] = true;
                for (auto x : Border) bio[x.x][x.y] = true;
                if (id == -1) OptDFS(Border[rand() % Border.size()], id, true);
                else OptDFS(Path[id], id, true);
            } TRACE(Path.size());*/
        }
    }

    return 0;
}
