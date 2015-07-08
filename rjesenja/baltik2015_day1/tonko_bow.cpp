#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <iostream>
#include <algorithm>
#include <vector>
using namespace std;

typedef long long llint;

llint dp[12][350][11][11];

int N;
int A[15];
char S[30];

struct State {
  string conf;
  vector<int> scores;
  int sum;
};

vector<State> states[15];

llint solve(int pos, int score, int n1, int n2) {
  if (score < 0) return 0;
  if (A[pos] != -1 && A[pos] != score) return 0;

  if (pos == 0) return 1;

  llint &ret = dp[pos][score][n1][n2];
  if (ret != -1) return ret;
  ret = 0;

  for (State &st : states[pos]) {
    const char *ss = st.conf.c_str();
    
    int nn1, nn2;
    if (st.scores.size() == 1) nn1 = st.scores[0], nn2 = n1;
    else nn1 = st.scores[0], nn2 = st.scores[1];
    
    
    if (ss[0] == 'x') {
      ret += solve(pos-1, score - st.sum - n1 - n2, nn1, nn2);
    }
    else if (ss[1] == '/') {
      ret += solve(pos-1, score - st.sum - n1, nn1, nn2);
    }
    else {
      ret += solve(pos-1, score - st.sum, nn1, nn2);
    }
  }

  return ret;
}    

inline bool gen_check(const string& w) {
  const char *s = w.c_str();
  if ((int)w.size() == 2) {
    if (s[0] == 'x') return s[1] == '-';
    if (isdigit(s[0]) && isdigit(s[1])) return s[0]+s[1]-'0'-'0' < 10;
    return isdigit(s[0]) && s[1] == '/';
  }
  if (s[0] == 'x' && s[1] == 'x') return isdigit(s[2]) || s[2] == 'x';
  if (s[0] == 'x') {
    if (isdigit(s[1]) && isdigit(s[2])) return s[1]+s[2]-'0'-'0' < 10;
    return isdigit(s[1]) && s[2] == '/';
  }
  if (isdigit(s[0]) && s[1] == '/') return s[2] != '-' && s[2] != '/';
  if (isdigit(s[0]) && isdigit(s[1])) return s[2] == '-' && s[0]+s[1]-'0'-'0' < 10;
  return 0;
}

inline State make_state(const string& s) {
  State ret;
  ret.conf = s;
  ret.sum = 0;
  for (int i = 0; i < (int)s.size(); ++i) {
    if (isdigit(s[i])) ret.scores.push_back(s[i] - '0');
    if (s[i] == '/') ret.scores.push_back(10 - s[i-1] + '0');
    if (s[i] == 'x') ret.scores.push_back(10);
  }
  for (int it: ret.scores) ret.sum += it;
  return ret;
}

void gen(int pos, int curr, int r) {
  static string str = "";
  if (curr == r) {
    if (gen_check(str))
      states[pos].push_back(make_state(str));
    return;
  }
  static const string qmask = "0123456789-/x";
  if (S[curr] != '?') {
    str += S[curr];
    gen(pos, curr+1, r);
    str.pop_back();
  }
  else {
    for (char c: qmask) {
      str += c;
      gen(pos, curr+1, r);
      str.pop_back();
    }
  }
}

void load() {
  scanf("%d", &N);
  scanf("%s", S+2);
  S[0] = S[1] = '0';
  A[0] = 0;
  for (int i = 1; i <= N; ++i) {
    scanf("%d", &A[i]);
  }
  // a sada cemo generirati sljedeca stanja
  for (int i = 0; i <= N; ++i) {
    states[i].clear();
    int l = 2*i;
    int r = 2*(i+1) + (int)(i == N);
    gen(i, l, r);
  }
}

int main(void) {
  int T;
  scanf("%d", &T);
  while (T--) {
    load();
    memset(dp, -1, sizeof dp);
    llint ans = 0;
    
    for (int i = 0; i <= 300; ++i)
      ans += solve(N, i, 0, 0);
    
    printf("%lld\n", ans);
  }
  return 0;
}

