#include <algorithm>
#include <cmath>
#include <cstdio>
#include <map>
#include <vector>

using namespace std;

const int MAXN = 200010;
const int MAXR = 25010;
const int MAXQ = 200010;

int n, r, q;

int region[MAXN];
int par[MAXN];
int disc[MAXN];
int finish[MAXN];
vector<int> V[MAXN];

int curr_t = 0;

vector<int> regije[MAXR];
vector<int> lg[MAXR];

void DFS(int curr) {
  disc[curr] = curr_t++;
  lg[region[curr]].push_back(disc[curr]);
  for (int i = 0; i < V[curr].size(); i++) {
    int next = V[curr][i];
    DFS(next);
  }
  finish[curr] = curr_t;
  return;
}

//map<int,int> lg[MAXR];

//void update(int x, int val, map<int,int> &lg) {
//  x++;
//  for (; x < MAXN; x+=x&-x) lg[x] += val;
//}
//
//int citaj(int x, map<int,int> &lg) {
//  x++;
//  int sol = 0;
//  for (; x > 0; x-=x&-x) {
//    if (lg.count(x))
//      sol += lg[x];
//  }
//  return sol;
//}

//void init_log() {
//  for (int i = 0; i < n; i++) {
//    update(disc[i], 1, lg[region[i]]);
//  }
//}

int big_cnt[500];
int big_sol[500][MAXR];
int big_region[MAXR];
vector<int> big_regions;

void gen_reg_sol(int curr) {
  if (big_region[region[curr]]) {
    big_cnt[big_region[region[curr]] - 1]++;
  }
  for (int i = 0; i < big_regions.size(); i++) {
    big_sol[i][region[curr]] += big_cnt[i];
  }
  for (int i = 0; i < V[curr].size(); i++) {
    int next = V[curr][i];
    gen_reg_sol(next);
  }
   if (big_region[region[curr]]) {
    big_cnt[big_region[region[curr]] - 1]--;
  }

  return;
}

int main() {
  scanf("%d%d%d", &n, &r, &q);

  scanf("%d", &region[0]);
  par[0] = 0;
  for (int i = 1; i < n; i++) {
    scanf("%d%d", &par[i], &region[i]);
    par[i]--;
    V[par[i]].push_back(i);
  }

  DFS(0);

//  init_log();

  for (int i = 0; i < n; i++) {
    regije[region[i]].push_back(i);
  }

  int pom = 500;
  for (int i = 1; i <= r; i++) {
    if (regije[i].size() >= pom) {
      big_region[i] = big_regions.size() + 1;
      big_regions.push_back(i);
    }
  }

  gen_reg_sol(0);

  for (int i = 0; i < q; i++) {
    int r1, r2;
    int sol = 0;
    scanf("%d%d", &r1, &r2);
    if (big_region[r1]) {
      printf("%d\n", big_sol[big_region[r1]-1][r2]);
      continue;
    }
    for (int j = 0; j < regije[r1].size(); j++) {
      int tr = regije[r1][j];
//      sol += citaj(finish[tr] - 1, lg[r2]) - citaj(disc[tr], lg[r2]);
        sol += upper_bound(lg[r2].begin(), lg[r2].end(), finish[tr] - 1) - lower_bound(lg[r2].begin(), lg[r2].end(), disc[tr]);
    }
    printf("%d\n", sol);
  }

  return 0;
}
