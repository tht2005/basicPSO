#include <iostream>
#include <fstream>
#include <chrono>
#include <random>

#include "tsp_n_small.h"
#include "tsp_pso.h"

using namespace std;

const long long maxc = 1LL << 30;

mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());

int n, ntest;

void input() {
	cout << "Input n: ";
	cin >> n;

	cout << "Input number of tests: ";
	cin >> ntest;

	assert(ntest > 0);
}

void test() {
	assert(fopen("./test/br17.txt", "r"));
	ifstream is("./test/br17.txt");
	
	string s;
	while(is >> s) {
		if(s == "DIMENSION:") {
			break;
		}
	}

	int n;
	is >> n;
	cout << n;

	while(is >> s) {
		if(s == "EDGE_WEIGHT_SECTION") {
			break;
		}
	}

	vector<vector<long long>> C(n);
	for(int i = 0; i < n; ++i) {
		C[i].resize(n);
		for(int j = 0; j < n; ++j) {
			is >> C[i][j];
		}
	}

	is.close();

	cout << tsp_n_small(n, C);
}

int main() {

	input();

	// if n <= 22
	test();

	return 0;
}
