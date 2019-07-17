#include <iostream>
#include <fstream>
#include <iomanip>
#include <climits>
#include <limits>
#include <algorithm>
#include <array>
#include <vector>
#include <deque>
#include <queue>
#include <list>
#include <stack>
#include <string>
#include <functional>
#include <numeric>
#include <map>
#include <set>
#include <cstdlib>
#include <bitset>
#include <unordered_map>
#include <random>
#include <cmath>
#include <complex>
// #include "utiltime.hpp"

using namespace std;

typedef long long int ll;
typedef vector<int> vi;
typedef vector<vector<int>> vvi;
typedef pair<int, int> P;
typedef pair<ll, ll> Pll;
typedef vector<ll> vll;
typedef vector<vector<ll>> vvll;

const int INFL = (int)1e9;
const ll INFLL = (ll)1e18;
const double INFD = numeric_limits<double>::infinity();
const double PI = 3.14159265358979323846;
#define Loop(i, n) for(int i = 0; i < (int)n; i++)
#define Loopll(i, n) for(ll i = 0; i < (ll)n; i++)
#define Loop1(i, n) for(int i = 1; i <= (int)n; i++)
#define Loopll1(i, n) for(ll i = 1; i <= (ll)n; i++)
#define Loopr(i, n) for(int i = (int)n - 1; i >= 0; i--)
#define Looprll(i, n) for(ll i = (ll)n - 1; i >= 0; i--)
#define Loopr1(i, n) for(int i = (int)n; i >= 1; i--)
#define Looprll1(i, n) for(ll i = (ll)n; i >= 1; i--)
#define Loopitr(itr, container) for(auto itr = container.begin(); itr != container.end(); itr++)
#define printv(vector) Loop(i, vector.size()) { cout << vector[i] << " "; } cout << endl;
#define printmx(matrix) Loop(i, matrix.size()) { Loop(j, matrix[i].size()) { cout << matrix[i][j] << " "; } cout << endl; }
#define quickio() ios::sync_with_stdio(false); cin.tie(0);
#define bitmanip(m,val) static_cast<bitset<(int)m>>(val)
ll rndf(double x) { return (ll)(x + (x >= 0 ? 0.5 : -0.5)); }
ll floorsqrt(double x) { ll m = (ll)sqrt(x); return m + (m * m <= (ll)(x) ? 0 : -1); }
ll ceilsqrt(double x) { ll m = (ll)sqrt(x); return m + ((ll)x <= m * m ? 0 : 1); }
ll rnddiv(ll a, ll b) { return (a / b + (a % b * 2 >= b ? 1 : 0)); }
ll ceildiv(ll a, ll b) { return (a / b + (a % b == 0 ? 0 : 1)); }

/*******************************************************/

ifstream header_ifs;
ofstream ofs;
string class_name = "";
vector<string> params_typename = {};
vector<string> params_name = {};
vector<string> test_params = {};
string signiture_name = "";
string method_name = "";

void failed(string error_message) {
	cout << "Failed: " << error_message << endl;
	exit(EXIT_SUCCESS);
}

void write_before_main() {
	ofs << endl << endl;
	ofs << "class " << class_name << " {" << endl;
	ofs << "public:" << endl;
	ofs << "\t" << signiture_name << " {" << endl;
	ofs << "\t" << "}" << endl;
	ofs << "};" << endl;
	ofs << endl;
	ofs << "struct params_t {" << endl;
	Loop(i, params_typename.size()) {
		ofs << "\t" << params_typename[i] << " " << params_name[i] << ";" << endl;
	}
	ofs << "};" << endl << endl;
	ofs << "int main() {" << endl;
	ofs << "\t" << "vector<params_t> testparams;" << endl;
}

void write_testcase() {
	ofs << "\t" << "testparams.push_back({ ";
	Loop(i, test_params.size()) {
		if (i > 0) ofs << ",";
		ofs << test_params[i];
	}
	ofs << " });" << endl;
}

void write_after_main() {
	ofs << "\t" << class_name << " classunit;" << endl;
	ofs << "\t" << "Loop(unused, testparams.size()) {" << endl;
	ofs << "\t\t" << "classunit." << method_name << "(";
	Loop(i, params_name.size()) {
		if (i > 0) ofs << ", ";
		ofs << "testparams[i]." << params_name[i];
	}
	ofs << ");" << endl;
	ofs << "\t" << "}" << endl;
	ofs << "}" << endl;
}


int main() {
	header_ifs.open("../auto_util_header.cpp");
	if (!header_ifs) failed("could not find ../auto_util_header.cpp file");
	ofs.open("base.cpp");
	if (!ofs) failed("could not make base.cpp file");
	string readline;
	while (getline(header_ifs, readline)) {
		ofs << readline << endl;
	}
	cout << "Paste the Statement from \"Definition\" or before it:" << endl << endl;
	const string symbol_class = "Class: ";
	const string symbol_parameters = "Parameters: ";
	const string symbol_method = "Method: ";
	const string symbol_signiture = "Method signature: ";
	bool class_written = false;
	string symbol_testcase = "0)";
	int in_test_case = -1;
	while (getline(cin, readline)) {
		if (in_test_case >= 0) {
			test_params.push_back(readline);
			in_test_case++;
			if (in_test_case >= params_typename.size()) {
				in_test_case = -1;
				write_testcase();
			}
			continue;
		}
		if (readline.substr(0, symbol_class.length()) == symbol_class) {
			class_name = readline.substr(symbol_class.length(), readline.length() - symbol_class.length());
		}
		else if (readline.substr(0, symbol_parameters.length()) == symbol_parameters) {
			int p = (int)symbol_parameters.length();
			while (true) {
				int q = (int)readline.find(',', p);
				if (q == -1) {
					params_typename.push_back(readline.substr(p, readline.length() - p));
					break;
				}
				else {
					params_typename.push_back(readline.substr(p, q - p));
					p = q + 2;
				}
			}
		}
		else if (readline.substr(0, symbol_method.length()) == symbol_method) {
			method_name = readline.substr(symbol_method.length(), readline.length() - symbol_method.length());
		}
		else if (readline.substr(0, symbol_signiture.length()) == symbol_signiture) {
			signiture_name = readline.substr(symbol_signiture.length(), readline.length() - symbol_signiture.length());
			int p = (int)symbol_signiture.length();
			p = (int)readline.find('(', p);
			Loop(i, (int)params_typename.size()) {
				p += (int)params_typename[i].length() + 2;
				int q;
				if (i < (int)params_typename.size() - 1) q = (int)readline.find(',', p);
				else q = (int)readline.find(')', p);
				params_name.push_back(readline.substr(p, q - p));
				p = q + 1;
			}
		}
		else if (class_written && readline.substr(0, symbol_testcase.length()) == symbol_testcase) {
			in_test_case = 0;
			test_params = {};
			symbol_testcase[0]++;
		}
		if (!class_written && !class_name.empty() && !signiture_name.empty()) {
			write_before_main();
			class_written = true;
		}
	}
	write_after_main();
}