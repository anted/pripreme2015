#include <cstdio>
#include <algorithm>

using namespace std;

typedef long long ll;

const int TOUR = 1<<19;
const ll INF = 1e18;

ll t[2*TOUR];
ll pref[TOUR];
int n;

void Stavi(int pos, ll val)
{
  for (t[pos+TOUR] = val, pos = (pos + TOUR) / 2; pos; pos /= 2)
    t[pos] = min(t[2*pos], t[2*pos+1]);
}

ll Vrati(int pos, int lo, int hi, int begin, int end)
{
  if (lo >= end || hi <= begin)
    return INF;
  if (lo >= begin && hi <= end)
    return t[pos];
  return min(Vrati(2*pos+0, lo, (lo+hi)/2, begin, end),
             Vrati(2*pos+1, (lo+hi)/2, hi, begin, end) );
}

ll Vrati(int poc, int kraj)
{
  if (kraj >= poc)
    return Vrati(1, 0, TOUR, poc, kraj + 1);
  return min(Vrati(1, 0, TOUR, 0, kraj + 1),
             Vrati(1, 0, TOUR, poc, n));
}

ll Sum(int poc, int kraj)
{
  if (kraj >= poc)
    return pref[kraj+1] - pref[poc];
  return pref[kraj+1] + pref[n] - pref[poc];
}

int Pos(int x)
{
  return (x + n) % n;
}

int main()
{
  int dulj;
  ll rje=0;

  scanf("%d", &n);
  dulj = (n + 1) / 2;

  for (int i=0; i<n; i++) {
    int tmp;
    scanf("%d", &tmp);
    pref[i+1] = pref[i] + tmp;
  }

  for (int i=0; i<n; i++) {
    ll tmp = Sum(i, Pos(i + dulj - 1));
    Stavi(i, tmp);
  }

  for (int i=0; i<n; i++)
    rje = max(rje, Vrati(i, Pos(i + dulj - 1)));

  printf("%lld\n", rje);

  return 0;
}
