#include <iostream>
#include "matrices.h"
using std::cin;
using std::cout;

int main() {
    const int n = 2; // Number of cycles!
    const int k = 1;
    const int a[n] = {4, 13};
    if (gcd(a, n) != 1) {
        cout << "GCD != 1\n";
        return 0;
    }
    const int N = (a[0] + a[1]) * k - n + 1;
    TropicalMatrix<N> A;
    int p = TforChamomile(a, n);
    makeChamomile(A, a, n);
    cout << a[0] << ' ' << a[1] << ": " << A.scramblingIndex() << ' ' << p << '\n';
    cout << P(a, n) << '\n';
    return 0;
}

