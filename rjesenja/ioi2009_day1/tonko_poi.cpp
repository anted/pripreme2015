#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <iostream>
#include <algorithm>
#include <vector>
using namespace std;

typedef long long llint;

const int MAXN = 2010;

int N, M, K;
int A[MAXN][MAXN];
int task[MAXN];

struct contestant {
  int score, solved, ID;
  friend bool operator< (contestant a, contestant b) {
    if (a.score != b.score) return a.score > b.score;
    if (a.solved != b.solved) return a.solved > b.solved;
    return a.ID < b.ID;
  }
} C[MAXN];

int main(void) {
  scanf("%d%d%d", &N, &M, &K);
  for (int i = 0; i < N; ++i)
    for (int j = 0; j < M; ++j)
      scanf("%d", &A[i][j]);
  
  for (int i = 0; i < M; ++i)
    for (int j = 0; j < N; ++j)
      if (!A[j][i]) ++task[i];
  
  for (int i = 0; i < N; ++i) {
    int cnt = 0, sc = 0;
    for (int j = 0; j < M; ++j)
      if (A[i][j]) {
        sc += task[j];
        ++cnt;
      }
    C[i].score = sc;
    C[i].solved = cnt;
    C[i].ID = i+1;
  }
  
  sort(C, C + N);
  for (int i = 0; i < N; ++i) {
    if (C[i].ID == K) {
      printf("%d %d\n", C[i].score, i+1);
      break;
    }
  }

  return 0;
}

