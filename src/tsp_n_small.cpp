#include <assert.h>
#include <algorithm>
#include <iostream>
#include <vector>

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

int main(int argc, char *argv[]) {
	assert(argc == 3);
	freopen(argv[1], "r", stdin);
	freopen(argv[2], "w", stdout);

	int n;
	cin >> n;

	vector<vector<long long>> C(n);
	for(int i = 0; i < n; ++i) {
		C[i].resize(n);

		for(int j = 0; j < n; ++j) {
			cin >> C[i][j];
		}
	}

	cout << tsp_n_small(n, C);

	return 0;
}

