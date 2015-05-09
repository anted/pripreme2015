#include <cstdio>
#include <cstring>
#include <algorithm>
#include <vector>
#include <ctime>
#include <unistd.h>

#define X first
#define Y second

using namespace std;

typedef pair<int, int> P;
typedef long long ll;

const int MAX = 505, MAXE = 2005, BRPOK = 10, BRPROM = 5000000;

P toc[MAX];
vector <P> V[MAX];
int koja[MAX], poz[MAX];
int rje[MAX];
int s, n, m, k;
int pres[MAXE][MAXE];
int tocpres[MAX];
int poc[MAX];
P sorted[MAX];
vector <P> E;

ll Ccw(P a, P b, P c)
{
  return (ll) a.X * (b.Y - c.Y) + (ll) b.X * (c.Y - a.Y) + (ll) c.X * (a.Y - b.Y);
}

int Sjec(P a1, P a2, P b1, P b2)
{
  ll a = Ccw(a1, a2, b1);
  ll b = Ccw(a1, a2, b2);
  ll c = Ccw(b1, b2, a1);
  ll d = Ccw(b1, b2, a2);

  return (((a < 0 && b > 0) || (a > 0 && b < 0)) && ((c < 0 && d > 0) || (c > 0 && d < 0)));
}

int UpdatePres(int a)
{
  int ret=0;

  for (int i=0; i<(int) V[a].size(); i++) {
    P t = V[a][i];

    for (int j=0; j<(int) E.size(); j++) {
      P e = E[j];
      if (e.X != a && e.Y != a && e.X != t.X && e.Y != t.X) {
        int pros = pres[j][t.Y];
        int nov = pres[j][t.Y] = pres[t.Y][j] = Sjec(toc[poz[a]], toc[poz[t.X]], toc[poz[e.X]], toc[poz[e.Y]]);
        int raz = nov - pros;

        ret += raz;
        tocpres[a] += raz;
        tocpres[t.X] += raz;
        tocpres[e.X] += raz;
        tocpres[e.Y] += raz;
      }
    }
  }

  return ret;
}

int BrPres()
{
  int ret=0;

  for (int i=0; i<(int) E.size(); i++)
    for (int j=i+1; j<(int) E.size(); j++)
      ret += Sjec(toc[poz[E[i].X]], toc[poz[E[i].Y]], toc[poz[E[j].X]], toc[poz[E[j].Y]]);

  return ret;
}

void UpdateSort()
{
  for (int i=0; i<n; i++)
    sorted[i] = P(-tocpres[i], i);

  sort(sorted, sorted + n);
}

void Ucitaj(int ind, int load)
{
  FILE *in;
  string str = "input_00";
  str += ('0' + ind);
  str += ".txt";

  in = fopen(str.c_str(), "r");
  fscanf(in, "%d%d%d", &s, &n, &m);

  for (int i=0; i<m; i++) {
    int a, b;
    fscanf(in, "%d%d", &a, &b); a--; b--;
    V[a].push_back(P(b, i));
    V[b].push_back(P(a, i));
    E.push_back(P(a, b));
  }

  fscanf(in, "%d", &k);
  for (int i=0; i<k; i++)
    fscanf(in, "%d%d", &toc[i].X, &toc[i].Y);

  fclose(in);
  str = "tout";
  str += ('0' + ind);
  in = fopen(str.c_str(), "r");

  if (load) {
    for (int i=0; i<n; i++) {
      fscanf(in, "%d", &poc[i]);
      poc[i]--;
    }
  }
}

void Ispis(int ind)
{
  FILE *out;
  string str = "tout";

  str += '0' + ind;
  
  out = fopen(str.c_str(), "w");

  memcpy(rje, poz, sizeof rje);

  for (int i=0; i<n; i++)
    fprintf(out, "%d\n", rje[i] + 1);
  fclose(out);
}

