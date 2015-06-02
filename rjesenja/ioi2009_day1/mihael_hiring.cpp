#include <algorithm>
#include <cstdio>
#include <iostream>
#include <vector>
#include <queue>

using namespace std;

const int MAXN = 500010;

int n;
double w;

double s[MAXN];
double q[MAXN];

vector<int> V;
vector<int> sol_V;

priority_queue<int> Q;
priority_queue<pair<double, int> > Q2;

bool cmp(const int &i1, const int &i2) {
  return s[i1]/q[i1] < s[i2]/q[i2];
}

int main() {
  scanf("%d%lf", &n, &w);

  double maxiQ, maxiW;
  for (int i = 0; i < n; i++) {
    scanf("%lf%lf", &s[i], &q[i]);
    V.push_back(i);
  }

  sort(V.begin(), V.end(), cmp);

//  for (int i = 0; i < V.size(); i++) {
//    cout << s[V[i]] << " " << q[V[i]] << endl;
//  }

  int best = 0;
  double quals = 0;
  double cost = 0.0;
  double best_cost = 1e9;
  int best_index;
  for (int i = 0; i < V.size(); i++) {
    double max_qual = w / (s[V[i]] / q[V[i]]);
    quals += q[V[i]];
    Q.push(q[V[i]]);
//    cout << cost << " " << max_qual << endl;
    while (quals > max_qual) {
      quals -= Q.top(); Q.pop();
    }
//    cout << cost << " " << max_qual << endl;
//    cout << cost << " " << best << " " << best_index << " " << best_cost << " " << i << endl;
    cost = quals * (s[V[i]] / q[V[i]]);
    if (Q.size() > best || (Q.size() == best && cost < best_cost)) {
      best = Q.size();
      best_index = i;
      best_cost = cost;
    }
//    cout << cost << " " << best << " " << best_index << " " << best_cost << " " << i << endl;
  }

//  cout << "/////////" << endl;
//  cout << best << " " << best_index << endl;
  for (int i = 0; i <= best_index; i++) {
    Q2.push(make_pair(q[V[i]], V[i]));
//    cout << q[V[i]] << " " << V[i] << endl;
  }
  for (int i = 0; Q2.size() > best; i++) {
    Q2.pop();
  }

//  sort(sol_V.begin(), sol_V.end());
  printf("%d\n", best);
  for (int i = 0; i < best; i++) {
    printf("%d\n", Q2.top().second + 1);
    Q2.pop();
  }

  return 0;
}
