#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <iostream>
#include <algorithm>
#include <vector>
#include <deque>
using namespace std;

typedef long long llint;

const int MAXN = 500010;
const int inf = 2e9;

int N;
int A[MAXN];
int sum[MAXN];

struct monoqueue {
  deque<int> A, B;
  void pop() {
    if (A.front() == B.front()) B.pop_front();
    A.pop_front();
  }
  void push(int x) {
    while (!B.empty() && B.back() > x) B.pop_back();
    A.push_back(x);
    B.push_back(x);
  }
  int get() {
    return B.front(); 
  }
} mq;


int get_sum(int l, int r) {
  return sum[r] - sum[l];
}

int main(void) {
 
  scanf("%d", &N);
  for (int i = 0; i < N; ++i) scanf("%d", &A[i]);
  for (int i = 0; i < N; ++i) sum[i+1] = sum[i] + A[i];
  for (int i = 0; i < N; ++i) sum[i+N+1] = sum[i+N] + A[i];

  int ans = 0;
  const int n = (N+1) / 2;
  for (int i = 0; i < n; ++i) mq.push(get_sum(i, i + n));
  for (int i = 0; i < N; ++i) {
    ans = max(ans, mq.get());
    mq.pop();
    mq.push(get_sum(i+n, i+n+n));
  }
  printf("%d\n", ans);

  return 0;
}

