#include <cstdio>
#include <algorithm>
#include <set>

using namespace std;

typedef pair<int, int> P;
#define X first
#define Y second

const int MAX = 300100;
const int TOUR = 1<<19;
const int INF = 0x3f3f3f3f;

struct node {
  node *l, *r;
  P val; //minn, poz
} *root[MAX];

P Merge(P a, P b)
{
  if (a.X < b.X)
    return a;
  return b;
}

int st[MAX], kojeg[MAX];
int polje[MAX];

node* Init(int lo, int hi)
{
  node *p = new node;
  p->val = P(INF, -1);

  if (lo + 1 == hi)
    return p;

  p->l = Init(lo, (lo+hi) / 2);
  p->r = Init((lo+hi) / 2, hi);

  return p;
}

node* Stavi(node *prij, int lo, int hi, int pos, P val)
{
  if (lo >= pos + 1 || hi <= pos)
    return prij;

  node *p = new node;

  if (lo == pos && hi == pos + 1) {
    p->val = val;
    return p;
  }

  p->l = Stavi(prij->l, lo, (lo+hi) / 2, pos, val);
  p->r = Stavi(prij->r, (lo+hi) / 2, hi, pos, val);
  p->val = Merge(p->l->val, p->r->val);

  return p;
}

P Nadi(node* p, int lo, int hi, int gran)
{
  if (lo == hi - 1)
    return p->val;

  if (p->r->val.X < gran)
    return Nadi(p->r, (lo+hi) / 2, hi, gran);
  return Nadi(p->l, lo, (lo+hi) / 2, gran);
}

int main()
{
  int n;

  scanf("%d", &n);

  root[0] = Init(0, TOUR);
  root[1] = Stavi(root[0], 0, TOUR, 1, P(0, 1));

  for (int i=2; i<n+2; i++) {
    scanf("%d", &polje[i]);

    if (polje[i] > 0)
      root[i] = Stavi(root[i-1], 0, TOUR, i, P(0, i));
    else {
      P tmp = Nadi(root[i-1], 0, TOUR, -polje[i]);
      root[i] = Stavi(root[tmp.Y-1], 0, TOUR, i, P(-polje[i], i));
    }

    printf("%d\n", polje[root[i]->val.Y]);
  }

  return 0;
}
