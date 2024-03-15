#include <iostream>
#include <fstream>
#include <ctype.h>
#include <chrono>
#include <random>

#include "tsp_n_small.hpp"
#include "tsp_pso.hpp"

using namespace std;

const long long maxc = 1LL << 30;

mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());

bool r;
int n, ntest;

void input() {
	cout << "Random test or not [y/n]: ";
	char ch;
	cin >> ch;
	if(tolower(ch) == 'y') {
		r = 1;
	}
	else if(tolower(ch) == 'n') {
		r = 0;
	}
	else {
		cout << "\ninvalid!" << endl;
		exit(0);
	}

	if(r) {
		cout << "Input n: ";
		cin >> n;

		cout << "Input number of tests: ";
		cin >> ntest;

		if(ntest <= 0) {
			cout << "\nNumber of tests must be greater than 0!" << endl;
			exit(0);
		}
	}
}

void readTest(ifstream& is, int& n, vector<vector<long long>>& C) {
	string s;
	while(is >> s) {
		if(s == "DIMENSION:") {
			break;
		}
	}

	is >> n;

	while(is >> s) {
		if(s == "EDGE_WEIGHT_SECTION") {
			break;
		}
	}

	C.resize(n);

	for(int i = 0; i < n; ++i) {
		C[i].resize(n);

		for(int j = 0; j < n; ++j) {
			is >> C[i][j];
		}
	}
}
void randomTest() {
	vector<vector<long long>> C(n, vector<long long> (n));
	for(int itest = 1; itest <= ntest; ++itest) {
		for(int i = 0; i < n; ++i) {
			for(int j = 0; j < n; ++j) {
				if(i == j) {
					C[i][j] = 0;
				}
				else {
					C[i][j] = (unsigned long long)rng() % maxc;
				}
			}
		}

		cout << "Test " << itest << ": " << tsp_n_small(n, C) << endl;
	}
}

int main() {

	input();

	randomTest();

	return 0;
}
