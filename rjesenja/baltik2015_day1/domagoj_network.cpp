#include <cstdio>
#include <algorithm>
#include <vector>

using namespace std;

typedef pair<int, int> P;
#define X first
#define Y second

const int MAX = 500100;

vector <int> V[MAX];
vector <P> R;

vector <int> Dfs(int node, int prosli)
{
  vector <int> T;

  if (V[node].size() == 1) {
    T.push_back(node);
    return T;
  }
  
  vector <vector<int> > S[2];

  for (int i=0; i<(int) V[node].size(); i++) {
    int nn = V[node][i];
    if (nn != prosli) {     
      vector <int> Tmp = Dfs(nn, node);
      S[Tmp.size()-1].push_back(Tmp);
    }
  }

  for (; S[1].size() > 1;) {
    vector <int> A = S[1].back();
    S[1].pop_back();
    vector <int> B = S[1].back();
    S[1].pop_back();

    int a = A.back();
    A.pop_back();
    int b = B.back();
    B.pop_back();

    R.push_back(P(a, b));

    S[0].push_back(A);
    S[0].push_back(B);
  }

  if (S[1].size() && S[0].size()) {
    int a = S[1][0].back();
    int b = S[0].back().back();

    R.push_back(P(a, b));

    S[1][0].pop_back();
    S[0].pop_back();
    S[0].push_back(S[1][0]);
    S[1].pop_back();
  }

  for (; S[0].size() > 2; ) {
    int a = S[0].back()[0];
    S[0].pop_back();
    int b = S[0].back()[0];
    S[0].pop_back();

    R.push_back(P(a, b));
  }

  for (int i=0; i<2; i++)
    for (int j=0; j<(int) S[i].size(); j++)
      for (int k=0; k<=i; k++)
        T.push_back(S[i][j][k]);

  return T;
}

int main()
{
  int n;

  scanf("%d", &n);

  for (int i=0; i<n-1; i++) {
    int a, b;
    scanf("%d%d", &a, &b); a--; b--;
    V[a].push_back(b);
    V[b].push_back(a);
  }

  int root = -1;
  for (int i=0; i<n; i++)
    if ((int) V[i].size() > 1)
      root = i;

  vector <int> T = Dfs(root, -1);
  if (T.size() == 2)
    R.push_back(P(T[0], T[1]));
  else
    R.push_back(P(root, T[0]));
  
  printf("%d\n", (int) R.size());
  for (int i=0; i<(int) R.size(); i++)
    printf("%d %d\n", R[i].X + 1, R[i].Y + 1);

  return 0;
}
