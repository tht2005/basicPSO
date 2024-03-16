/*
 *
 * 	TSP solution that start with vertex 0:
 * 		0 + permutationOf(1, 2, ..., n - 1)
 * 	Each particle is a vecter of (1, 2, ..., n - 1)
 *
 */

// Important parameters of this program.
// Adjust this
const int NMAX = 5000;				// number of maximum vertex of graph
const long long MAXCOST = 0x3f3f3f3f3f3f3f3f;

// loops
const int CNT_ITER = 2000;			// number of main loop of this program
const int CNT_PARTICLE = 200;			// number of particle to initialize
// control how particles move
const int CNT_CROSSOVER = 10;			// number of: crossover with pbest and then gbest
const int CNT_SWAP_CROSSOVER = 10;		// number of: randomly swap to position and crossover with pbest then gbest

#include <algorithm>
#include <assert.h>
#include <iostream>
#include <vector>

#include <chrono>
#include <random>

using namespace std;

mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());

inline bool ckmin(long long& a, long long b) {
	return (a > b) ? (a = b), 1 : 0;
}

void cross_over(const vector<int>& p0, const vector<int>& p1, vector<int>& pc) {
	assert(p0.size() == p1.size());

	pc.resize(p0.size());

	static bool keep[NMAX];

	for(int i = 0; i < (int)p0.size(); ++i) {
		if((unsigned)rng() % 2) {
			pc[i] = -1;
			keep[p0[i]] = 0;
		}
		else {
			pc[i] = p0[i];
			keep[p0[i]] = 1;
		}
	}

	for(int i = 0, ptr = 0; i < (int)p0.size(); ++i) {
		if(pc[i] == -1) {
			while(keep[p1[ptr]]) {
				++ptr;
			}
			pc[i] = p1[ptr];
			++ptr;
		}
	}
}

long long f(const vector<int>& a, const vector<vector<long long>>& C) {
	long long res = C[0][a[0]];
	for(int i = 0; i + 1 < (int)a.size(); ++i) {
		res += C[a[i]][a[i + 1]];
	}
	res += C[a.back()][0];
	return res;
}

void swapRandom(vector<int>& a) {
	if((int)a.size() < 2) {
		return;
	}

	int i = (unsigned)rng() % a.size();
	int j;
	do {
		j = (unsigned)rng() % a.size();
	} while(i == j);

	assert(i != j);

	swap(a[i], a[j]);
}

void tsp_pso(int n, const vector<vector<long long>>& C) {
	// init particle
	cerr << "PSO: INIT" << endl;
	//
	vector<vector<int>> particle(CNT_PARTICLE);

	vector<long long> pCost(CNT_PARTICLE, MAXCOST + 10);
	long long gCost = MAXCOST + 10;
	vector<int> gbest;

	for(int i = 0; i < CNT_PARTICLE; ++i) {
		// generate particle[i]
		particle[i].resize(n - 1);
		iota(particle[i].begin(), particle[i].end(), 1);
		shuffle(particle[i].begin(), particle[i].end(), default_random_engine(unsigned(rng())));

		pCost[i] = f(particle[i], C);
		if(ckmin(gCost, pCost[i])) {
			gCost = pCost[i];
			gbest = particle[i];
		}
	}

	vector<vector<int>> pbest(particle);

	long long next_pCost;
	vector<int> tmp, tmp2, next_pbest;
	
	for(int t = 1; t <= CNT_ITER; ++t) {
		cerr << "PSO: ITERATION " << t << endl;

		for(int i = 0; i < CNT_PARTICLE; ++i) {
			next_pCost = pCost[i];
			next_pbest = pbest[i];

			// move by swap and crossover with pbest and gbest
			for(int iter = CNT_SWAP_CROSSOVER; iter--; ) {
				tmp = particle[i];
				swapRandom(tmp);

				if(iter & 1) {
					cross_over(tmp, pbest[i], tmp2);
				}
				else {
					cross_over(pbest[i], tmp, tmp2);
				}

				if(iter >> 1 & 1) {
					cross_over(tmp2, gbest, tmp);
				}
				else {
					cross_over(gbest, tmp2, tmp);
				}

				// update Cost
				long long cost = f(tmp, C);
				if(ckmin(next_pCost, cost)) {
					next_pCost = cost;
					next_pbest = tmp;
				}
			}

			// move by crossover with pbest and gbest
			for(int iter = CNT_CROSSOVER; iter--; ) {
				tmp = particle[i];

				if(iter & 1) {
					cross_over(tmp, pbest[i], tmp2);
				}
				else {
					cross_over(pbest[i], tmp, tmp2);
				}

				if(iter >> 1 & 1) {
					cross_over(tmp2, gbest, tmp);
				}
				else {
					cross_over(gbest, tmp2, tmp);
				}

				// update Cost
				long long cost = f(tmp, C);
				if(ckmin(next_pCost, cost)) {
					next_pCost = cost;
					next_pbest = tmp;
				}
			}

			if(ckmin(pCost[i], next_pCost)) {
				pbest[i] = next_pbest;
			}
		}

		// update gbest
		for(int i = 0; i < CNT_PARTICLE; ++i) {
			if(ckmin(gCost, pCost[i])) {
				gCost = pCost[i];
				gbest = pbest[i];
			}
		}
	}

	cout << gCost << '\n';
	cout << 0;
	for(int i : gbest) {
		cout << ' ' << i;
	}
}

int main(int argc, char *argv[]) {
	assert(argc == 3);

	freopen(argv[1], "r", stdin);
	freopen(argv[2], "w", stdout);

	int n;
	cin >> n;

	assert(n > 0);

	vector<vector<long long>> C(n);
	for(int i = 0; i < n; ++i) {
		C[i].resize(n);

		for(int j = 0; j < n; ++j) {
			cin >> C[i][j];
		}
	}

	if(n == 1) {
		cout << C[0][0];
		return 0;
	}

	tsp_pso(n, C);

	return 0;
}

