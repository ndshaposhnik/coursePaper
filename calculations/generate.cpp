#include <fstream>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <iostream>
using namespace std;

ofstream out("array.txt");

int main() {
    srand(time(NULL));
    int n = 10000;
    vector<long long> v(n);
    out << n << '\n';
    v[0] = 10000;
    for (int i = 1; i < n; i++) {
        v[i] = v[0] + rand() % static_cast<long long>(1e4 + 7) + 1;
    }
    sort(v.begin(), v.end());
    for (auto& i : v) {
        out << i << ' ';
    }
    out << '\n';

    return 0;
}
