#include <iostream>
#include <fstream>
#include "matrices.h"
using namespace std;

ifstream in("array.txt");

int main() {
    int n;
    in >> n;
    std::vector<long long> a(n);
    for (auto& i : a) {
        in >> i;
    }
    cout << P(a) << '\n';

    return 0;
}

