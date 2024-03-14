#include "tsp_n_small.h"

using namespace std;

const int maxn = 22;
const long long INF = 0x3f3f3f3f3f3f3f3f;

long long f[1 << maxn][maxn];

long long tsp_n_small(int n, const vector<vector<long long>>& C) {
	assert(n <= maxn);

	for(int t = 1 << n; t--; ) {
		for(int i = n; i--; ) {
			f[t][i] = INF;
		}
	}
	f[1 << 0][0] = 0;

	for(int t = 0; t < (1 << n); ++t) {
		for(int i = 0; i < n; ++i) {
			if(!(t >> i & 1)) {
				continue;
			}

			long long ft = f[t][i];
			if(ft == INF) {
				continue;
			}

			for(int j = 0; j < n; ++j) {
				if(t >> j & 1) {
					continue;
				}
				f[t | (1 << j)][j] = min(f[t | (1 << j)][j], ft + C[i][j]);
			}
		}
	}

	long long res = INF;
	for(int i = 0; i < n; ++i) {
		res = min(res, f[(1 << n) - 1][i] + C[i][0]);
	}

	return res;
}

