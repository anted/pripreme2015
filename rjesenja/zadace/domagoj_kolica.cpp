#include <cstdio>
#include <cstring>
#include <algorithm>
#include <set>

using namespace std;

const int MAX = 500100, INF = 0x3f3f3f3f;

struct str {
  int x, y, ind;
  str (int _x, int _y, int _ind) : x(_x), y(_y), ind(_ind) {}
  str () {}
};

bool operator < (const str &a, const str &b)
{
  if (a.x != b.x)
    return a.x < b.x;
  return a.y < b.y;
}

struct event {
  int ind1, ind2, t;
  event (int _ind1, int _ind2, int _t) : ind1(_ind1), ind2(_ind2), t(_t) {}
  event () {}
};

bool operator < (const event &a, const event &b)
{
  if (a.t != b.t)
    return a.t < b.t;
  if (min(a.ind1, a.ind2) != min(b.ind1, b.ind2))
    return min(a.ind1, a.ind2) < min(b.ind1, b.ind2);
  return a.ind1 + a.ind2 < b.ind1 + b.ind2;
}

set <str> Uz[MAX], Ur[MAX], Rz[MAX], Rr[MAX], Dz[MAX], Dr[MAX], Lz[MAX], Lr[MAX];
set <str> U[MAX], R[MAX], D[MAX], L[MAX];
set <event> E;
vector <int> Raz, Zbroj, X, Y;
int xx[MAX], yy[MAX], smj[MAX]; //0 - U, 1 - R, 2 - D, 3 - L
int a[MAX], b[MAX], c[MAX];
int nestao[MAX];
char smjerovi[10] = "rslj";

int Presjek(int ind1, int ind2)
{
  int broj = c[ind2] * b[ind1] - c[ind1] * b[ind2];
  int naz = a[ind1] * b[ind2] - a[ind2] * b[ind1];
  
  if (naz) {
    if (broj % naz)
     return -1;

    int x = broj / naz;

    broj = a[ind1] * c[ind2] - a[ind2] * c[ind1];
    naz = a[ind2] * b[ind1] - a[ind1] * b[ind2];

    if (broj % naz)
      return -1;

    int y = broj / naz;

    return 2 * (abs(xx[ind1] - x) + abs(yy[ind1] - y));
  }
  
  return abs(xx[ind1] - xx[ind2]) + abs(yy[ind1] - yy[ind2]);
}

void Stavi(const int &ind, set <str> :: iterator it, const set <str> &S)
{
  if (it == S.end())
    return;

  int ind2 = it->ind;
  int t = Presjek(ind, ind2);

  if (t > 0)
    E.insert(event(ind, ind2, t));
}

set <str> :: iterator Prosli(set <str> &S, set <str> :: iterator it)
{
  if (it != S.begin())
    return --it;
  return S.end();
}

void Potrpaj(int ind)
{
  int x = xx[ind];
  int y = yy[ind];
  int indr = lower_bound(Raz.begin(), Raz.end(), xx[ind] - yy[ind]) - Raz.begin();
  int indz = lower_bound(Zbroj.begin(), Zbroj.end(), xx[ind] + yy[ind]) - Zbroj.begin();
  int indx = lower_bound(X.begin(), X.end(), xx[ind]) - X.begin();
  int indy = lower_bound(Y.begin(), Y.end(), yy[ind]) - Y.begin();
  str ja = str(x, y, -1);

  if (!smj[ind]) {
    Stavi(ind, D[indx].lower_bound(ja), D[indx]);
    Stavi(ind, Prosli(Rz[indz], Rz[indz].lower_bound(ja)), Rz[indz]);
    Stavi(ind, Lr[indr].lower_bound(ja), Lr[indr]);
  }
  else if (smj[ind] == 1) {
    Stavi(ind, L[indy].lower_bound(ja), L[indy]);
    Stavi(ind, Uz[indz].lower_bound(ja), Uz[indz]);
    Stavi(ind, Dr[indr].lower_bound(ja), Dr[indr]);
  }
  else if (smj[ind] == 2) {
    Stavi(ind, Prosli(U[indx], U[indx].lower_bound(ja)), U[indx]);
    Stavi(ind, Lz[indz].lower_bound(ja), Lz[indz]);
    Stavi(ind, Prosli(Rr[indr], Rr[indr].lower_bound(ja)), Rr[indr]);
  }
  else {
    Stavi(ind, Prosli(R[indy], R[indy].lower_bound(ja)), R[indy]);
    Stavi(ind, Prosli(Dz[indz], Dz[indz].lower_bound(ja)), Dz[indz]);
    Stavi(ind, Prosli(Ur[indr], Ur[indr].lower_bound(ja)), Ur[indr]);
  }
}

