#include <cstdio>
#include <cstring>
#include <algorithm>
#include <vector>

using namespace std;

const int TOUR = 1<<19, INF = 2 * 1e9 + 100;

struct fair {
  int t, poz, val;
  fair (int t, int poz, int val) : t(t), poz(poz), val(val) {}
  fair () {}
} p[TOUR];

struct str {
  int *poz, prij, nov;
  str (int *poz, int prij, int nov) : poz(poz), prij(prij), nov(nov) {}
  str () {}
};

bool operator < (const fair &a, const fair &b)
{
  if (a.t != b.t)
    return a.t < b.t;
  return a.poz < b.poz;
}

int tour[2][2*TOUR];
int prij[2][2*TOUR], nov[2][2*TOUR];
vector <int> R;

void Stavi(int t[2*TOUR], int pos, int val, int st)
{
  for (pos += TOUR; pos; pos /= 2) {
    if (t[pos] < val) {
      if (st) {
        int ind = (t + pos - tour[0]) / (2 * TOUR);
        R.push_back(t + pos - tour[0]);
        prij[ind][pos] = min(prij[ind][pos], t[pos]);
        nov[ind][pos] = val;
      }
      t[pos] = val;
    }
  }
}

int Vrati(int t[2*TOUR], int pos, int lo, int hi, int begin, int end)
{
  if (lo >= end || hi <= begin)
    return -INF;
  if (lo >= begin && hi <= end)
    return t[pos];
  return max(Vrati(t, 2*pos+0, lo, (lo+hi)/2, begin, end),
             Vrati(t, 2*pos+1, (lo+hi)/2, hi, begin, end) );
}

void VratiNazad()
{
  for (int i=0; i<(int) R.size(); i++) {
    int ind = R[i] / (2 * TOUR);
    int poz = R[i] % (2 * TOUR);
    tour[ind][poz] = min(tour[ind][poz], prij[ind][poz]);
  }
}

void StaviOpet()
{
  for (int i=0; i<(int) R.size(); i++) {
    int ind = R[i] / (2 * TOUR);
    int poz = R[i] % (2 * TOUR);
    tour[ind][poz] = max(tour[ind][poz], nov[ind][poz]);
    prij[ind][poz] = INF;
  }

  R.clear();
}

int main()
{
  int n, u, d, poc;

  scanf("%d%d%d%d", &n, &u, &d, &poc);

  for (int i=0; i<n; i++)
    scanf("%d%d%d", &p[i].t, &p[i].poz, &p[i].val);
  p[n++] = fair(INF, poc, 0);

  sort(p, p + n);

  for (int i=0; i<2*TOUR; i++)
    tour[0][i] = tour[1][i] = -INF;
  
  for (int i=0; i<2*TOUR; i++)
    prij[0][i] = prij[1][i] = INF;

  Stavi(tour[0], poc, poc * d, 0);
  Stavi(tour[1], poc, -poc * u, 0);
  
  for (int i=0; i<n; ) {    
    int kraj;
    for (kraj=i; kraj<n && p[i].t == p[kraj].t; kraj++);
    kraj--;

    for (int j=i; j<=kraj; j++) {
      int best = max(Vrati(tour[0], 1, 0, TOUR, 0, p[j].poz + 1) - d * p[j].poz,
                     Vrati(tour[1],   1, 0, TOUR, p[j].poz,  TOUR) + u * p[j].poz)
                 + p[j].val;

      Stavi(tour[0], p[j].poz, best + d * p[j].poz, 1);
      Stavi(tour[1], p[j].poz, best - u * p[j].poz, 1);
    }

    VratiNazad();

    for (int j=kraj; j>=i; j--) {
      int best = max(Vrati(tour[0], 1, 0, TOUR, 0, p[j].poz + 1) - d * p[j].poz,
                     Vrati(tour[1],   1, 0, TOUR, p[j].poz,  TOUR) + u * p[j].poz)
                 + p[j].val;

      Stavi(tour[0], p[j].poz, best + d * p[j].poz, 0);
      Stavi(tour[1], p[j].poz, best - u * p[j].poz, 0);
    }
    
    StaviOpet();
    i = kraj + 1;
  }

  printf("%d\n", Vrati(tour[0], 1, 0, TOUR, poc, poc + 1) - poc * d);

  return 0;
}
