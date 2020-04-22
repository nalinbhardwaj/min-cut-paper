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
using namespace std;

class edge {
public:
	int u, v, idx;
	double w;
	edge(int _u, int _v, int _idx, double _w) { u = _u, v = _v, idx = _idx, w = _w; }
};

typedef edge* pedge;

class graph {
public:
	int n, m;
	vector<vector<pedge>> adj;
	vector<pedge> E;
	graph(int _n, int _m, vector<pedge>& _E)
	{
		n = _n, m = _m, E = _E;
		adj.clear(); adj.resize(n);
		for(auto it: E) adj[it->u].push_back(it), adj[it->v].push_back(it);
	}
};

typedef graph* pgraph;

class tree : public graph {
public:
	tree(int _n, vector<pedge>& _E) : graph(_n, _n-1, _E) { }
};

typedef tree* ptree;