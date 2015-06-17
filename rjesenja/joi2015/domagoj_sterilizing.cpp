#include <cstdio>
#include <algorithm>
#include <vector>
#include <set>

using namespace std;

typedef long long ll;

const int MAX = 100050;

ll fen[MAX];
int p[MAX];
set <int> S;

void Stavi(int pos, int val)
{
  for (pos++; pos < MAX; pos += pos & -pos)
    fen[pos] += val;
}

ll Vrati(int pos)
{
  ll ret=0;
  for (pos++; pos; pos -= pos & -pos)
    ret += fen[pos];
  return ret;
}

int main()
{
  int n, q, k;

  scanf("%d%d%d", &n, &q, &k);

  for (int i=0; i<n; i++) {
    scanf("%d", &p[i]);
    Stavi(i, p[i]);
    if (k > 1)
      S.insert(i);
  }

  for (int i=0; i<q; i++) {
    int st;
    scanf("%d", &st);

    if (st == 1) {
      int poz, nval;
      scanf("%d%d", &poz, &nval); poz--;
      Stavi(poz, nval - p[poz]);
      p[poz] = nval;
      if (k > 1 && nval > 0)
        S.insert(poz);
    }
    else if (st == 2) {
      int l, r;
      scanf("%d%d", &l, &r); l--; r--;
      
      if (k > 1) {
        set <int> :: iterator it = S.lower_bound(l);
       for (; it != S.end() && *it <= r;) {
          int nval = p[*it] / k;
          Stavi(*it, nval - p[*it]);
          p[*it] = nval;
          if (!nval) {
            set <int> :: iterator it2 = it++;
            S.erase(it2);
          }
          else
            it++;
        }
      }
    }
    else {
      int l, r;
      scanf("%d%d", &l, &r); l--; r--;
      printf("%lld\n", Vrati(r) - Vrati(l-1)); 
    }
  }

  return 0;
}
