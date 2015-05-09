#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <iostream>
#include <algorithm>
#include <vector>
using namespace std;

#define REP(i, n) for (int i = 0; i < (n); ++i)

typedef long long llint;

const int MAXN = 3010;
const int MAXK = 1000100;
const int LIMIT = 2000000; 

int N, M, K;
int LL; // duljina

int par[2*MAXN];
int len[2*MAXN];
int path_len[2*MAXN];

bool valid[MAXK];

vector<int> V[2*MAXN];
vector<int> dir_lens;
vector<int> plens[2*MAXN];

int prime[MAXK];

vector<pair<int, int>> vec;

int curr_num;

vector<int> D[MAXK];
vector<int> divs;

void rek(int p, int num) {
  if (p == (int)vec.size()) {
    divs.push_back(num); 
    return;
  }
  int t = 1;
  for (int i = 0; i <= vec[p].second; ++i) {
    rek(p+1, num*t);
    t *= vec[p].first;
  }
}

void precompute() {
  prime[0] = prime[1] = 1;
  for (int i = 2; i < MAXK; ++i) {
    if (prime[i]) continue;
    prime[i] = i;
    if (i*i > MAXK) break;
    for (int j = i*i; j < MAXK; j += i)
      prime[j] = i;
  }
  for (int i = 0; i < MAXK; ++i)
    if (!prime[i])
      prime[i] = i;
}

void get_divisors(int x) {
  vec.clear();
  divs.clear();
  curr_num = x;
  while (x > 1) {
    int k = prime[x];
    pair<int, int> add = {k, 0};
    for (; x % k == 0; x /= k)
      ++add.second;
    vec.push_back(add);
  }
  rek(0, 1);
}

void load() {
  scanf("%d%d%d", &N, &M, &K);
  scanf("%d", &LL); ++LL;
  for (int i = 1; i <= N; ++i) {
    scanf("%d%d", &par[i], &len[i]);
    V[par[i]].push_back(i);
  }
  for (int i = N+1; i <= N+M; ++i) {
    scanf("%d%d", &par[i], &len[i]);
    V[par[i]].push_back(i);
  }
  for (int i = 1; i <= N+M; ++i) {
    ++len[i];
  }
}

vector<int> dirlen;

void dfs(int curr, int sum) {
  if (sum > LIMIT) sum = LIMIT;
  path_len[curr] = sum;
  if (curr <= N)
    dirlen.push_back(sum);
  for (int next: V[curr]) {
    dfs(next, len[next] + sum);
  }
  if (curr <= N) {
    plens[curr].push_back(LL);
    for (int next: V[curr]) {
      for (int it: plens[next]) {
        plens[curr].push_back(it + len[next]);
      }
    }
  }
}

bool exists_dirlen(int val) {
  vector<int>::iterator it = lower_bound(dirlen.begin(), dirlen.end(), val);
  if (it == dirlen.end()) return false;
  return val == *it;
}

bool query_1(int pos) {
  // brez ckilusa
  int sum = LL;
  for (int i = pos; i != 0; i = par[i]) {
    sum += len[i];
    if (exists_dirlen(K - sum))
      return 1;
  }
  return 0;
}

bool exists_2(int p, int k) {
  vector<int>::iterator it = lower_bound(plens[p].begin(), plens[p].end(), k);
  if (it == plens[p].end()) return false;
  return *it == k;
}

bool exists(int p, int flen) {
  for (int div: divs) {
    if (exists_2(p, div)) 
      return 1;
  }
  return 0;
}

bool query_2(int pos) {
  // sa ciklusom
  int flen = K - path_len[pos];
  if (flen < 0) return false;
  if (flen == 0) return true;
  get_divisors(flen);
  for (int i = pos; i != 0; i = par[i]) {
    if (exists(par[i], flen)) 
      return 1;
  }
  return 0;
}

bool query(int pos) {
  return query_1(pos) || query_2(pos);
}

void solve() {
  dfs(0, 0);
  sort(dirlen.begin(), dirlen.end());
  for (int i = 0; i <= N; ++i)
    sort(plens[i].begin(), plens[i].end());
  for (int i = N+1; i <= N+M; ++i)
    puts(query(i) ? "YES" : "NO");
}

int main(void) {
  precompute();
  
  load();
  solve();
  return 0;
}

