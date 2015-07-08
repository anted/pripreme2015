#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <iostream>
#include <algorithm>
#include <vector>
#include <cassert>
#include <ctime>
#include <set>
#include <map>
#include <queue>
using namespace std;

#define REP(i, n) for (int i = 0; i < (n); ++i)
#define ALL(x) x.begin(), x.end()

typedef long long llint;

const int MAXW = 210;
const int MAXN = 10010;
const double e = 2.7182818;

#define MAX_TS_ITERS 200
#define MAX_TS_SIZE  15
#define MAX_SA_ITERS 300
#define SA_COEF      0.97
#define SA_START_T   3
#define TS_START_NUM 15

#define MAX_PM 300
#define STEP_T 5000
#define MAX_T  15000000

int N;        // ulazni blokovi
int K;        // konacni blokovi
int R, C;     // dimenzije

int GRID[MAXW][MAXW];
int P[MAXN];  // cijena

vector<int> V[MAXN];  // susjedstva

char INPUT_FILE[128];
char OUTPUT_DIR[128];
char OUTPUT_FILE[128];

double Random() {
  return (rand() & ((1<<30) - 1)) / (double)(1<<30);
}

struct UF {
  int dad[MAXN];

  UF() {
    memset(dad, -1, sizeof dad);
  }

  int find(int x) {
    if (dad[x] == -1) return x;
    return dad[x] = find(dad[x]);
  }

  bool join(int x, int y) {
    x = find(x);
    y = find(y);
    if (x == y) return false;
    if (rand() & 1) dad[x] = y;
    else dad[y] = x;
    return true;
  }
};

struct State {
  int ans[MAXN];
  double score; 
};


// radi tako da pocnem spajati zajedno manje dijelove
// i postepeno idem prema sve vecim dijelovima, time
// mi na kraju omjer dodje oko 2
State build_random() {
  State ret;
  UF uf;

  REP(i, N) ret.ans[i] = i;

  static vector<int> v[MAXN];
  REP(i, N) {
    v[i] = V[i];
    random_shuffle(v[i].begin(), v[i].end());
  }

  static int sc[MAXN];
  REP(i, N) sc[i] = P[i] - MAX_PM + rand() % (2 * MAX_PM);

  auto f = [&](int p, int a, int b) {
    static vector<int> tmp;
    tmp.clear();
    for (int it : v[a]) 
      if (uf.find(it) != p)
        tmp.push_back(it);
    for (int it : v[b]) 
      if (uf.find(it) != p)
        tmp.push_back(it);
    sort(ALL(tmp));
    tmp.resize(unique(ALL(tmp)) - tmp.begin());
    random_shuffle(ALL(tmp));
    v[p] = tmp;
  };

  int sz = N;
  for (int it0 = 0; it0 < 1 && sz > K; ++it0) {
    for (int T = 0; T < MAX_T && sz > K; T += STEP_T) {
      for(int iter = 0; iter < 500 && sz > K; ++iter) {
        int a = uf.find(rand() % N);
        if (sc[a] > T) continue;
        for (int x: v[a]) {
          int b = uf.find(x);
          if (a == b) continue;
          if (sc[a] + sc[b] > T) continue;
          uf.join(a, b);
          int na = uf.find(a);
          sc[na] = sc[a] + sc[b];
          f(na, a, b);
          --sz;
          break;
        }
      }
    }
  }

  assert(sz == K);

  int c = 0;
  static int bio[MAXN];
  memset(bio, 0, sizeof bio);
  for (int i = 0; i < N; ++i) {
    int x = uf.find(i);
    if (bio[x] == 0) bio[x] = ++c;
    ret.ans[i] = bio[x] - 1;
  }
 
  return ret;
}

double eval(const State& state) {
  static int sum[MAXN];
  for (int i = 0; i < K; ++i) sum[i] = 0;
  for (int i = 0; i < N; ++i)
    sum[state.ans[i]] += P[i];
  int mn = 1e9;
  int mx = 0;
  for (int i = 0; i < K; ++i) {
    if (sum[i] > mx) mx = sum[i];
    if (sum[i] < mn) mn = sum[i];
  }
  return (double)mx / mn;
}

void save(const State& st) {
  FILE *fp = fopen(OUTPUT_FILE, "w");
  REP(i, N) fprintf(fp, "%d\n", st.ans[i]+1);
  fclose(fp);
}

struct SA {
  double score;
  State state;
  
