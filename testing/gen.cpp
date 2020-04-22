// Generate test cases of format described in README.md
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <set>
#include <cassert>
#include <cstring>
#include <random>
using namespace std;

int N, M;
double W;

int main(int argc, char * argv[])
{
	assert(argc == 4);

	N = strtol(argv[1], NULL, 10);
	M = strtol(argv[2], NULL, 10);
	W = strtod(argv[3], NULL);
	int n = N, m = M;
	printf("%d %d\n", n, m);

	uniform_real_distribution<double> wt(0,W);
	uniform_int_distribution<int> node(0,N-1);
	default_random_engine re;

	set<pair<int, int>> S;
	for(int i = 1;i < n;i++)
	{
		int u = node(re)%i, v = i;
		S.insert({u, v});
		printf("%d %d %lf\n", u, v, wt(re)+1);
	}

	for(int i = n;i <= m;i++)
	{
		int u = node(re), v = node(re);
		if(u < v) swap(u, v);
		while(S.find({u, v}) != S.end())
		{
			u = node(re), v = node(re);
			if(u < v) swap(u, v);
		}
		S.insert({u, v});
		printf("%d %d %lf\n", u, v, wt(re)+1);
	}
	printf("1\n");
}