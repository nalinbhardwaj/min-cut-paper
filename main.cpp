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
#include "mincut.cpp"
#include "tworespectingtrees.cpp"
using namespace std;

// Read graph from stdin
// Format:
// n m
// u_1 v_1 w_1
// u_2 v_2 w_2
// .
// .
// .
// u_m v_m w_m
pgraph graph_reader()
{
	int n, m;
	scanf("%d%d", &n, &m);
	vector<pedge> E;
	for(int i = 0;i < m;i++)
	{
		int u, v;
		double c;
		scanf("%d%d%lf", &u, &v, &c);
		//u--, v--;
		pedge tmp = new edge(u, v, i, c);
		E.push_back(tmp);
	}

	return new graph(n, m, E);
}

// Read tree from stdin
// Format:
// n
// u_1 v_1 idx_1 w_1
// u_2 v_2 idx_2 w_2
// .
// .
// .
// u_{n-1} v_{n-1} idx_{n-1} w_{n-1}
ptree tree_reader()
{
	int n;
	scanf("%d", &n);
	vector<pedge> E;
	for(int i = 0;i < n-1;i++)
	{
		int u, v, idx;
		double c;
		scanf("%d%d%d%lf", &u, &v, &idx, &c);
		//u--, v--;
		//cout << u << " " << v << " " << c << "\n";
		pedge tmp = new edge(u, v, idx, c);
		E.push_back(tmp);
	}

	return new tree(n, E);
}

void tester(pgraph G)
{
	double d;
	scanf("%lf",&d);
	vector<ptree> trees = tworespectingtrees(d, G);
	printf("%d\n", int(trees.size()));

	// Fix wts using edge index
	for(auto it: trees)
	{
		for(auto gt: it->E)
		{
			gt->w = G->E[gt->idx]->w;
			//cout << gt->u << " " << gt->v << " " << gt->idx << " " << gt->w << "\n";
		}
		//cout << "\n";
	}

	//vector<ptree> trees;
	//trees.push_back(tree_reader());

	double res = double(1e9)+5;
	for(auto it: trees)
	{
		mincut mc(G, it);
		res = min(res, mc.compute());
	}

	printf("%lf\n", res);
}

void tester_lemma8ds()
{
	ptree T = tree_reader();
	lemma8ds D(T);

	int m;
	scanf("%d", &m);

	while(m--)
	{
		int t, u, v;
		double x;
		scanf("%d", &t);
		if(t == 0) printf("%lf\n", D.QueryMinimum());
		else if(t == 1)
		{
			scanf("%d%d%lf", &u, &v, &x);
			D.PathAdd(u, v, x);
		}
		else if(t == 2)
		{
			scanf("%d%d%lf", &u, &v, &x);
			D.NonPathAdd(u, v, x);
		}
	}
}

int main(void)
{
	srand(time(NULL));
	pgraph G = graph_reader();
	tester(G);
	//tester2();
}