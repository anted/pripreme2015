#include "grader.h"
#include "cluedo.h"

#include <cstdio>
#include <set>
using namespace std;

const int A = 6;
const int B = 10;
const int C = 6;

set<int> Sa, Sb, Sc;

void init() {
  Sa.clear();
  Sb.clear();
  Sc.clear();
  for (int i = 1; i <= A; ++i) Sa.insert(i);
  for (int i = 1; i <= B; ++i) Sb.insert(i);
  for (int i = 1; i <= C; ++i) Sc.insert(i);
}

void Solve() {
  init();
  while (true) {
    set<int>::iterator it;
    int a, b, c;
    it = Sa.begin(); a = *it;
    it = Sb.begin(); b = *it;
    it = Sc.begin(); c = *it;
    int t = Theory(a, b, c);
    if (t == 0) return;
    if (t == 1) Sa.erase(Sa.begin());
    if (t == 2) Sb.erase(Sb.begin());
    if (t == 3) Sc.erase(Sc.begin());
  }
}
