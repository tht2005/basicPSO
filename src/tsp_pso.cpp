/*
 *
 * 	TSP solution that start with vertex 0:
 * 		0 + permutationOf(1, 2, ..., n - 1)
 * 	Each particle is a vecter of (1, 2, ..., n - 1)
 *
 */

// Important parameters of this program.
// Adjust this
const int NMAX = 120;				// number of maximum vertex of graph
const long long MAXCOST = 0x3f3f3f3f3f3f3f3f;

// loops
const int CNT_ITER = 5000;			// number of main loop of this program
const int CNT_PARTICLE = 300;			// number of particle to initialize
						//
const int REMOVE_CHANCE = 5;			// chance of a position deleted in crossover : 1/x
// control how particles move
const int CNT_CROSSOVER = 10;			// number of: crossover with pbest and then gbest
const int CNT_SWAP_CROSSOVER = 10;		// number of: randomly swap to position and crossover with pbest then gbest

const int CNT_GREEDY_CHOOSE = 2;		// number of: minimum path choose in greedy's initialization each step

#include <algorithm>
#include <assert.h>
#include <iostream>
#include <vector>
#include <string.h>
#include <utility>

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
		if((unsigned)rng() % REMOVE_CHANCE) {
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

void greedyInit(int n, vector<int>& a, const vector<vector<long long>>& C) {
	vector<pair<int, int>> choose(n - 1);
	for(int i = 1; i < n; ++i) {
		choose[i - 1].second = i;
	}

	a.clear();

	for(int last = 0; !choose.empty(); ) {
		for(pair<int, int>& pr : choose) {
			pr.first = C[last][pr.second];
		}
		sort(choose.begin(), choose.end());

		// choose a random vertex to go
		int cnt_choose = min((int)choose.size(), CNT_GREEDY_CHOOSE);
		assert(cnt_choose > 0);

		// push my choice to the answer
		int my_choice = (unsigned)rng() % cnt_choose;
		a.push_back(choose[my_choice].second);
		last = a.back();

		// delete the current vertex out of the list
		swap(choose[my_choice], choose.back());
		choose.pop_back();
	}
}

void add_edge(int u, int v, vector<vector<int>>& adj) {
	adj[u].push_back(v);
	adj[v].push_back(u);
}

void mst(int n, vector<vector<int>>& adj, const vector<vector<long long>>& C) {
	static int pre[NMAX];
	static long long dist[NMAX];

	dist[0] = MAXCOST;
	for(int i = 1; i < n; ++i) {
		pre[i] = 0;
		dist[i] = min(C[0][i], C[i][0]);
	}

	for(int _ = n - 1; _--; ) {
		int v = min_element(dist, dist + n) - dist;
		assert(dist[v] != MAXCOST);

		dist[v] = MAXCOST;
		add_edge(pre[v], v, adj);

		for(int u = 1; u < n; ++u) {
			if(dist[u] == MAXCOST) {
				continue;
			}
			if(ckmin(dist[u], min(C[u][v], C[v][u]))) {
				pre[u] = v;
			}
		}
	}
}

void euler(int cur, vector<vector<int>>& adj, vector<int>& stk) {
	while(!adj[cur].empty()) {
		int nxt = adj[cur].back();
		adj[cur].pop_back();

		euler(nxt, adj, stk);
	}	
	stk.push_back(cur);
}

void christofide(int n, vector<int>& res, const vector<vector<int>>& mst_adj, vector<int>& odd) {
	assert((int)mst_adj.size() == n);
	assert(((int)odd.size() & 1) == 0);

	shuffle(odd.begin(), odd.end(), default_random_engine(unsigned(rng())));

	vector<vector<int>> adj(mst_adj);
	int mid = (int)odd.size() >> 1;
	for(int i = 0; i < mid; ++i) {
		add_edge(odd[i], odd[i + mid], adj);
	}

	vector<int> path;
	euler(0, adj, path);

	assert(path[0] == 0);

	static int cnt = -1, vst[NMAX];
	if(cnt == -1) {
		memset(vst, -1, sizeof(vst));
	}
	++cnt;

	// dont push vertex 0
	vst[0] = cnt;

	for(int v : path) {
		if(vst[v] == cnt) {
			continue;
		}
		vst[v] = cnt;
		res.push_back(v);
	}
}

void tsp_pso(int n, const vector<vector<long long>>& C) {
	// init particle
	//
	vector<vector<int>> particle(CNT_PARTICLE);

	vector<long long> pCost(CNT_PARTICLE, MAXCOST + 10);
	long long gCost = MAXCOST + 10;
	vector<int> gbest;

	// generate particles
	//
	// 1/3 random
	for(int i = 0; i < CNT_PARTICLE / 3; ++i) {
		cerr << "\rPSO: INIT " << (i + 1);

		particle[i].resize(n - 1);
		iota(particle[i].begin(), particle[i].end(), 1);
		shuffle(particle[i].begin(), particle[i].end(), default_random_engine(unsigned(rng())));

		pCost[i] = f(particle[i], C);
		if(ckmin(gCost, pCost[i])) {
			gCost = pCost[i];
			gbest = particle[i];
		}
	}
	// 1/3 greedy
	for(int i = CNT_PARTICLE / 3; i < 2 * (CNT_PARTICLE / 3); ++i) {
		cerr << "\rPSO: INIT " << (i + 1);

		greedyInit(n, particle[i], C);

		pCost[i] = f(particle[i], C);
		if(ckmin(gCost, pCost[i])) {
			gCost = pCost[i];
			gbest = particle[i];
		}
	}

	// find a mst
	vector<vector<int>> mst_adj(n);
	mst(n, mst_adj, C);

	vector<int> odd;
	for(int i = 0; i < n; ++i) {
		if((int)mst_adj[i].size() & 1) {
			odd.push_back(i);
		}
	}
	// 1/3 Christofide
	for(int i = 2 * (CNT_PARTICLE / 3); i < CNT_PARTICLE; ++i) {
		cerr << "\rPSO: INIT " << (i + 1);

		christofide(n, particle[i], mst_adj, odd);

		pCost[i] = f(particle[i], C);
		if(ckmin(gCost, pCost[i])) {
			gCost = pCost[i];
			gbest = particle[i];
		}
	}

	cerr << endl;

	vector<vector<int>> pbest(particle);

	long long next_pCost, cost;
	vector<int> tmp, tmp2, next_pbest;

	
	for(int t = 1; t <= CNT_ITER; ++t) {
		cerr << "\rPSO: ITERATION " << t;

		for(int i = 0; i < CNT_PARTICLE; ++i) {
			next_pCost = pCost[i];
			next_pbest = pbest[i];

			// move by swap and crossover with pbest and gbest
			for(int iter = CNT_SWAP_CROSSOVER; iter--; ) {
				tmp = particle[i];
				swapRandom(tmp);

				cost = f(tmp, C);
				if(ckmin(next_pCost, cost)) {
					next_pCost = cost;
					next_pbest = tmp;
				}

				if(iter & 1) {
					cross_over(tmp, pbest[i], tmp2);
				}
				else {
					cross_over(pbest[i], tmp, tmp2);
				}

				cost = f(tmp2, C);
				if(ckmin(next_pCost, cost)) {
					next_pCost = cost;
					next_pbest = tmp2;
				}

				if(iter >> 1 & 1) {
					cross_over(tmp2, gbest, tmp);
				}
				else {
					cross_over(gbest, tmp2, tmp);
				}

				// update Cost
				cost = f(tmp, C);
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

				cost = f(tmp2, C);
				if(ckmin(next_pCost, cost)) {
					next_pCost = cost;
					next_pbest = tmp2;
				}

				if(iter >> 1 & 1) {
					cross_over(tmp2, gbest, tmp);
				}
				else {
					cross_over(gbest, tmp2, tmp);
				}

				// update Cost
				cost = f(tmp, C);
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
	cerr << endl;

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

