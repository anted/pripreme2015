#include <cstdlib>
#include <cstdio>
#include <set>
#include <map>
#include <vector>
using namespace std;

#include "grader.h"
#include "lang.h"

#define SZ 110

typedef long long llint;

int prev[1100000];

#define double long double

set<int> abeceda[SZ];
int abeceda_inv[100000][SZ];
map<llint, int> parovi[SZ];

map<pair<int, int>, vector<int>> pairs;

vector<double> sredi_abecedu(int *E) {
  static bool bio[100000];
  static vector<double> vec[SZ];
  static vector<double> tmp[SZ];
  vector<double> ans;

  static double kk[SZ][SZ];
  for (int i = 0; i < 56; ++i)
    for (int j = 0; j < SZ; ++j)
      kk[i][j] = 0;


  for (int i = 0; i < 100; ++i) {
    vec[i].clear();
    tmp[i].clear();
  }

  for (int i = 0; i < 100; ++i) {
    if (bio[E[i]]) continue;
    bio[E[i]] = 1;
    for (int j = 0; j < 56; ++j) {
      vec[i].push_back(abeceda_inv[E[i]][j]);
    }

    double sum = 0.000001;
    for (double &it: vec[i]) {
      it *= it;
      sum += it;
    }
    for (double &it: vec[i]) it /= sum; 
    
    for (int j = 0; j < 56; ++j)
      tmp[j].push_back(vec[i][j]);
  }

  for (int i = 0; i < 56; ++i) {
    double sum = 0;
    for (int j = 0; j < (int)tmp[i].size(); ++j) {
      tmp[i][j] *= tmp[i][j];
      sum += tmp[i][j];
    }
    ans.push_back(sum);
  }

  for (int i = 0; i < 100; ++i)
    bio[E[i]] = 0;
  
  return ans;
} 




vector<double> sredi_parove(int *E) {
  static vector<double> vec[SZ], tmp[SZ];
  vector<double> ans;
  for (int i = 0; i < 100; ++i) {
    vec[i].clear();
    tmp[i].clear();
  }

  for (int i = 1; i < 100; i += 2) {
    
    llint par = E[i-1]*60000LL + E[i];
    for (int j = 0; j < 56; ++j) {
      int cnt = parovi[j][par];
      vec[i].push_back(cnt); 
    }
    double sum = 0.000001;
    for (double &it: vec[i]) {
      sum += it;
    }
    for (double &it: vec[i]) it /= sum;

    for (int j = 0; j < 56; ++j) tmp[j].push_back(vec[i][j]);
  }

  for (int i = 0; i < 56; ++i) {
    double sum = 0;
    for (int j = 0; j < (int)tmp[i].size(); ++j) {
      tmp[i][j] *= tmp[i][j];
      sum += tmp[i][j];
    }
    ans.push_back(sum);
  }
  return ans;
}

int get_best(int *E) {
  static vector<double> f1, f2;
  
  f1 = sredi_abecedu(E);
  //f2.clear();
  //for (int i =0; i  <56; ++i) f2.push_back(0);
  f2 = sredi_parove(E);

//  for (int i = 0; i < 10; ++i) printf("%.2Lf ", f2[i]);

  int ret = 0;
  for (int i = 1; i < 56; ++i)
    if (f1[i] + 118*f2[i] > f1[ret] + 118*f2[ret])
      ret = i;

  return ret;
}

void excerpt(int *E) {

  static int haha = 0;
  ++haha;

  int x = get_best(E); 
  
  int lang = language(x);

  // dodaj u abeced
  for (int i = 0; i < 100; ++i) {
    abeceda[lang].insert(E[i]);
    abeceda_inv[E[i]][lang]++;
  }

  for (int i = 1; i < 100; ++i) {
    llint par = E[i-1]*60000LL + E[i];  
    parovi[lang][par]++;
  }



}