  bool bio[MAXN]; 

  void dfs(const State& st, int curr) {
    if (bio[curr]) return;
    bio[curr] = 1;
    for (int nxt: V[curr]) {
      if (st.ans[curr] != st.ans[nxt]) continue;
      if (bio[nxt]) continue;
      dfs(st, nxt);
    }
  }

  bool connected(const State& st) {
    static bool bio_col[MAXN];
    REP(i, K) bio_col[i] = 0;
    REP(i, N) bio[i] = 0;
    REP(i, N) {
      if (bio[i]) continue; 
      if (bio_col[st.ans[i]]) return false;
      bio_col[st.ans[i]] = 1;
      dfs(st, i);
    }
    return true; 
  }

  State find_neighbour() {
    State ret = state;
    while (true) {
      int x = rand() % N;
      int y = rand() % (int)V[x].size();
      y = V[x][y];
      int x_ans = ret.ans[x];
      ret.ans[x] = ret.ans[y];
      if (connected(ret))
        break;
      else
        ret.ans[x] = x_ans;
    }
    return ret;
  }

  // implementacija simulated annealinga pronadjenog
  // negdje na internetu... na pocetku je veca sansa
  // da se pogorsa rjesenje
  void run() {
    int itt = 0;
    for (double temp = SA_START_T; temp > 0.0001; temp *= SA_COEF) {
      REP(it, MAX_SA_ITERS) {
        State next_state = find_neighbour();
        double cs = eval(state);
        double ns = eval(next_state);
        if ((itt++) % (int)1e4 == 0)  printf(" - %lf\n", score);
        if (ns < cs || (Random() <= pow(e, -(ns - cs) / temp))) {
          state = next_state; 
          score = ns;
        }
      }
    }
  }
  
  void Build_random() {
    state = build_random();
    //state.build_random();
    score = eval(state);
  }

  friend bool operator< (const SA& a, const SA& b) {
    return a.score < b.score;
  }
};

SA make_new_SA() {
  SA ret;
  ret.Build_random();
  return ret;
}

void solve() {
  vector<SA> list;
  REP(i, TS_START_NUM) {
    list.push_back(make_new_SA());
  } 
  
  // u listi uvijek drzim vise trenutnih State-ova
  // u svakoj iteraciji uzmem jedan state na random
  // i pokrenem simulated annealing, kad je gotov
  // dodam ga u listu i izbrisem trenutni najgori u listi
  
  REP(it, MAX_TS_ITERS) {
    int x = rand() % (int)list.size(); 
    SA new_SA = list[x];    
    new_SA.run();
    list.push_back(new_SA);
    sort(ALL(list));
    if (list.size() > MAX_TS_SIZE) {
      list.pop_back(); 
    }
    save(list[0].state);
    printf("%lf\n", list[0].score);
  }
}

void build_graph() {
  REP(i, R) REP(j, C) {
    if (i > 0 && GRID[i][j] != GRID[i-1][j]) {
      V[GRID[i][j]].push_back(GRID[i-1][j]);
      V[GRID[i-1][j]].push_back(GRID[i][j]);
    }
    if (j > 0 && GRID[i][j] != GRID[i][j-1]) {
      V[GRID[i][j]].push_back(GRID[i][j-1]);
      V[GRID[i][j-1]].push_back(GRID[i][j]);
    }
  }
  REP(i, N) sort(ALL(V[i]));
  REP(i, N) V[i].resize(unique(ALL(V[i])) - V[i].begin());
}

void load() {
  int test;
  scanf("%d", &test);
  sprintf(INPUT_FILE, "input_%03d.txt", test);
  sprintf(OUTPUT_DIR, "output_%03d", test);
  sprintf(OUTPUT_FILE, "%s/best_%d.txt", OUTPUT_DIR,time(NULL));
  
  FILE *fp = fopen(INPUT_FILE, "r");
  fscanf(fp, "%d%d%d%d", &R, &C, &N, &K);
  REP(i, R) REP(j, C) fscanf(fp, "%d", &GRID[i][j]);
  REP(i, N) fscanf(fp, "%d", &P[i]); 
  fclose(fp);

  REP(i, R) REP(j, C) --GRID[i][j];
  build_graph();

  printf("N=%d, K=%d\n", N, K);
}

int main(int argc, const char* argv[]) {
  srand(time(NULL));
  load();  
  solve();

  return 0;
}

