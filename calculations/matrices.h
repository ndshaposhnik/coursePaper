#pragma once
#include <vector>
#include <set>
#include "tropicalSemiring.h"
#include "mathFunctions.h"

template<size_t N, size_t M = N>
class TropicalMatrix {
private:
    std::vector<std::vector<Tropical>> matrix;
public:
    TropicalMatrix() : matrix(std::vector<std::vector<Tropical>>(N, std::vector<Tropical>(M))) {}

    TropicalMatrix(const std::initializer_list<std::vector<Tropical>>& list) {
        std::vector<std::vector<Tropical>> tmp(list);
        matrix.resize(N, std::vector<Tropical>(M));
        for (size_t i = 0; i < N; ++i) {
            for (size_t j = 0; j < M; ++j) {
                matrix[i][j] = tmp[i][j];
            }
        }
    }

    std::vector<Tropical>& operator[] (int ind) {
        return matrix[ind];
    }

    const std::vector<Tropical>& operator[] (int ind) const {
        return matrix[ind];
    }

    template<size_t A, size_t B>
    friend bool operator == (const TropicalMatrix<A, B>&, const TropicalMatrix<A, B>&);

    template<size_t A, size_t B>
    friend TropicalMatrix<A, B> operator + (const TropicalMatrix<A, B>&, const TropicalMatrix<A, B>&);

    template<size_t A, size_t B, size_t C>
    friend TropicalMatrix<A, C> operator * (const TropicalMatrix<A, B>&, const TropicalMatrix<B, C>&);

    TropicalMatrix operator += (const TropicalMatrix<N, M>& rhs) {
        for (size_t i = 0; i < N; ++i) {
            for (size_t j = 0; j < M; ++j) {
                (*this)[i][j] += rhs[i][j];
            }
        }
        return *this;
    }

    template<size_t K>
    TropicalMatrix operator *= (const TropicalMatrix<M, K>& rhs) {
        *this = *this * rhs;
        return *this;
    }

    static TropicalMatrix<N, N> unit() {
        TropicalMatrix<N, N> res;
        for (size_t i = 0; i < N; ++i) {
            for (size_t j = 0; j < N; ++j) {
                res[i][j] = _inf;
            }
        }
        for (size_t i = 0; i < N; ++i) {
            res[i][i] = 0;
        }
        return res;
    }

    TropicalMatrix<N, N> kleene() const {
        TropicalMatrix<N> res, tmp = unit();
        for (size_t i = 0; i < N; ++i) {
            res += tmp;
            tmp *= (*this);
        }
        return res;
    }

    TropicalMatrix<N, N> power(size_t n) const {
        if (n == 0) {
            return unit();
        }
        if (n == 1) {
            return *this;
        }
        if (n % 2 == 0) {
            return ((*this) * (*this)).power(n / 2);
        }
        return (*this) * power(n - 1);
    }

    TropicalMatrix<N, N> operator ^ (size_t pow) const {
        return power(pow);
    }

    int exponent() const {
        int cnt = 1;
        auto B = *this;
        while (cnt <= wielandtNumber(N) && !(B > TropicalMatrix<N>())) {
            cnt++;
            B *= *this;
        }
        if (cnt == wielandtNumber(N) + 1) {
            return -1;
        }
        return cnt;
    }

    int scramblingIndex() const {
        int l = 1, r = wielandtNumber(N) + 1;
        while (r - l > 1) {
            int m = (r + l) / 2;
            auto B = this->power(m);
            bool isOk = true;
            for (size_t v = 0; v < N; v++) {
                for (size_t u = 0; u < N; u++) {
                    bool isPath = false;
                    for (size_t w = 0; w < N; w++) {
                        if (B[v][w] > _inf && B[u][w] > _inf) {
                            isPath = true;
                            break;
                        }
                    }
                    if (!isPath) {
                        isOk = false;
                        break;
                    }
                }
                if (!isOk) {
                    break;
                }
            }
            if (isOk) {
                r = m;
            } else {
                l = m;
            }
        }
        if (r == wielandtNumber(N) + 1) {
            return 0;
        }
        return r;
    }

    TropicalMatrix<M, N> t() const {
        TropicalMatrix<M, N> res;
        for (size_t i = 0; i < M; i++) {
            for (size_t j = 0; j < N; j++) {
                res[i][j] = (*this)[j][i];
            }
        }
        return res;
    }

};

template<size_t N, size_t M>
TropicalMatrix<N, M> operator + (const TropicalMatrix<N, M>& lhs, const TropicalMatrix<N, M>& rhs) {
    TropicalMatrix<N, M> res = lhs;
    res += rhs;
    return res;
}