int main()
{
  int ind, load, gran, kor, sor;
  int best = 1e9;
  
  printf("IND LOAD GRAN KOR SORT\n");
  scanf("%d%d%d%d%d", &ind, &load, &gran, &kor, &sor);
  Ucitaj(ind, load);
  
  for (int tt=0; tt<BRPOK; tt++) {
    memset(koja, -1, sizeof koja);
    memset(poz, -1, sizeof poz);
    memset(pres, 0, sizeof pres);

    for (int i=0; i<n; i++) {
      if (!load) {
        for (;;) {
          int tmp = rand() % k;
          if (koja[tmp] == -1) {
            koja[tmp] = 1;
            poz[i] = tmp;
            break;
          }
        }
      }
      else {
        koja[poc[i]] = i;
        poz[i] = poc[i];
      }
    }

    fprintf(stderr, "DA\n");

    int brpr=0;
    for (int i=0; i<n; i++)
      brpr += UpdatePres(i);

    if (sor)
      UpdateSort();
  
    int pros = brpr;
    for (int prom=0; prom<BRPROM; prom++) {
      int treba = 1;
      if (!(prom % kor)) {
        fprintf(stderr, "TT PROM %d %d %d\n", tt, prom, brpr);
        pros = brpr;
        if (brpr < best) {
          best = brpr;
          Ispis(ind);
        }

        if (prom && pros - brpr < gran)
          treba = 0;
      }
      
      int a;
      if (sor) {
        if (rand() & 1)
          a = sorted[rand() % (n / 10)].Y;
        else if (rand() & 1)
          a = sorted[rand() % (n / 3)].Y;
        else 
          a = sorted[rand() % n].Y;
      }
      else
        a = rand() % n;
  
      int node; 
      int indnaj = -1;
      int post = rand() % 100;

      if (post < 3) {
        int naj = 1e9;
        for (int i=0; i<k; i++) {
          if (koja[i] == -1) {
            int prij = poz[a];
            poz[a] = i;
            koja[i] = a;
            int tmp = UpdatePres(a);

            if (tmp < naj) {
              naj = tmp;
              indnaj = i;
            }
            poz[a] = prij;
            koja[i] = -1;
            UpdatePres(a);
          }
          else {
            int b = koja[i];
            int prij = poz[a];
            swap(poz[a], poz[b]);
            swap(koja[prij], koja[i]);
            int tmp = UpdatePres(a) + UpdatePres(b);
            
            if (tmp < naj) {
              naj = tmp;
              indnaj = i;
            }
            swap(poz[a], poz[b]);
            swap(koja[prij], koja[i]);
            UpdatePres(a);
            UpdatePres(b);
          }
        }

        node = indnaj;
      }
      else if (post < 6) {
        for (;;) {
          int e1 = rand() % m;
          int e2 = rand() % m;
          if (pres[e1][e2]) {
            if (rand() & 1)
              a = E[e1].X;
            else
              a = E[e1].Y;

            if (rand() & 1)
              node = E[e2].X;
            else
              node = E[e2].Y;

            break;
          }
        }
      }
      else
        node = rand() % k;      

      if (koja[node] == -1) {
        int prij = poz[a];
        poz[a] = node;
        koja[node] = a;

        int tmp = UpdatePres(a);
        if (tmp > 0 && treba) {
          poz[a] = prij;
          koja[node] = -1;
          UpdatePres(a);
        }
        else {
          brpr += tmp;
          if (sor)
            UpdateSort();
        }
      }
      else {
        int b = koja[node];
        int prij = poz[a];
        swap(poz[a], poz[b]);
        swap(koja[prij], koja[node]);

        int tmp = UpdatePres(a) + UpdatePres(b);
        if (tmp > 0 && treba) {
          swap(poz[a], poz[b]);
          swap(koja[prij], koja[node]);
          UpdatePres(a);
          UpdatePres(b);
        }
        else {
          brpr+= tmp;
          if (sor)
            UpdateSort();
        }
      }

      if (brpr < best) {
        best = brpr;
        Ispis(ind);
      }
    }
  }

  Ispis(ind);

  return 0;
}
