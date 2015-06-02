#include <algorithm>
#include <cstdio>
#include <map>
#include <set>
#include <vector>

using namespace std;

const int MAXN = 3010;
const int MAXM = 3010;

int n, m, k;
int s;
pair<int,int> parD[MAXN];
pair<int,int> parF[MAXM];

vector<pair<int,int> > VD[MAXN];
vector<pair<int,int> > VF[MAXM];

vector<int> djelitelji[1000010];

int good[MAXM];

set<int> from_root;
set<int> from_node[MAXN];
map<int, int> cycles;

void DFS(int curr, int dist) {
  from_root.insert(dist);
  for (int i = 0; i < VD[curr].size(); i++) {
    DFS(VD[curr][i].first, dist+VD[curr][i].second);
  }
}

void all_cycles(int curr, int dist, int op) {
  from_node[op].insert(dist + s);
//  printf("Cycle from %d of length %d\n", op, dist + s);
  for (int i = 0; i < VD[curr].size(); i++) {
    all_cycles(VD[curr][i].first, dist + VD[curr][i].second, op);
  }
}

bool check(int file) {
  int currD = parF[file].first;
  int curr_sum = parF[file].second;
  if (from_root.count(k-curr_sum-s)) return true;
//  printf("%d %d %d\n", file, curr_sum, currD);
  while (currD != 0) {
    curr_sum += parD[currD].second;
    currD = parD[currD].first;
    if (from_root.count(k-curr_sum-s)) return true;
  }
  if (curr_sum == k) return true;
  return false;
}

void DFS2(int curr, int dist) {
//  printf("%d %d\n", curr, dist);
  int currD = curr;
  bool done = false;
  while (!done) {
    for (int i = 0; i < VF[curr].size(); i++) {
      if (good[VF[curr][i].first]) continue;
      int need = k - dist - VF[curr][i].second;
//      printf("For file %d need %d\n", VF[curr][i].first, need);
      for (int j = 0; j < djelitelji[need].size(); j++) {
        if (from_node[currD].count(djelitelji[need][j])) {
          good[VF[curr][i].first] = 1;
          break;
        }
      }
    }
    if (currD == 0) done = true;
    currD = parD[currD].first;
  }

  for (int i = 0; i < VD[curr].size(); i++) {
    DFS2(VD[curr][i].first, dist + VD[curr][i].second);
  }
}

int main() {

  for (int i = 1; i < 1000000; i++) {
    for (int j = i; j < 1000000; j += i) {
      djelitelji[j].push_back(i);
    }
  }

  scanf("%d%d%d", &n, &m, &k);
  scanf("%d", &s);
  s++;

  for (int i = 1; i <= n; i++) {
    int p, l;
    scanf("%d%d", &p, &l);
    l++;
    parD[i].first = p;
    parD[i].second = l;
    VD[p].push_back(make_pair(i, l));
  }

  for (int i = 1; i <= m; i++) {
    int p, l;
    scanf("%d%d", &p, &l);
    l++;
    parF[i].first = p;
    parF[i].second = l;
    VF[p].push_back(make_pair(i, l));
  }

  DFS(0, 0);
  for (int i = 0; i <= n; i++) {
    all_cycles(i, 0, i);
  }

  for (int i = 1; i <= m; i++) {
    if (check(i)) good[i] = true;
  }

  cycles[s] = 1;
  DFS2(0, 0);

  for (int i = 1; i <= m; i++) {
    if (good[i]) printf("YES\n"); else printf("NO\n");
  }

  return 0;
}
