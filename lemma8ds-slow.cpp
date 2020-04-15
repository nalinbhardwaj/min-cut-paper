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

// Lemma 8 DS: Slow version
class lemma8ds {
public:
	const int inf = int(1e9)+5;

	ptree T;
	int curst = 0, totchain = 0, maxlog;
	double totdelta = 0;
	vector<int> sz, H, inchain, inst, head;
	vector<double> st, lz, val;
	vector<vector<int>> jmp;

	lemma8ds(ptree _T)
	{
		T = _T;
		curst = 0;
		maxlog = ceil(log(T->n));
		st.clear(); st.resize(4*T->n+10);
		lz.clear(); lz.resize(4*T->n+10);
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
		build(1, 0, curst-1);
	}

	inline int left(int node) { return (node<<1); }
	inline int right(int node) { return (node<<1)+1; }

	void build(int node, int L, int R)
	{
		if(L == R) st[node] = val[L];
		else
		{
			build(left(node), L, (L+R)/2), build(right(node), (L+R)/2+1, R);
			st[node] = min(st[left(node)], st[right(node)]);
		}
	}

	void shift(int node, int L, int R)
	{
		if(!lz[node] || L == R)
		{
			lz[node] = 0;
			return;
		}

		lz[left(node)] += lz[node], st[left(node)] += lz[node];
		lz[right(node)] += lz[node], st[right(node)] += lz[node];
		lz[node] = 0;
	}

	void upd(int node, int L, int R, int a, int b, double v)
	{
		if(a > R || b < L) return;
		else if(a <= L && R <= b) st[node] += v, lz[node] += v;
		else
		{
			shift(node, L, R);
			upd(left(node), L, (L+R)/2, a, b, v), upd(right(node), (L+R)/2+1, R, a, b, v);
			st[node] = min(st[left(node)], st[right(node)]);
		}
	}

	double qry(int node, int L, int R, int a, int b)
	{
		if(a > R || b < L) return 0;
		else if(a <= L && R <= b) return st[node];
		else
		{
			shift(node, L, R);
			return qry(left(node), L, (L+R)/2, a, b)+qry(right(node), (L+R)/2+1, R, a, b);
		}
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
		val[curst] = inc;
		inst[node] = curst++;

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

	void pathupd(int x, int y, double delta)
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
			ancestryupd(origy, child, delta);
			return;
		}
		for(int i = maxlog;i >= 0;i--)
		{
			if(jmp[x][i] != jmp[y][i]) x = jmp[x][i], y = jmp[y][i];
		}

		ancestryupd(origx, x, delta); ancestryupd(origy, y, delta);
	}

	void ancestryupd(int node, int anc, double delta)
	{
		int cur = node;
		while(inchain[cur] != inchain[anc])
		{
			upd(1, 0, curst-1, inst[head[inchain[cur]]], inst[cur], delta);
			cur = jmp[head[inchain[cur]]][0];
		}
		upd(1, 0, curst-1, inst[anc], inst[cur], delta);
	}

	void PathAdd(int u, int v, double x)
	{
		pathupd(u, v, x);
	}

	void NonPathAdd(int u, int v, double x)
	{
		totdelta += x;
		pathupd(u, v, -x);
	}

	double QueryMinimum()
	{
		return st[1]+totdelta;
	}

	double QueryEdge(int node)
	{
		return qry(1, 0, curst-1, inst[node], inst[node])+totdelta;
	}
};

typedef lemma8ds* plemma8ds;
