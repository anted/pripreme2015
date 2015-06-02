#include <cstdio>
#include <vector>
#include <queue>

using namespace std;

const int MAXN = 810;

int n, s;
int mat[MAXN][MAXN];
char red[MAXN];

pair<int,int> start;
pair<int,int> finish;
vector<pair<int,int> > hiv;

queue<pair<int,int> > Q;
priority_queue<pair<pair<int,int>,pair<int,int> > > PQ;
vector<pair<int,int> > sprd;

int bio[MAXN][MAXN];
pair<int,int> dp[MAXN][MAXN];

int vx[4] = {-1, 0, 1, 0}, vy[4] = {0, 1, 0, -1};

void solve() {
  for (int i = 0; i < hiv.size(); i++) {
    Q.push(hiv[i]);
  }

  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      dp[i][j] = make_pair(-1e9, 1e9);
    }
  }
//  Q.push(make_pair(-1, -1));

  while (!Q.empty()) {
    pair<int,int> tr = Q.front(); Q.pop();
//    if (tr.first == -1 && tr.second == -1) {
//      if (Q.empty()) break;
//      Q.push(tr);
//      sprd.push_back(tr);
//      continue;
//    }

    for (int i = 0; i < 4; i++) {
      int nx = tr.first + vx[i];
      int ny = tr.second + vy[i];
      if (nx < 0 || ny < 0 || nx >= n || ny >= n) continue;
      if (bio[nx][ny] || mat[nx][ny] == -1) continue;
      bio[nx][ny] = bio[tr.first][tr.second] + 1;
      Q.push(make_pair(nx, ny));
//      sprd.push_back(make_pair(nx, ny));
    }
  }

  PQ.push(make_pair(make_pair(1e9, 0), make_pair(finish.first, finish.second)));

  while (!PQ.empty()) {
    pair<int,int> state = PQ.top().first;
    pair<int,int> pos = PQ.top().second;
    PQ.pop();

    for (int i = 0; i < 4; i++) {
      int nx = pos.first + vx[i];
      int ny = pos.second + vy[i];
      if (nx < 0 || ny < 0 || nx >= n || ny >= n) continue;
      if (dp[nx][ny].first != -1e9 || mat[nx][ny] == -1) continue;

      if (bio[nx][ny] < state.first) {
        dp[nx][ny].first = bio[nx][ny];
        dp[nx][ny].second = s - 1;
      }
      if (bio[nx][ny] >= state.first) {
        if (state.second != 0) {
          dp[nx][ny].first = state.first;
          dp[nx][ny].second = state.second - 1;
        } else {
          dp[nx][ny].first = state.first - 1;
          dp[nx][ny].second = s - 1;
        }
      }
      PQ.push(make_pair(make_pair(dp[nx][ny].first, dp[nx][ny].second), make_pair(nx, ny)));
    }
  }

//  for (int i = 0; i < n; i++) {
//    for (int j = 0; j < n; j++) {
//      printf("(%d %d) ", dp[i][j].first, dp[i][j].second);
//    }
//    printf("\n");
//  }
  if (dp[start.first][start.second].first <= 0) printf("-1\n");
  else printf("%d\n", dp[start.first][start.second].first - 1);

  return;
}

int main() {
  scanf("%d%d", &n, &s);

  for (int i = 0; i < n; i++) {
    scanf("%s", red);
    for (int j = 0; j < n; j++) {
      if (red[j] == 'G') mat[i][j] = 0;
      if (red[j] == 'T') mat[i][j] = -1;
      if (red[j] == 'M') {
        start.first = i; start.second = j;
        mat[i][j] = 0;
      }
      if (red[j] == 'D') {
        finish.first = i; finish.second = j;
        mat[i][j] = -1;
      }
      if (red[j] == 'H') {
        hiv.push_back(make_pair(i, j));
        mat[i][j] = -1;
      }
    }
  }

  solve();

  return 0;
}