template<size_t N, size_t M, size_t K>
TropicalMatrix<N, K> operator * (const TropicalMatrix<N, M>& lhs, const TropicalMatrix<M, K>& rhs) {
    TropicalMatrix<N, K> res;
    for (size_t i = 0; i < N; ++i) {
        for (size_t j = 0; j < M; ++j) {
            for (size_t k = 0; k < M; ++k) {
                res[i][j] += (lhs[i][k] *  rhs[k][j]);
            }
        }
    }
    return res;
}

template<size_t N, size_t M>
std::ostream& operator << (std::ostream& out, const TropicalMatrix<N, M>& tm) {
    for (size_t i = 0; i < N; ++i) {
        for (size_t j = 0; j < M; ++j) {
            out << tm[i][j];
        }
        out << '\n';
    }
    return out;
}

/*
template<size_t N, size_t M>
std::ostream& operator << (std::ostream& out, const TropicalMatrix<N, M>& tm) {
    for (size_t i = 0; i < N; ++i) {
        for (size_t j = 0; j < M; ++j) {
            out << tm[i][j] << '&';
        }
        out << "\\\\ \n";
    }
    return out;
}
*/

template<size_t N, size_t M>
bool operator == (const TropicalMatrix<N, M>& lhs, const TropicalMatrix<N, M>& rhs) {
    return lhs.matrix == rhs.matrix;
}

template<size_t N, size_t M>
bool operator != (const TropicalMatrix<N, M>& lhs, const TropicalMatrix<N, M>& rhs) {
    return !(lhs == rhs);
}

template<size_t N, size_t M>
bool operator < (const TropicalMatrix<N, M>& lhs, const TropicalMatrix<N, M>& rhs) {
    for (size_t i = 0; i < N; ++i) {
        for (size_t j = 0; j < M; ++j) {
            if (lhs[i][j] >= rhs[i][j]) {
                return false;
            }
        }
    }
    return true;
}

template<size_t N, size_t M>
bool operator > (const TropicalMatrix<N, M>& lhs, const TropicalMatrix<N, M>& rhs) {
    for (size_t i = 0; i < N; ++i) {
        for (size_t j = 0; j < M; ++j) {
            if (lhs[i][j] <= rhs[i][j]) {
                return false;
            }
        }
    }
    return true;
}

template<size_t N, size_t M>
bool operator <= (const TropicalMatrix<N, M>& lhs, const TropicalMatrix<N, M>& rhs) {
    for (size_t i = 0; i < N; ++i) {
        for (size_t j = 0; j < M; ++j) {
            if (lhs[i][j] > rhs[i][j]) {
                return false;
            }
        }
    }
    return true;
}

template<size_t N, size_t M>
bool operator >= (const TropicalMatrix<N, M>& lhs, const TropicalMatrix<N, M>& rhs) {
    for (size_t i = 0; i < N; ++i) {
        for (size_t j = 0; j < M; ++j) {
            if (lhs[i][j] < rhs[i][j]) {
                return false;
            }
        }
    }
    return true;
}

template<size_t N>
TropicalMatrix<N, N> getC(const TropicalMatrix<N, N>& M, std::set<size_t> g) {
    TropicalMatrix<N, N> res;
    for (int v : g) {
        for (size_t i = 0; i < N; ++i) {
            res[i][v] = M[i][v];
        }
    }
    return res;
}

template<size_t N>
TropicalMatrix<N, N> getR(const TropicalMatrix<N, N>& M, std::set<size_t> g) {
    TropicalMatrix<N, N> res;
    for (int v : g) {
        for (size_t i = 0; i < N; ++i) {
            res[v][i] = M[v][i];
        }
    }
    return res;
}

template<size_t N>
TropicalMatrix<N, N> getS(const TropicalMatrix<N, N>& A, std::set<size_t> g) {
    TropicalMatrix<N, N> res;
    for (size_t i : g) {
        for (size_t j : g) {
            res[i][j] = A[i][j]; // * -lambda(A) TODO
        }
    }
    return res;
}

template<size_t N>
TropicalMatrix<N, N> getB(const TropicalMatrix<N, N>& A, std::set<size_t> g) {
    TropicalMatrix<N, N> res;
    for (size_t i = 0; i < N; ++i) {
        for (size_t j = 0; j < N; j++) {
            if (g.count(i) || g.count(j)) {
                res[i][j] = _inf;
            } else {
                res[i][j] = A[i][j];
            }
        }
    }
    return res;
}