void Brisi(int ind)
{
  int x = xx[ind];
  int y = yy[ind];
  int indr = lower_bound(Raz.begin(), Raz.end(), xx[ind] - yy[ind]) - Raz.begin();
  int indz = lower_bound(Zbroj.begin(), Zbroj.end(), xx[ind] + yy[ind]) - Zbroj.begin();
  int indx = lower_bound(X.begin(), X.end(), xx[ind]) - X.begin();
  int indy = lower_bound(Y.begin(), Y.end(), yy[ind]) - Y.begin();  
  str ja = str(x, y, ind);

  if (!smj[ind]) {
    U[indx].erase(ja);
    Uz[indz].erase(ja);
    Ur[indr].erase(ja);
  }
  else if (smj[ind] == 1) {
    R[indy].erase(ja);
    Rz[indz].erase(ja);
    Rr[indr].erase(ja);
  }
  else if (smj[ind] == 2) {
    D[indx].erase(ja);
    Dz[indz].erase(ja);
    Dr[indr].erase(ja);
  }
  else {
    L[indy].erase(ja);
    Lz[indz].erase(ja);
    Lr[indr].erase(ja);
  }
}

void Update1(int ind, set <str> &S)
{
  set <str> :: iterator poc = S.lower_bound(str(xx[ind], yy[ind], ind));

  if (poc != S.end())
    Potrpaj(poc->ind);
  if (poc != S.begin()) {
    set <str> :: iterator it = poc;
    it--;
    Potrpaj(it->ind);
  }
  if (poc != S.end()) {
    set <str> :: iterator it = poc;
    it++;
    Potrpaj(it->ind);
  }
}

void Update(int ind)
{
  int indr = lower_bound(Raz.begin(), Raz.end(), xx[ind] - yy[ind]) - Raz.begin();
  int indz = lower_bound(Zbroj.begin(), Zbroj.end(), xx[ind] + yy[ind]) - Zbroj.begin();
  int indx = lower_bound(X.begin(), X.end(), xx[ind]) - X.begin();
  int indy = lower_bound(Y.begin(), Y.end(), yy[ind]) - Y.begin();  

  if (!smj[ind] || smj[ind] == 2) {
    Update1(ind, U[indx]);
    Update1(ind, D[indx]);
    Update1(ind, Rz[indz]);
    Update1(ind, Rr[indr]);
    Update1(ind, Lz[indz]);
    Update1(ind, Lr[indr]);
  }
  else {
    Update1(ind, L[indy]);
    Update1(ind, R[indy]);
    Update1(ind, Uz[indz]);
    Update1(ind, Ur[indr]);
    Update1(ind, Dz[indz]);
    Update1(ind, Dr[indr]);
  }
}

int main()
{
  int n;

  scanf("%d", &n);

  for (int i=0; i<n; i++) {
    char s[10];
    scanf("%d%d %s", &xx[i], &yy[i], s);

    for (int j=0; j<4; j++)
      if (s[2] == smjerovi[j])
        smj[i] = j;

    if (!smj[i] || smj[i] == 2) {
      a[i] =  1;
      b[i] = 0;
      c[i] = -xx[i];
    }
    else {
      a[i] = 0;
      b[i] = 1;
      c[i] = -yy[i];
    }

    Raz.push_back(xx[i] - yy[i]);
    Zbroj.push_back(xx[i] + yy[i]);
    X.push_back(xx[i]);
    Y.push_back(yy[i]);
  }

  sort(Raz.begin(), Raz.end());
  sort(Zbroj.begin(), Zbroj.end());
  sort(X.begin(), X.end());
  sort(Y.begin(), Y.end());

  for (int i=0; i<n; i++) {
    int indr = lower_bound(Raz.begin(), Raz.end(), xx[i] - yy[i]) - Raz.begin();
    int indz = lower_bound(Zbroj.begin(), Zbroj.end(), xx[i] + yy[i]) - Zbroj.begin();
    int indx = lower_bound(X.begin(), X.end(), xx[i]) - X.begin();
    int indy = lower_bound(Y.begin(), Y.end(), yy[i]) - Y.begin();
    str tmp = str(xx[i], yy[i], i);

    if (!smj[i]) {
      Uz[indz].insert(tmp);
      Ur[indr].insert(tmp);
      U[indx].insert(tmp);
    }
    else if (smj[i] == 1) {
      Rz[indz].insert(tmp);
      Rr[indr].insert(tmp);
      R[indy].insert(tmp);
    }
    else if (smj[i] == 2) {
      Dz[indz].insert(tmp);
      Dr[indr].insert(tmp);
      D[indx].insert(tmp);
    }
    else {
      Lz[indz].insert(tmp);
      Lr[indr].insert(tmp);
      L[indy].insert(tmp);
    }
  }

  for (int i=0; i<n; i++)
    Potrpaj(i);
  
  memset(nestao, INF, sizeof nestao);

  for (; !E.empty();) {
    int t = E.begin()->t;

    for (; !E.empty();) {
      event tmp = *(E.begin());    

      if (tmp.t != t)
        break;

      if (nestao[tmp.ind1] < t || nestao[tmp.ind2] < t || (nestao[tmp.ind1] == t && nestao[tmp.ind2] == t)) {
        E.erase(E.begin());
        continue;
      }
      
      nestao[tmp.ind1] = nestao[tmp.ind2] = t;

      Brisi(tmp.ind1);
      Brisi(tmp.ind2);
      Update(tmp.ind1);
      Update(tmp.ind2);

      E.erase(E.begin());
    }
  }

  int da = 0;
  for (int i=0; i<n; i++)
    if (nestao[i] == INF) {
      da = 1;
      printf("%d\n", i + 1);
    }

  if (!da)
    printf("nema\n");

  return 0;
}
