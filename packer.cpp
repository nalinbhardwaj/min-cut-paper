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

void dsu(int x, int y, vector<int>& P, vector<int>& sz)
{
	x = root(x, P), y = root(y, P);
	if(sz[x] > sz[y]) swap(x, y);
	P[x] = y;
	sz[y] += sz[x];
}

// Algorithm 1: Given an unweighted, undirected graph, compute packing of weight at least .4c
pair<double, vector<ptree>> packer(ppackergraph G)
{
	double W = 0;
	vector<vector<ppackeredge>> packerEdges;

	vector<int> P(G->n), sz(G->n);
	while(true)
	{
		sort(G->E.begin(), G->E.end(), [](ppackeredge a, ppackeredge b) { return *a->l.begin() < *b->l.begin(); });
		vector<ppackeredge> T;
		for(int i = 0;i < G->n;i++) P[i] = i, sz[i] = 1;
		for(auto it: G->E)
		{
			int a = it->u, b = it->v;
			if(root(a, P) != root(b, P))
			{
				dsu(a, b, P, sz);
				T.emplace_back(it);
				double newl = *it->l.begin()+1.0/(75.0*log(G->m));
				if(newl > 1)
				{
					pair<double, vector<ptree>> res;
					res.first = W;
					for(auto it: packerEdges)
					{
						vector<pedge> tmpedges;
						for(auto gt: it) tmpedges.emplace_back(new edge(gt->u, gt->v, gt->idx, gt->w));
						ptree tmp = new tree(G->n, tmpedges);
						res.second.emplace_back(tmp);
					}

					return res;
				}

				it->l.erase(it->l.begin());
				it->l.insert(newl);
			}
		}

		packerEdges.emplace_back(T);
		W += 1.0/(75.0*log(G->m));
	}
}