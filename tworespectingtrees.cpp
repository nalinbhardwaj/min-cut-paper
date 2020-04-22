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
#include "packer.cpp"
using namespace std;

const double inf = 1e9+5;

double compute_U(const pgraph G)
{
	double res = inf;
	for(auto it: G->adj)
	{
		double tmp = 0;
		for(auto gt: it) tmp += gt->w;
		res = min(res, tmp);
	}
	return res;
}

int binom(int trials, double p)
{
	default_random_engine generator;
	binomial_distribution<int> distribution(trials, p);
	return distribution(generator);
}

vector<ptree> sample(const vector<ptree>& packing, double d, int n)
{
	default_random_engine generator;
	uniform_int_distribution<int> distribution(0, int(packing.size())-1);
	vector<ptree> res;
	for(int i = 0;i < ceil(36.43*d*log(n));i++) res.push_back(packing[distribution(generator)]);
	return res;
}

const double eps = 1e-11;

// Algorithm 2: Obtain spanning trees with probability of success 1 − 1/(G->n)^d
vector<ptree> tworespectingtrees(double d, pgraph _G)
{
	vector<pedge> Edash;
	for(auto it: _G->E) Edash.push_back(new edge(it->u, it->v, it->idx, it->w));

	pgraph Gdash = new graph(_G->n, _G->m, Edash);

	double b = 3.0*6*6*(d+2.0)*log(Gdash->n);

	for(auto it: Gdash->E)
	{
		it->w = round(it->w*100);
	}

	double c = compute_U(Gdash);

	bool lastrun = 0;
	while(true)
	{
		//cout << c << " " << lastrun << " " << b/c << "\n";
		vector<ppackeredge> HE;
		double p = min(1.0, b/c);
		for(auto it: Gdash->E)
		{
			double wt = min(binom(it->w, p), int(ceil((7.0/6.0)*12.0*b)));
			//if(lastrun) cout << it->idx << " " << wt << "\n";
			multiset<double> l;
			for(int i = 0;i < wt;i++) l.insert(0);
			HE.push_back(new packeredge(it->u, it->v, it->idx, wt, l));
		}

		ppackergraph H = new packergraph(Gdash->n, Gdash->m, HE);
		pair<double, vector<ptree>> packing = packer(H);

		if(lastrun || fabs(1.0-p) < eps) return sample(packing.second, d, Gdash->n);
		else if(packing.first >= (24.0*b/70.0))
		{
			c /= 6.0;
			lastrun = 1;
		}
		else c /= 2.0;
	}
}