#include <cstdio>
#include <cstring>
#include <algorithm>
#include <queue>
#include <vector>

using namespace std;

const int MAX = 500050, INF = 0x3f3f3f3f;

int indeg[MAX];
int p[MAX];
int bio[MAX];
int najbliz[MAX];
int n, k, rje;
int csz;
vector <int> C, K;
queue <int> Q;

void Stabla()
{
  for (; !Q.empty(); ) {
    int node = Q.front();
    bio[node] = 1;
    Q.pop();
    if (najbliz[node] > k) {
      rje++;
      najbliz[node] = 1;
    }
    indeg[p[node]]--;
    najbliz[p[node]] = min(najbliz[p[node]], najbliz[node] + 1);
    if (!indeg[p[node]])
      Q.push(p[node]);
  }
}

int Poz(int x)
{
  return (x % csz + csz) % csz;
}

int Run(int poc)
{
  int ret=0;
  for (int kol=0; kol < csz; ret++, kol += K[poc], poc = Poz(poc + K[poc]));
  return ret;
}

int RijesiCik()
{
  int poc=-1;

  K.clear();
  for (int i=0; i<csz; i++) {
    if (C[i] != 1)
      K.push_back(k-1);
    else
      K.push_back(k);
    if (najbliz[C[i]] > k) {
      poc = Poz(i - 1);
    }
  }

  if (poc == -1)
    return 0;

  if (csz < k)
    return 1;
  
  int npoc = Poz(poc - k + 1);
  for (int i=poc, kor=0, kolko = k; i != poc || !kor; kor++, i = Poz(i - 1)) {
    K[Poz(i - k + 1)] = kolko;
    if (najbliz[C[i]] > k)
      kolko = k;
    else
      kolko++;
  }

  int ret = INF;
  for (int i=npoc, j=0; j<=k; j++, i = Poz(i + 1))
    ret = min(ret, Run(i));

  return ret;
}

void Ciklusi()
{
  for (int i=1; i<=n; i++) {
    if (!bio[i]) {
      csz = 0;
      C.clear();
      int minn = INF + 5, ind = -1;
      for (int j=i; j != i || !csz; csz++, j = p[j]) {
        bio[j] = 1;
        C.push_back(j);
        if (najbliz[j] < minn) {
          minn = najbliz[j];
          ind = j;
        }
      }

      for (int j=ind, sz=0, tdist = INF; j != ind || !sz; sz++, j = p[j]) {
        tdist = min(tdist + 1, najbliz[j]);
        najbliz[j] = tdist;
      }
      
      rje += RijesiCik();
    }
  }
}

int main()
{
  scanf("%d%d", &n, &k);

  for (int i=1; i<=n; i++) {
    int a, b;
    scanf("%d%d", &a, &b);
    p[a] = b;
    indeg[b]++;
  }

  for (int i=1; i<=n; i++)
    if (!indeg[i])
      Q.push(i);

  memset(najbliz, INF, sizeof najbliz);
  najbliz[1] = 0;

  Stabla();
  Ciklusi();

  printf("%d\n", rje);

  return 0;
}
