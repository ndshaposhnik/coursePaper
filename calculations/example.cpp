#include <iostream>
#include "matrices.h"
using std::cin;
using std::cout;

int main() {
    TropicalMatrix<5> A = { {0, 0, -1, _inf, -7},
                            {0, 0, -1, _inf, -7},
                            {-1, -1, -1, -3, -7},
                            {-3, _inf, _inf, -2, -7},
                            {-7, -7, -7, -7, -3} };

    int sigma = 1;
    auto M = A.kleene();
    std::set<size_t> g = {1};
    auto C = getC(M, g);
    auto S = getS(A, g);
    auto R = getR(M, g);
    
    auto B = getB(A, g);
    cout << A << '\n' << M << '\n' << C << '\n' << S << '\n' << R << '\n' << B << '\n';
    size_t T = getT(A, C, S, R);
    size_t T1 = getT1(A, B, C, S, R);
    size_t T2 = getT2(B, C, S, R);
    cout << T << ' ' << T1 << ' ' << T2 << '\n';
    //cout << A.power(T1) << '\n' << C * S.power(T1) * R + B.power(T1) << '\n';
    cout << C * S.power(T2) * R << '\n' << B.power(T2) << '\n';

    return 0;
}
