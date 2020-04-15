#pragma once

#include <iostream>
#include <cstdio>
#include <cmath>
#include <vector>
#include <map>
#include <set>
#include <random>
#include <cstdlib>
#include <algorithm>
#include "graph.cpp"
using namespace std;

class packeredge {
public:
	int u, v, idx;
	double w;
	multiset<double> l;
	packeredge(int _u, int _v, int _idx, double _w, multiset<double>& _l) { u = _u, v = _v, idx = _idx, w = _w, l = _l; }
};

typedef packeredge* ppackeredge;

class packergraph {
public:
	int n, m;
	vector<ppackeredge> E;
	packergraph(int _n, int _m, vector<ppackeredge>& _E)
	{
		n = _n, m = _m, E = _E;
	}
};

typedef packergraph* ppackergraph;

inline int root(int x, vector<int>& P) { return (P[x]==x)?x:(P[x]=root(P[x], P)); }

void dsu(int x, int y, vector<int>& P)
{
	x = root(x, P), y = root(y, P);
	P[x] = y;
}

typedef long long int lli;

const lli MOD = lli(1e9)+7;

lli hsh(const vector<ppackeredge>& T, lli r)
{
	lli res = 1ll;
	for(auto it: T)
	{
		res *= lli(r+it->idx)%MOD;
		res %= MOD;
	}
	return res;
}

// Algorithm 1: Given an unweighted, undirected graph, compute packing of weight at least .4c
pair<double, vector<ptree>> packer(ppackergraph G)
{
	double maxl = 0;
	lli r = rand()%MOD;
	map<lli, vector<ppackeredge>> mp;
	map<lli, double> wt;

	while(maxl < 1.0)
	{
		sort(G->E.begin(), G->E.end(), [](ppackeredge a, ppackeredge b) { return *a->l.begin() < *b->l.begin(); });
		vector<ppackeredge> T;
		vector<int> P(G->n);
		for(int i = 0;i < G->n;i++) P[i] = i;
		for(auto it: G->E)
		{
			int a = it->u, b = it->v;
			if(root(a, P) != root(b, P))
			{
				dsu(a, b, P);
				T.push_back(it);
				double newl = *it->l.begin()+1.0/(75.0*log(G->m));
				it->l.erase(it->l.begin());
				it->l.insert(newl);
				maxl = max(maxl, newl);
			}
		}
		lli h = hsh(T, r);

		if(wt.find(h) == wt.end()) mp[h] = T;
		wt[h] += 1.0/(75.0*log(G->m));
	}

	pair<double, vector<ptree>> res;

	for(auto it: mp)
	{
		vector<pedge> tmpedges;
		for(auto gt: it.second) tmpedges.push_back(new edge(gt->u, gt->v, gt->idx, gt->w));
		ptree tmp = new tree(G->n, wt[it.first], tmpedges);
		res.first += tmp->wt;
		res.second.push_back(tmp);
	}

	return res;
}