#include <cstdio>
#include <cstring>
#include <algorithm>

using namespace std;

const int MAX = 200100;

int DEB = 0;
int broj=0;

typedef long long ll;

int p[MAX];
int l[MAX], r[MAX];
int pozsor[2*MAX], pozl[MAX], pozr[MAX];
int sorted[MAX], slj[MAX];
int n;
ll rje[2][MAX];
ll brojl[MAX], brojr[MAX], suml[MAX], sumr[MAX];

bool Manji(const int &a, const int &b)
{
  return l[a] + r[a] < l[b] + r[b];
}

void Stavi(ll fen[MAX], int pos, ll val)
{
  for (pos+=2; pos < MAX; pos += pos & -pos)
    fen[pos] += val;
}

ll Vrati(ll fen[MAX], int pos)
{
  ll ret=0;

  for (pos+=2; pos; pos -= pos & -pos)
    ret += fen[pos];

  return ret;
}

ll Eval(int ind)
{
  const int tpoz = pozsor[ind];
  const ll pravpoz = sorted[ind];

  if (broj == 5) {
    DEB = 1;
    Vrati(brojl, tpoz);
    DEB = 0;
  }

  ll ret = Vrati(brojr, tpoz) * 2 * pravpoz - Vrati(suml, tpoz) - Vrati(sumr, tpoz) + 
         Vrati(sumr, MAX-5) - Vrati(sumr, tpoz) +
         Vrati(suml, MAX-5) - Vrati(suml, tpoz) -
         (Vrati(brojl, MAX-5) - Vrati(brojl, tpoz)) * 2 * pravpoz;
  broj++;

  return ret;
}

void NadiSlj()
{
  for (int i=0; i<2*n;) {
    int kraj=i;
    for (; kraj<2*n && sorted[kraj] == sorted[i]; kraj++);
    int poc = sorted[i];
    for (; i < 2*n && sorted[i] == poc; i++)
      slj[i] = kraj;
  }
}

void Best(ll polje[MAX], int poc, int kor)
{
  for (int br=0, i=poc; br<n; br++, i+=kor) {
    int ind = p[i];
    Stavi(brojl, pozl[ind], 1);
    Stavi(brojr, pozr[ind], 1);
    Stavi(suml, pozl[ind], l[ind]);
    Stavi(sumr, pozr[ind], r[ind]);

    int lo = 0, hi = 2 * n;
    for (; lo < hi;) {
      int mid = (lo + hi) / 2;

      if (Eval(pozsor[mid]) < Eval(slj[mid]))
        hi = mid;
      else
        lo = slj[mid];
    }
    polje[i] = Eval(pozsor[lo]);
  }
}

int main()
{
  int brmost;
  ll rez=0;

  scanf("%d%d", &brmost, &n);

  for (int i=0; i<n; i++) {
    char c1, c2;
    int a, b;
    scanf(" %c%d %c%d", &c1, &a, &c2, &b);
    if (a > b)
      swap(a, b);

    if (c1 == c2) {
      n--;
      i--;
      rez += b - a;
    }
    else {
      l[i] = a;
      r[i] = b;
      sorted[2*i] = l[i];
      sorted[2*i+1] = r[i];
      rez++;
    }    
  }

  sort(sorted, sorted + 2 * n);
  for (int i=0; i<2*n; i++) {
    pozsor[i] = lower_bound(sorted, sorted + 2 * n, sorted[i]) - sorted;
    if (i < n) {
      pozl[i] = lower_bound(sorted, sorted + 2 * n, l[i]) - sorted;
      pozr[i] = lower_bound(sorted, sorted + 2 * n, r[i]) - sorted;
    }
  }

  for (int i=0; i<n; i++)
    p[i] = i;
  sort(p, p + n, Manji);

  NadiSlj();
  Best(rje[0], 0, 1);

  memset(brojl, 0, sizeof brojl);
  memset(brojr, 0, sizeof brojr);
  memset(suml, 0, sizeof suml);
  memset(sumr, 0, sizeof sumr);
  Best(rje[1], n-1, -1);

  if (!n)
    printf("%lld\n", rez);
  else if (brmost == 1)
    printf("%lld\n", rje[0][n-1] + rez);
  else {
    ll minn = 1e18;
    for (int i=0; i<n-1; i++)
      minn = min(minn, rje[0][i] + rje[1][i+1]);

    printf("%lld\n", minn + rez);
  }

  return 0;
}
