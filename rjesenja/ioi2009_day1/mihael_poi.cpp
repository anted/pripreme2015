#include <algorithm>
#include <cstdio>
#include <vector>

using namespace std;

const int MAXN = 2010;

int n, t, p;
int mat[MAXN][MAXN];
int num_solved[MAXN];
int score[MAXN];
int task[MAXN];

vector<int> V;

bool cmp(const int &i1, const int &i2) {
  if (score[i1] != score[i2]) return score[i1] > score[i2];
  if (num_solved[i1] != num_solved[i2]) return num_solved[i1] > num_solved[i2];
  return i1 < i2;
}

int main() {

  scanf("%d%d%d", &n, &t, &p);
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < t; j++) {
      scanf("%d", &mat[i][j]);
      if (mat[i][j] == 1) task[j]++;
    }
    V.push_back(i);
  }

  for (int i = 0; i < n; i++) {
    for (int j = 0; j < t; j++) {
      if (mat[i][j] == 1) {
        num_solved[i]++;
        score[i] += n-task[j];
      }
    }
  }

  sort(V.begin(), V.end(), cmp);

  for (int i = 0; i < V.size(); i++) {
    if (V[i]+1 == p) printf("%d %d\n", score[V[i]], i+1);
  }

  return 0;
}
