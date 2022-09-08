#pragma once
#include <vector>

long long gcd(long long a, long long b) {
    while (b) {
        long long c = a % b;
        a = b;
        b = c;
    }
    return a;
}

long long gcd(std::vector<long long> a) {
    long long res = 0;
    for (long long i : a) {
        res = gcd(i, res);
    }
    return res;
}

long long gcd(const long long* a, long long n) {
    std::vector<long long> v(n);
    for (long long i = 0; i < n; i++) {
        v[i] = a[i];
    }
    return gcd(v);
}

long long wielandtNumber(long long n) {
    return n * n - 2 * n + 2;
}

long long slowP(const std::vector<long long>& a) {
    long long n = a.size();
    long long N = 0;
    for (long long i = 0; i < n; i++) {
        N += a[i];
    }
    N += -n + 1;
    long long ANS = wielandtNumber(N);
    std::vector<bool> dp(ANS + a[0]);
    dp[0] = true;
    long long cnt = 0;
    for (long long p = 0; p < ANS + a[0]; p++) {
        for (long long i = 0; i < n; i++) {
            if (p - a[i] >= 0 && dp[p - a[i]]) {
                dp[p] = true;
                break;
            }
        }
        if (!dp[p]) {
            cnt = 0;
        } else {
            cnt++;
        }
        if (cnt == a[0]) {
            return p - a[0] + 1;
        }
    }
    return -1;
}

long long slowP(const long long a[], long long n) {
    std::vector<long long> v(n);
    for (long long i = 0; i < n; i++) {
        v[i] = a[i];
    }
    return slowP(v);
}

long long P(std::vector<long long> a) {
    long long n = a.size();
    long long N = 0;
    for (long long i = 0; i < n; i++) {
        N += a[i];
    }
    N -= n + 1;
    long long INF = (a[0] - 1) * (a[n - 1] - 1) + a[0] * (2 * a[n - 1] - 2) + a[0];
    std::vector<long long> M(a[0], INF);
    M[0] = 0;
    for (long long i = 1; i < n; i++) {
        for (long long k = 0; k < a[0]; k++) {
            long long now = a[i] * k % a[0];
            M[now] = std::min(M[now], a[i] * k);
        }
    }
    long long deg = 1;
    while (deg <= n) {
        for (long long i = 0; i < a[0]; i++) {
            for (long long j = 0; j < a[0]; j++) {
                long long now = (i + j) % a[0];
                M[now] = std::min(M[now], M[i] + M[j]);
            }
        } 
        deg *= 2; // CHANGE
    }
    return *std::max_element(M.begin(), M.end()) - a[0] + 1;
}

long long P(const long long a[], long long n) {
    std::vector<long long> v(n);
    for (long long i = 0; i < n; i++) {
        v[i] = a[i];
    }
    return P(v);
}

long long TforChamomile(const long long a[], long long n) {
    return P(a, n) + 2 * a[n - 1] - 2;
}