template<size_t N> 
size_t getT(const TropicalMatrix<N>& A, const TropicalMatrix<N>& C, const TropicalMatrix<N>& S, const TropicalMatrix<N>& R) {
    if (C * R == TropicalMatrix<N>::unit()) {
        return 0;
    }
    int l = 0;
    int r = 1;
    auto powS = S;
    auto powA = A;
    while (C * powS * R != powA) { 
        powS *= powS;
        powA *= powA;
        r *= 2;
    }
    while (r - l > 1) {
        int m = (r + l) / 2;
        if (C * S.power(m) * R == A.power(m)) {
            r = m;
        } else {
            l = m;
        }
    }
    return r;
}

template<size_t N> 
size_t getT1(const TropicalMatrix<N>& A, const TropicalMatrix<N>& B, const TropicalMatrix<N>& C, const TropicalMatrix<N>& S, const TropicalMatrix<N>& R) {
    if (C * R + B == TropicalMatrix<N>::unit()) {
        return 0;
    }
    int l = 0;
    int r = 1;
    auto powS = S;
    auto powA = A;
    auto powB = B;
    while (C * powS * R + powB != powA) { 
        powS *= powS;
        powA *= powA;
        powB *= powB;
        r *= 2;
    }
    while (r - l > 1) {
        int m = (r + l) / 2;
        if (C * S.power(m) * R + B.power(m) == A.power(m)) {
            r = m;
        } else {
            l = m;
        }
    }
    return r;
}

template<size_t N>
size_t getT2(const TropicalMatrix<N>& B, const TropicalMatrix<N>& C, const TropicalMatrix<N>& S, const TropicalMatrix<N>& R) {
    if (C * R >= B) {
        return 0;
    }
    int l = 0;
    int r = 1;
    auto powS = S;
    auto powB = B;
    while (C * powS * R >= powB) { 
        powS *= powS;
        powB *= powB;
        r *= 2;
    }
    while (r - l > 1) {
        int m = (r + l) / 2;
        if (C * S.power(m) * R >= B.power(m)) {
            r = m;
        } else {
            l = m;
        }
    }
    return r;
}

template<size_t N>
size_t getTold(const TropicalMatrix<N>& A, const TropicalMatrix<N>& C, const TropicalMatrix<N>& S, const TropicalMatrix<N>& R) {
    auto powA = TropicalMatrix<N>::unit(), powS = TropicalMatrix<N>::unit();
    for (size_t t = 0; wielandtNumber(N); ++t) {
        if (powA == C * powS * R) {
            return t;
        }
        powA *= A;
        powS *= S;
    }
}

template<size_t N> 
size_t getT1old(const TropicalMatrix<N>& A, const TropicalMatrix<N>& B, const TropicalMatrix<N>& C,const TropicalMatrix<N>& S, const TropicalMatrix<N>& R) {
    auto powA = TropicalMatrix<N>::unit(), powS = TropicalMatrix<N>::unit(), powB = TropicalMatrix<N>::unit();
    for (size_t t = 0; ; ++t) {
        if (powA == C * powS * R + powB) {
            return t;
        }
        powA *= A;
        powS *= S;
        powB *= B;
    }
}

template<size_t N> 
size_t getT2old(const TropicalMatrix<N>& B, const TropicalMatrix<N>& C,const TropicalMatrix<N>& S, const TropicalMatrix<N>& R) {
    auto powA = TropicalMatrix<N>::unit(), powS = TropicalMatrix<N>::unit(), powB = TropicalMatrix<N>::unit();
    for (size_t t = 0; ; ++t) {
        if (C * powS * R >= powB) {
            return t;
        }
        powS *= S;
        powB *= B;
    }
}

template<size_t N>
void addCycle(TropicalMatrix<N>& A, int v, int len) {
    A[0][v] = 0;
    for (int i = 0; i < len - 2; i++) {
        A[v][v + 1] = 0;
        v += 1;
    }
    A[v][0] = 0;
}

template<size_t N>
void makeChamomile(TropicalMatrix<N>& A, const std::vector<int>& cycles) {
    int n = cycles.size();
    int tmp = 1;
    for (int i = 0; i < n; i++) {
        addCycle(A, tmp, cycles[i]);
        tmp += cycles[i] - 1;
    }
}


template<size_t N>
void makeChamomile(TropicalMatrix<N>& A, const int cycles[], int n) {
    std::vector<int> c(n);
    for (int i = 0; i < n; i++) {
        c[i] = cycles[i];
    }
    makeChamomile(A, c);
}
