#include <stdlib.h>
#include <stdio.h>

#include "grader.h"
#include "lang.h"

#include <vector>
#include <cstdio>
#include <algorithm>
#include <cstring>
#include <unordered_map>
#include <map>

#define FOR(a,b,c) for (int c = (a), _for = (b); c < _for; ++c)
#define REP(n) FOR(0, n, _rep)
#define pb push_back
#define pii pair < int, int >
#define x first
#define y second
#define ll long long
#define ull unsigned ll

using namespace std;

map < ull, vector < int > > M2;
map < ull, vector < int > > M3;
map < ull, vector < int > > M4;

ull F(ull a, ull b){
	return (a << 16) + b;
}
ull F(ull a, ull b, ull c){
	return (a << 32) + (b << 16) + c;
}
ull F(ull a, ull b, ull c, ull d){
	return (a << 48) + (b << 32) + (c << 16) + d;
}

void excerpt(int *E) {
	vector < int > V(E, E + 100);
	vector < double > Score(56, 0.0);
	FOR(0, V.size() - 3, i){
		ull t = F(V[i], V[i + 1], V[i + 2], V[i + 3]);
		if (M4.count(t) == 0) M4[t] = vector < int >(56, 0);
		vector < int > &vt = M4[t];
		int S = 0;
		for (auto x : vt) S += x;
		FOR(0, 56, i) if (S) Score[i] += (double)vt[i] / S;
	}
	FOR(0, V.size() - 2, i){
		ull t = F(V[i], V[i + 1], V[i + 2]);
		if (M3.count(t) == 0) M3[t] = vector < int >(56, 0);
		vector < int > &vt = M3[t];
		int S = 0;
		for (auto x : vt) S += x;
		FOR(0, 56, i) if (S) Score[i] += (double)vt[i] / S;
	}
	FOR(0, V.size() - 1, i){
		ull t = F(V[i], V[i + 1]);
		if (M2.count(t) == 0) M2[t] = vector < int >(56, 0);
		vector < int > &vt = M2[t];
		int S = 0;
		for (auto x : vt) S += x;
		FOR(0, 56, i) if (S) Score[i] += (double)vt[i] / S;
	}
	int Best = 0;
	FOR(0, 56, i) if (Score[i] > Score[Best]) Best = i;
	int Cor = language(Best);
	FOR(0, V.size() - 3, i){
		ull t = F(V[i], V[i + 1], V[i + 2], V[i + 3]);
		++M4[t][Cor];
	}
	FOR(0, V.size() - 2, i){
		ull t = F(V[i], V[i + 1], V[i + 2]);
		++M3[t][Cor];
	}
	FOR(0, V.size() - 1, i){
		ull t = F(V[i], V[i + 1]);
		++M2[t][Cor];
	}
}
