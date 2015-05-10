#include <cstdio>
#include <iostream>
#include <vector>
#include <queue>
#include <cstring>
#include <cassert>
#include <list>
#include <cmath>

#define TRACE(x) cerr << #x << " = " << x << endl
#define _ << " _ " <<

using namespace std;

const int MAX = 30020;
const int LIM = 600;

typedef pair<short int, short int> P;

int n;
int m;
int b[MAX];
int p[MAX];
int mod[MAX];
vector<short int> small[LIM][LIM];
vector<short int> big[MAX];

int main() {
  cin >> n >> m;
  for (int i=0; i<m; i++) { 
    cin >> b[i] >> p[i];
    mod[i] = b[i]%p[i];
  }

  vector<int> bitni(LIM, 0);
  for (int i=0; i<m; i++) 
    if (p[i] < LIM)
      bitni[p[i]] = 1;
  for (int j=1; j<LIM; j++)
    if (bitni[j])
      for (int i=0; i<m; i++) 
        small[j][b[i]%j].push_back(i);
  
  for (int i=0; i<m; i++) 
    big[b[i]].push_back(i);

  priority_queue<P, vector<P>, greater<P> > q;
  q.push({0, 0});
  static int done[MAX];
  static int smallest[MAX];
  memset(done, 0, sizeof done);
  memset(smallest, 0x3F3F3F3F, sizeof smallest);
  while (!q.empty()) {
    int d = q.top().first;
    int x = q.top().second;
    q.pop();
    if (done[x])
      continue ;
    done[x] = 1;
    if (x == 1) {
      cout << d << endl;
      return 0;
    }
    if (p[x] < LIM) {
      for (auto i : small[p[x]][mod[x]])
        if (!done[i]) {
          int newd = d+(abs(b[i]-b[x]))/p[x];
          if (newd < smallest[i])
            smallest[i] = newd, q.push({newd, i});
        }
    } else {
      for (int k=mod[x]; k<n; k+=p[x])
        for (auto i : big[k])
          if (!done[i]) {
            int newd = d+(abs(b[i]-b[x]))/p[x];
            if (newd < smallest[i])
              smallest[i] = newd, q.push({newd, i});
          }
    }
  }
  cout << -1 << endl;
  return 0;
}
