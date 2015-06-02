#include <cstdio>
#include <algorithm>

using namespace std;

typedef long double llf;
typedef long long ll;

const int LOG = 19, MAX = 1<<(LOG+1);
const llf EPS = 1e-12;
const int BAZA = 20001;

struct str {
  int s, q, ind;
} p[MAX];

bool operator < (const str &a, const str &b)
{
  return a.q < b.q;
}

int konst[LOG][MAX];
ll imam;
int n;
int pot[MAX];
ll pref[MAX];

llf Vrij(int x)
{
  return (llf) (x / BAZA) / (x % BAZA);
}

void GenKonst()
{
  for (int i=1; i<=n; i++)
    konst[0][i] = p[i].s * BAZA + p[i].q;

  for (int i=1, raz=1; i<LOG; i++, raz *= 2)
    for (int j=1; j<=n; j++) {
      if (Vrij(konst[i-1][j]) > Vrij(konst[i-1][j+raz]))
        konst[i][j] = konst[i-1][j];
      else
        konst[i][j] = konst[i-1][j+raz];
    }
}

void GenPot()
{
  for (int i=1, um=1, j=0; i<MAX; i++) {
    pot[i] = j;
    if (um * 2 == i) {
      um *= 2;
      j++;
    }
  }
}

llf Vrati(int l, int r)
{
  int ind = pot[r - l + 1];
  return max(Vrij(konst[ind][l]), Vrij(konst[ind][r - (1<<ind) + 1]));
}

void GenPref()
{
  for (int i=1; i<=n; i++)
    pref[i] = pref[i-1] + p[i].q;
}

llf Eval(int poc, int kraj)
{
  llf koef = Vrati(poc, kraj);
  return koef * (pref[kraj] - pref[poc-1]);
}

int main()
{
  int kolko = 0, poc = 0;
  ll plat = 1e15;

  scanf("%d%lld", &n, &imam);

  for (int i=1; i<=n; i++) {
    scanf("%d%d", &p[i].s, &p[i].q);
    p[i].ind = i;
  }

  sort(p, p+n);

  GenPot();
  GenKonst();
  GenPref();

  for (int i=1; i<=n; i++) {
    int lo=i, hi=n;

    for (; lo < hi; ) {
      int mid = (lo + hi + 1) / 2;
      if (Eval(i, mid) - EPS < (llf) imam)
        lo = mid;
      else
        hi = mid - 1;
    }

    llf tmp = Eval(i, lo);
    if (lo - i + 1 > kolko || (lo - i + 1 == kolko && tmp < plat)) {
      poc = i;
      kolko = lo - i + 1;
      plat = tmp;
    }
  }
  
  printf("%d\n", kolko);
  for (int i=poc; i<poc + kolko; i++)
    printf("%d\n", p[i].ind);

  return 0;
}
