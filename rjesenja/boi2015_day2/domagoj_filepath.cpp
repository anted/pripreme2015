#include <cstdio>
#include <algorithm>
#include <vector>
#include <map>

//koristi puno memorije - acc na BOIu, ne prolazi na fer evaluatoru

using namespace std;

const int MAXN = 3005, MAX = 1000005; //change MAX

vector <int> V[MAXN];
int par[MAXN], val[MAXN];
int doroota[MAXN];
int tdulj, symdulj;
int disc[MAXN], fin[MAXN];
int vr;
vector <int> S;
map <int, int> M[MAXN];
map <int, int> C[MAXN];

void Dfs(int node, int dosad)
{
  disc[node] = vr++;
  doroota[node] = dosad + val[node];
  S.push_back(node);

  for (int i=0; i<(int) S.size(); i++) {
    int nn = S[i];
    int l = doroota[node] - doroota[nn] + symdulj;
    for (int j=0; j<MAX; j+=l)
      M[nn][j] = 1;
    M[node][doroota[nn] - doroota[node] + symdulj] = 1;
  }

  for (int i=0; i<(int) V[node].size(); i++)
    Dfs(V[node][i], doroota[node]);

  S.pop_back();
  fin[node] = vr++;
}

int main()
{
  int n, m;

  scanf("%d%d%d%d", &n, &m, &tdulj, &symdulj); symdulj++;

  for (int i=1; i<=n; i++) {
    scanf("%d%d", &par[i], &val[i]); val[i]++;
    V[par[i]].push_back(i);
  }

  Dfs(0, 0);
  par[0] = -1;

  for (int i=0; i<=n; i++) {
    for (int j=0; j<=n; j++) { //iz i u j, sideedge
      if (!(disc[i] > disc[j] && disc[i] < fin[j]) &&
          !(disc[j] > disc[i] && disc[j] < fin[i]))
            M[j][doroota[i] + symdulj - doroota[j]] = 1;
    }
  }

  for (int i=0; i<m; i++) {
    int poc, dulj, da=0, jaroot;
    scanf("%d%d", &poc, &dulj);
    jaroot = dulj + 1 + doroota[poc];

    for (; poc >= 0; poc = par[poc])
      da |= M[poc][tdulj - jaroot];

    if (da)
      printf("YES\n");
    else
      printf("NO\n");
  }

  return 0;
}
