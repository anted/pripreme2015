#include <cstdio>
#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <unistd.h>

using namespace std;

typedef pair<int, int> P;
#define X first
#define Y second

const int MAXN = 7500, MAXV = 105;

int best;
int brtile, brboj;
int brred, brstup;
string indtp;
int load;
int p[MAXV][MAXV];
int val[MAXV][MAXV];
int sz[MAXN], boj[MAXN];
int pr[4] = {1, 0, -1, 0}, ps[4] = {0, 1, 0, -1};
vector <P> V[MAXN];

int Vani(int r, int s)
{
  return r < 0 || s < 0 || r >= brred || s >= brstup;
}

int Vrijednost(int r1, int s1, int r2, int s2)
{
  if (Vani(r1, s1) || Vani(r2, s2) || p[r1][s1] == p[r2][s2])
    return 0;
  return val[boj[p[r1][s1]]][boj[p[r2][s2]]];
}

int Val1(int ind)
{
  int ret = 0;

  for (int i=0; i<(int) V[ind].size(); i++)
    for (int j=0; j<4; j++)
      ret += Vrijednost(V[ind][i].X, V[ind][i].Y, V[ind][i].X + pr[j], V[ind][i].Y + ps[j]);
  
  return ret;
}

int EvalPoc()
{
  int ret = 0;

  for (int i=0; i<brred; i++) {
    for (int j=0; j<brstup; j++) {
      ret += Vrijednost(i, j, i+1, j);
      ret += Vrijednost(i, j, i, j+1);
    }
  }

  return ret;
}

void Save()
{
  string tmp = "out";
  tmp += indtp;

  for (int br=0; br<2; br++) {
    string aaa = tmp;

    if (!br)
      aaa += "_1";
    else
      aaa += "_2";

    FILE *f = fopen(aaa.c_str(), "w");

    for (int i=0; i<brtile; i++) {
      for (int j=0; j<(int) V[i].size(); j++) {
        fprintf(f, "%d %d", V[i][j].X + 1, V[i][j].Y + 1);
        if (j < (int) V[i].size() - 1)
          fprintf(f, " ");
      }
      fprintf(f, "\n");
    }

    fclose(f);
  }
}

void Init()
{
  int indr = 0, inds = 0;

  for (int i=0; i<brtile; i++) {
    if (sz[i] == 2) {
      V[i].push_back(P(indr, inds));
      V[i].push_back(P(indr, inds+1));
      p[indr][inds] = p[indr][inds+1] = i;

      inds += 2;
      if (inds == brstup) {
        inds = 0;
        indr++;
      }
    }
  }

  for (int i=0; i<brtile; i++) {
    if (sz[i] == 1) {
      V[i].push_back(P(indr, inds));
      p[indr][inds] = i;

      inds++;
      if (inds == brstup) {
        inds = 0;
        indr++;
      }
    }
  }
}

void Input()
{
  string tmp = "input_0";
  tmp += indtp;
  tmp += ".txt";

  FILE *f = fopen(tmp.c_str(), "r");
  fscanf(f, "%d%d%d%d", &brred, &brstup, &brboj, &brtile);
  for (int i=0; i<brtile; i++) {
    fscanf(f, "%d%d", &sz[i], &boj[i]);
    boj[i]--;
  }

  for (int i=0; i<brboj; i++)
    for (int j=0; j<brboj; j++)
      fscanf(f, "%d", &val[i][j]);

  fclose(f);

  if (!load)
    Init();
  else {
    tmp = "out";
    tmp += indtp;
    tmp += "_1";
    
    f = fopen(tmp.c_str(), "r");

    for (int i=0; i<brtile; i++) {
      for (int j=0; j<sz[i]; j++) {
        int r, s;
        fscanf(f, "%d%d", &r, &s); r--; s--;
        p[r][s] = i;
        V[i].push_back(P(r, s));
      }
    }

    fclose(f);
  }

  boj[MAXN-1] = MAXV-1;
}

void Popuni(int ind, int scim)
{
  for (int i=0; i<(int) V[ind].size(); i++)
    p[V[ind][i].X][V[ind][i].Y] = scim;
}

int Zamijeni(int ind1, int ind2)
{
  int ret = 0;

  if (sz[ind1] != sz[ind2])
    return 0;

  if (ind1 == ind2)
    return 0;

  ret -= Val1(ind1);
  Popuni(ind1, MAXN-1);
  
  ret -= Val1(ind2);
  Popuni(ind2, MAXN-1);

  swap(V[ind1], V[ind2]);

  Popuni(ind1, ind1);
  ret += Val1(ind1);

  Popuni(ind2, ind2);
  ret += Val1(ind2);

  return ret;
}

int main()
{
  int kor, prosli=-1, kolko;
  srand(time(0) * getpid());

  printf("INDTP LOAD KOR KOLKO\n");
  cin >> indtp >> load >> kor >> kolko;

  Input();
  Save();
  int rje = EvalPoc();

  int fejl = 0;
  for (int tt=0;; tt++) {
    if (!(tt % kor)) {
      fejl = 0;
      if (rje > best) {
        best = rje;
        Save();
      }
      else if (rje == prosli)
        fejl = 1;

      prosli = rje;
      printf("LL %d %d %d\n", tt, rje, best);
    }
  
    int a = rand() % brtile;
    int b = rand() % brtile;    
    int tmp = Zamijeni(a, b);

    if (tmp < 0 && (!fejl || tt % kor >= kolko))
      Zamijeni(a, b);
    else
      rje += tmp;
  }


  return 0;
}
