#include "grader.h"
#include "encoder.h"
#include "decoder.h"

#include <cstdio>
#include <queue>
#include <vector>
using namespace std;

// No communication/shared data between two namespaces please.
namespace Encoder {
// Your encoder implementation goes here.
  void encode_num(long long x, int y) {
    for (int i = 0; i < y; ++i) {
      encode_bit(x & 1);
      x >>= 1;
    } 
  }

  int N, H;

  vector<int> V[1111];;
  int par[1111];
  bool bio[1111];
  int dist[36][1111];
  long long haha[1111];


  void dfs(int x, int prev) {
    if (bio[x] == 1) return;
    bio[x] = 1;
    
    if (x != 0) {
      long long tmp = 0;
      for (int i = 0; i < H; ++i) {
        int d = 1 + dist[i][x] - dist[i][prev];
        tmp *= 3;
        tmp += d;
      }
      par[x] = prev;
      haha[x] = tmp;
    }

    for (int y: V[x])
      dfs(y, x);
  }

  void encode(int nv, int nh, int ne, int *v1, int *v2){
    N = nv;
    H = nh;
    for (int i = 0; i < ne; ++i) {
      V[v1[i]].push_back(v2[i]);
      V[v2[i]].push_back(v1[i]);
    }
    
    for (int i = 0; i < nh; ++i) {
      for (int j = 0; j < nv; ++j) dist[i][j] = -1;
      dist[i][i] = 0;
      queue<int> q;
      for (q.push(i); !q.empty(); q.pop()) {
        int x = q.front();
        for (int y: V[x]) {
          if (dist[i][y] != -1) continue;
          dist[i][y] = dist[i][x] +1;
          q.push(y);
        }
      }
    }
    
    for (int i = 0; i < H; ++i) {
      encode_num(dist[i][0], 10);
    }

    dfs(0, -1);

    for (int i = 1; i < N; ++i) {
      encode_num(par[i], 10);
      encode_num(haha[i], 58);
    }

    return;
  }
};

// No communication/shared data between two namespaces please.
namespace Decoder {
// Your decoder implementation goes here.
  long long decode_num(int k) {
    long long ret = 0;
    for (int i = 0; i < k; ++i)
      ret += decode_bit() * (1LL << i);
    return ret;
  }
  
  int N, H;
  int par[1111];
  long long val[1111];

  void decode(int nv, int nh) {
    N = nv, H = nh;
    static int ans[1111][1111];
    for (int i = 0; i < H; ++i)
      ans[i][0] = (int)decode_num(10);
    
    static vector<int> v[1111];
    for (int i = 1; i < N; ++i) {
      par[i] = decode_num(10);
      val[i] = decode_num(58);
      v[par[i]].push_back(i);
    }

    static queue<int> q;
    for (q.push(0); !q.empty(); q.pop()) {
      int x = q.front();
      if (x != 0) {
        int y = par[x];
        for (int i = H-1; i >= 0; --i) {
          int d = (val[x] % 3) - 1;
          ans[i][x] = ans[i][y] + d;
          val[x] /= 3;
        }
      }
      for (int y: v[x]) q.push(y);
    }  

    for (int i = 0; i < H; ++i)
      for (int j = 0; j < N; ++j)
        hops(i,j,ans[i][j]);
  }
};

// Called by grader. Do not modify.
void encode(int nv, int nh, int ne, int *v1, int *v2) {
  Encoder::encode(nv, nh, ne, v1, v2);
}
void decode(int nv, int nh) {
  Decoder::decode(nv, nh);
}
