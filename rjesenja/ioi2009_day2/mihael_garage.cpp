#include <cstdio>
#include <queue>

using namespace std;

const int MAXN = 110;
const int MAXM = 2010;

int n, m;
int r[MAXN], w[MAXM];

int pos[MAXM];
int st[MAXN];

queue<int> Q;
queue<int> Q2;

int main() {
  scanf("%d%d", &n, &m);

  for (int i = 0; i < n; i++) {
    scanf("%d", &r[i]);
  }

  for (int i = 1; i <= m; i++) {
    scanf("%d", &w[i]);
  }

  for (int i = 0; i < m * 2; i++) {
    int tmp;
    scanf("%d", &tmp);
    Q.push(tmp);
  }

  int free = n;
  int sol = 0;
  while (!Q.empty()) {
    int tr = Q.front(); Q.pop();
    if (tr > 0) {
      if (free) {
        for (int i = 0; i < n; i++) {
          if (st[i] == 0) {
            st[i] = tr;
            pos[tr] = i;
            free--;
            sol += w[tr] * r[pos[tr]];
//            printf("%d zauzima %d\n", tr, pos[tr]);
            break;
          }
        }
      } else {
        Q2.push(tr);
      }
    } else {
      st[pos[-tr]] = 0;
      if (!Q2.empty()) {
        int tr2 = Q2.front(); Q2.pop();
        st[pos[-tr]] = tr2;
        pos[tr2] = pos[-tr];
        free--;
        sol += w[tr2] * r[pos[tr2]];
//        printf("%d zauzima %d\n", tr2, pos[tr2]);
      }
      pos[-tr] = 0;
      free++;
    }
  }

  printf("%d\n", sol);

  return 0;
}
