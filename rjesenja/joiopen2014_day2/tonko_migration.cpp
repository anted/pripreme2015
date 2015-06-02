#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <iostream>
#include <algorithm>
#include <vector>
#include <map>
#include <set>
#include <ctime>
using namespace std;

typedef long long llint;

char OUTPUT_DIR[32];

const int MAXN = 3100;


struct point {
  int x, y;
  bool friend operator< (point a, point b) {
    if (a.x != b.x) return a.x < b.x;
    return a.y < b.y;
  }
};

point make_point(int x, int y) {
  point ret;
  ret.x = x;
  ret.y = y;
  return ret;
}

llint ccw(point a, point b, point c) {
  return (llint)a.x * (b.y-c.y) + (llint)b.x*(c.y-a.y) + (llint)c.x*(a.y-b.y);
}

int sig(llint x) {
  if (x < 0) return -1;
  if (x > 0) return 1;
  return 0;
}

bool intersect(point a1, point a2, point b1, point b2) {
  int a = sig(ccw(a1, b1, a2));
  int b = sig(ccw(a1, b2, a2));
  int c = sig(ccw(b1, a1, b2));
  int d = sig(ccw(b1, a2, b2));
  if (a > b) swap(a, b); 
  if (c > d) swap(c, d); 
  return (a == -1 && b == 1 && c == -1 && d == 1);
}

int N, M, L;
int u[MAXN], v[MAXN];
point A[MAXN];
vector<int> V[MAXN];

int pos[MAXN];
int idx[MAXN];
double SCORE;
double BEST = 10000000.0;

void load_task(int x) {
  static char filepath[32];
  sprintf(OUTPUT_DIR, "output_%03d", x);
  sprintf(filepath, "input_%03d.txt", x);
  FILE *fp = fopen(filepath, "r");
  fscanf(fp, "%d%d", &N, &M);
  for (int i = 0; i < M; ++i) {
    fscanf(fp, "%d%d", &u[i], &v[i]);
    --u[i]; --v[i];
    V[u[i]].push_back(v[i]);
    V[v[i]].push_back(u[i]);
  }
  fscanf(fp, "%d", &L);
  for (int i = 0; i < L; ++i) {
    fscanf(fp, "%d%d", &A[i].x, &A[i].y); 
  }
  fclose(fp);
}

void init(int argc, char *argv[]) {
  int x;
  if (argc == 1) scanf("%d", &x);
  if (argc == 2) sscanf(argv[1], "%d", &x);
  load_task(x);
}

void build_random() {
  static int tmp[MAXN];
  for (int i = 0; i < L; ++i) tmp[i] = i;
  random_shuffle(tmp, tmp+L);
  for (int i = 0; i < L; ++i) idx[i] = tmp[i];
}

double eval() {
  double ret = 0;
  for (int i = 0; i < M; ++i) {
    for (int j = 0; j < i; ++j) {
      if (i == j) continue;
      point a1 = A[idx[u[i]]];
      point a2 = A[idx[v[i]]];
      point b1 = A[idx[u[j]]];
      point b2 = A[idx[v[j]]];
      if (intersect(a1, a2, b1, b2))
        ++ret;
    }
  }
  return ret;
}

void swap_pts(int x, int y) {
  swap(idx[x], idx[y]);
}

double get_int_cnt(int x) {
  if (x >= N) return 0;
  double ret = 0;
  for (int y: V[x]) {
    for (int i = 0; i < M; ++i) {
      point a1 = A[idx[x]];
      point a2 = A[idx[y]];
      point b1 = A[idx[u[i]]];
      point b2 = A[idx[v[i]]];
      if (intersect(a1, a2, b1, b2))
        ++ret;
    } 
  }
  return ret;
}

double delta_score(int x, int y) {
  double ret = 0;
  ret -= get_int_cnt(x) + get_int_cnt(y);
  swap_pts(x, y);

  ret += get_int_cnt(x)+get_int_cnt(y);
  swap_pts(x, y);
  return ret;
}

double delta_score(int x, int y, int z) {
  double ret = 0;
  ret -= get_int_cnt(x)+get_int_cnt(y) +get_int_cnt(z);
  int t = idx[x];
  idx[x] = idx[y];
  idx[y] = idx[z];
  idx[z] = t;

  ret += get_int_cnt(x)+get_int_cnt(y) +get_int_cnt(z);
  for (int i=0;i<2;++i){
    t = idx[x];
    idx[x] = idx[y];
    idx[y] = idx[z];
    idx[z] = t;
  }
  return ret;
}

bool check(int x, int y) {
  double sc = delta_score(x, y);
  if (sc < 0) {
    SCORE += sc;
    swap_pts(x, y);
    return true;
  }
  return false;
}

bool repair() {
  if ((rand() & 511) == 0) {
    int x = rand() % L;
    int y = rand() % L;
    swap_pts(x, y);
    SCORE = eval();
    return true;
  }

  static vector<pair<double, pair<int, int>>> vec;
  vec.clear();


  double mn = 10000000.0;
  int xp, yp, zp;


  for (int it = 0; it < 20; ++it) {
    int x = rand()%L;
    int y = rand()%L;
    double sc = delta_score(x, y);
    if (sc < mn) {
      mn = sc;
      xp = x;
      yp = y;
    }
  }

  swap_pts(xp, yp);
  double sc = SCORE + mn;

  if (sc < SCORE) {
    SCORE = sc; 
    return true;
  } else {
    swap_pts(xp, yp);
    return false;
  }

}

void load_best() {
  static int tmp[MAXN];
  for (int i= 0;i<L;++i)tmp[i]=0;
  char file[128];
  sprintf(file, "%s/out.txt", OUTPUT_DIR);

  FILE *fp = fopen(file, "r");
  for (int i = 0; i < N; ++i) {
    fscanf(fp, "%d", &idx[i]);
    --idx[i];
    tmp[idx[i]]=1;
  }
  int tr = N;
  for (int i = 0; i < L; ++i)
    if (!tmp[i])
      idx[tr++] = i;
  fclose(fp);
  random_shuffle(idx+N,idx+L);
}

void save() {
  static char file[128];
  if (SCORE < BEST) {
    BEST = SCORE;
    sprintf(file, "%s/out.txt", OUTPUT_DIR);
    FILE *fp = fopen(file, "w");
    for (int i = 0; i < N; ++i) fprintf(fp, "%d\n", 1+idx[i]);
    fclose(fp);
  }
}

void solve() {
  load_best();

  SCORE = eval();
  printf("SC %lf\n", SCORE);
  
  for (int it = 0; it < 500000; ++it) {
    if (it % 50 == 0) printf("%6d-th iteration: %lf (%.2lf)\n", it, eval(), BEST);
    repair();
    //SCORE = eval();
    save();
    BEST= min(BEST, SCORE);

    if (it == 2) {
      build_random();
      SCORE = eval();
    }
  }
  printf("SC %lf\n", SCORE);
}

int main(int argc, char *argv[]) {
  srand(time(NULL));
  init(argc, argv);
  solve();
  return 0;
}

