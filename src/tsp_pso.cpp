#include <assert.h>
#include <iostream>
#include <vector>

using namespace std;

void tsp_pso(int n, const vector<vector<long long>>& C) {

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

	tsp_pso(n, C);

	return 0;
}

