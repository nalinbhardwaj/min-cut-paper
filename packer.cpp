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
#include <queue>
#include "graph.cpp"
using namespace std;

const double eps = 1/5.0;

class packeredge {
public:
	int u, v, idx;
	priority_queue<double> l;
	packeredge(int _u, int _v, int _idx, priority_queue<double>& _l) { u = _u, v = _v, idx = _idx, l = _l; }
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
	vector<pair<int, int>> sorter(G->m);
	vector<ppackeredge> href(G->m);
	for(auto it: G->E) href[it->idx] = it;
	while(true)
	{
		for(auto it: G->E) sorter[it->idx] = {-it->l.top(), it->idx};
		sort(sorter.begin(), sorter.end());
		vector<ppackeredge> T;
		for(int i = 0;i < G->n;i++) P[i] = i, sz[i] = 1;
		for(auto idx: sorter)
		{
			auto it = href[idx.second];
			int a = it->u, b = it->v;
			if(root(a, P) != root(b, P))
			{
				dsu(a, b, P, sz);
				T.emplace_back(it);
				double newl = -it->l.top()+(eps*eps)/(3.0*log(G->m));
				if(newl > 1)
				{
					pair<double, vector<ptree>> res;
					res.first = W;
					for(auto kt: packerEdges)
					{
						vector<pedge> tmpedges;
						for(auto gt: kt)
						{
							pedge tmp = new edge(gt->u, gt->v, gt->idx, 0);
							tmpedges.emplace_back(tmp);
						}
						
						ptree tmp = new tree(G->n, tmpedges);
						res.second.emplace_back(tmp);
					}

					return res;
				}

				it->l.pop();
				it->l.emplace(-newl);
			}
		}

		packerEdges.emplace_back(T);
		W += (eps*eps)/(3.0*log(G->m));
	}
}