// Stoer-Wagner: implementation from 
// https://github.com/nalinbhardwaj/icpc-notebook/blob/master/content/graph/GlobalMinCut.h
// Time complexity: O(V^3)
#include <iostream>
#include <cstdio>
#include <vector>
using namespace std;

const double inf = 1e9+5;

pair<double, vector<int>> GetMinCut(vector<vector<double>>& weights)
{
	int N = int(weights.size());
	vector<bool> used(N);
	vector<int> cut, best_cut;
	double best_weight = -1;

	for (int phase = N-1; phase >= 0; phase--)
	{
		vector<double> w = weights[0];
		vector<bool> added = used;
		int prev, k = 0;
		for(int i = 0;i < phase;i++)
		{
			prev = k;
			k = -1;
			for(int j = 1;j < N;j++)
			{
				if (!added[j] && (k == -1 || w[j] > w[k])) k = j;
			}
			if(i == phase-1)
			{
				for(int j = 0;j < N;j++) weights[prev][j] += weights[k][j];
				for(int j = 0;j < N;j++) weights[j][prev] = weights[prev][j];
				used[k] = true;
				cut.push_back(k);
				if (best_weight < 0 || w[k] < best_weight)
				{
					best_cut = cut;
					best_weight = w[k];
				}
			}
			else
			{
				for(int j = 0;j < N;j++)
				{
					w[j] += weights[k][j];
				}
				added[k] = true;
			}
		}
	}
	return {best_weight, best_cut};
}

int main(void) {
	int n, m;
	scanf("%d%d", &n, &m);
	vector<vector<double>> W(n, vector<double>(n));

	for(int i = 0;i < m;i++)
	{
		int u, v;
		double w;
		scanf("%d%d%lf", &u, &v, &w);
		W[u][v] = w;
		W[v][u] = w;
	}
	for(int i = 0;i < n;i++) W[i][i] = inf;
	double mc = GetMinCut(W).first;
	printf("%lf\n", mc);
}
