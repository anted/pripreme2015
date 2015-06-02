#include <algorithm>
#include <cstdio>
#include <cstring>
#include <vector>

using namespace std;

const int MAXN = 500010;

class fair {
 public:
  int t;
  int l;
  int rl;
  int m;
} fairs[MAXN];

int n, U, D, s;

vector<int> pos;
vector<fair> V;
vector<pair<int,int> > V_sol;

int off = 1<<19;
int tourU[1<<20], tourD[1<<20];

bool cmp (const fair &f1, const fair &f2) {
  if (f1.t != f2.t) return f1.t < f2.t;
  return f1.l < f2.l;
}

void update(int x, int val, int tour[]) {
  x += off;
  tour[x] = val;
  while (x != 1) {
    x /= 2;
    tour[x] = max(tour[x*2], tour[x*2+1]);
  }
}

int read_val(int l, int r, int tour[], int lo = 0, int hi = off, int curr = 1) {
  if (hi <= l || r <= lo) return -1e9;
  if (l <= lo && hi <= r) {
    return tour[curr];
  }
  int mid = (lo + hi) / 2;
  return max(read_val(l, r, tour, lo, mid, curr * 2), read_val(l, r, tour, mid, hi, curr * 2 + 1));
}

pair<int,int> calc_sol(const fair &f1) {
  int solD = read_val(0, f1.rl, tourD) - f1.l * D + f1.m;
  int solU = read_val(f1.rl + 1, pos.back() + 1, tourU) + f1.l * U + f1.m;
  int sol = max(solD, solU);
  return make_pair(sol, sol);
}

int main() {
  for (int i = 0; i < (1<<20); i++) {
    tourU[i] = tourD[i] = -1e9;
  }

  scanf("%d%d%d%d", &n, &U, &D, &s);

  pos.push_back(s);
  for (int i = 0; i < n; i++) {
    scanf("%d%d%d", &fairs[i].t, &fairs[i].l, &fairs[i].m);
    pos.push_back(fairs[i].l);
  }

  sort(pos.begin(), pos.end());
  int sz = unique(pos.begin(), pos.end()) - pos.begin();
  while (pos.size() > sz) pos.pop_back();

  int rs = lower_bound(pos.begin(), pos.end(), s) - pos.begin();
  update(rs, s * D, tourD);
  update(rs, -s * U, tourU);

  for (int i = 0; i < n; i++) {
    fairs[i].rl = lower_bound(pos.begin(), pos.end(), fairs[i].l) - pos.begin();
  }

  sort(fairs, fairs + n, cmp);

  fairs[n].t = 1e9;

  for (int i = 0; i < n; i++) {
    V.clear();
    V_sol.clear();

    int curr = i;
    V.push_back(fairs[curr]);
    while (fairs[curr+1].t == fairs[curr].t) {
      V.push_back(fairs[curr+1]);
      curr++;
    }

    for (int j = 0; j < V.size(); j++) {
      V_sol.push_back(calc_sol(V[j]));
    }

    for (int j = 1; j < V.size(); j++) {
      if (V_sol[j].first < V_sol[j-1].first - (V[j].l - V[j-1].l) * D + V[j].m) {
        V_sol[j].first = V_sol[j-1].first - (V[j].l - V[j-1].l) * D + V[j].m;
      }
    }
    for (int j = V.size()-2; j >= 0; j--) {
      if (V_sol[j].second < V_sol[j+1].second - (V[j+1].l - V[j].l) * U + V[j].m) {
        V_sol[j].second = V_sol[j+1].second - (V[j+1].l - V[j].l) * U + V[j].m;
      }
    }

    for (int j = 0; j < V.size(); j++) {
      int best = max(V_sol[j].first, V_sol[j].second);
      update(V[j].rl, best + V[j].l * D, tourD);
      update(V[j].rl, best - V[j].l * U, tourU);
    }

    i = curr;
  }

  int best = 0;
//  printf("%d (%d %d)\n", s, read_val(rs, rs+1, tourD), read_val(rs, rs+1, tourU));
  for (int i = 0; i < n; i++) {
    int sol = read_val(fairs[i].rl, fairs[i].rl+1, tourD) - fairs[i].l * D;
    if (fairs[i].l < s) {
      sol -= (s - fairs[i].l) * D;
    } else {
      sol -= (fairs[i].l - s) * U;
    }
    if (sol > best) best = sol;
//    printf("%d %d\n", fairs[i].l, read_val(fairs[i].rl, fairs[i].rl+1, tourD));
  }
  printf("%d\n", best);

  return 0;
}
