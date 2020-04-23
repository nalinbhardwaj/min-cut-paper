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
#include "lemma8ds-slow.cpp"
#include "graph.cpp"
using namespace std;

// Algorithm 4: initialise and call compute() to solve.
class mincut {
public:
	const double inf = 1e12+5;

	ptree T;
	pgraph G;
	plemma8ds D;
	int totchain = 0, maxlog;
	vector<int> sz, H, inchain, inst, head, st;
	vector<double> val;
	vector<vector<int>> jmp;

	mincut(pgraph _G, ptree _T)
	{
		G = _G, T = _T;
		D = new lemma8ds(T);
		maxlog = ceil(log(T->n));
		st.clear();
		sz.clear(); sz.resize(T->n);
		H.clear(); H.resize(T->n);
		inchain.clear(); inchain.resize(T->n);
		val.clear(); val.resize(T->n);
		inst.clear(); inst.resize(T->n);
		head.clear(); head.resize(T->n, -1);
		jmp.clear(); jmp.resize(T->n, vector<int>(maxlog+1, -1));

		dfs0(0, -1, 0);
		dfs1(0, -1, totchain++, inf);
		init();
	}

	void dfs0(int node, int par, int ht)
	{
		sz[node] = 1; H[node] = ht; jmp[node][0] = par;
		for(auto it: T->adj[node])
		{
			int child = (it->u == node)?it->v:it->u;
			if(child != par)
			{
				dfs0(child, node, ht+1);
				sz[node] += sz[child];
			}
		}
	}
	
	void dfs1(int node, int par, int chain, double inc)
	{
		inchain[node] = chain;
		if(head[chain] == -1) head[chain] = node;
		val[int(st.size())] = inc;
		inst[node] = int(st.size());
		st.emplace_back(node);

		pair<int, pair<double, int>> largest = {-1, {-1, -1}};
		for(auto it: T->adj[node])
		{
			int child = (it->u == node)?it->v:it->u;
			if(child != par) largest = max(largest, {sz[child], {it->w, child}});
		}
		if(largest.second.second != -1) dfs1(largest.second.second, node, chain, largest.second.first);
		for(auto it: T->adj[node])
		{
			int child = (it->u == node)?it->v:it->u;
			if(child != par && child != largest.second.second)
			{
				dfs1(child, node, totchain++, it->w);
			}
		}
	}


	void init()
	{
		for(int j = 1;j <= maxlog;j++)
		{
			for(int i = 0;i < T->n;i++)
			{
				if(jmp[i][j-1] != -1) jmp[i][j] = jmp[jmp[i][j-1]][j-1];
			}
		}
	}

	vector<pair<int, int>> pathbreakdown(int x, int y)
	{
		if(H[x] > H[y]) swap(x, y);
		int origx = x, origy = y;
		for(int i = maxlog;i >= 0;i--)
		{
			if(H[y]-(1<<i) >= H[x]) y = jmp[y][i];
		}
		if(x == y)
		{
			int child = origy;
			for(int i = maxlog;i >= 0;i--)
			{
				if(H[child]-(1<<i) > H[x]) child = jmp[child][i];
			}
			return ancestrybreakdown(origy, child);
		}
		for(int i = maxlog;i >= 0;i--)
		{
			if(jmp[x][i] != jmp[y][i]) x = jmp[x][i], y = jmp[y][i];
		}


		vector<pair<int, int>> res = ancestrybreakdown(origx, x);
		vector<pair<int, int>> tmp = ancestrybreakdown(origy, y);
		for(auto it: tmp) res.emplace_back(it);

		return res;
	}

	vector<pair<int, int>> ancestrybreakdown(int node, int anc)
	{
		vector<pair<int, int>> res;
		int cur = node;

		while(inchain[cur] != inchain[anc])
		{
			res.push_back({inst[head[inchain[cur]]], inst[cur]});
			cur = jmp[head[inchain[cur]]][0];
		}

		res.push_back({inst[anc], inst[cur]});
		return res;
	}

	double compute()
	{
		double res = inf;
		set<int> intree;		
		for(auto it: T->E) intree.insert(it->idx);

		//for(auto it: st) cout << it << " ";
		//cout << "\n";

		vector<vector<vector<pedge>>> events(int(st.size()), vector<vector<pedge>>(2));
		for(auto it: G->E)
		{
			if(intree.find(it->idx) != intree.end()) continue;


			vector<pair<int, int>> path = pathbreakdown(it->u, it->v);

			for(auto gt: path)
			{
				int a = min(gt.first, gt.second), b = max(gt.first, gt.second);

				events[a][0].emplace_back(it);
				events[b][1].emplace_back(it);
			}

			D->PathAdd(it->u, it->v, it->w);
		}


		for(int i = 0;i < int(st.size());i++)
		{
			for(auto it: events[i][0])
			{
				D->NonPathAdd(it->u, it->v, it->w);
				D->PathAdd(it->u, it->v, -it->w);
			}

			double selfwt = D->QueryEdge(st[i]);
			D->PathAdd(st[i], st[i], inf);
			//cout << i << " " << st[i] << " " << selfwt << " " << D->QueryMinimum() << "\n";
			res = min(res, selfwt+D->QueryMinimum());
			D->PathAdd(st[i], st[i], -inf);

			for(auto it: events[i][1])
			{
				D->NonPathAdd(it->u, it->v, -it->w);
				D->PathAdd(it->u, it->v, it->w);
			}
		}

		double singlecut = D->QueryMinimum();
		res = min(singlecut, res);
		
		return res;
	}
};