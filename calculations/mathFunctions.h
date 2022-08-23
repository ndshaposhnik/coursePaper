#pragma once
#include <vector>

int gcd(int a, int b) {
    while (b) {
        int c = a % b;
        a = b;
        b = c;
    }
    return a;
}

int gcd(std::vector<int> a) {
    int res = 0;
    for (int i : a) {
        res = gcd(i, res);
    }
    return res;
}

int gcd(const int* a, int n) {
    std::vector<int> v(n);
    for (int i = 0; i < n; i++) {
        v[i] = a[i];
    }
    return gcd(v);
}

int wielandtNumber(int n) {
    return n * n - 2 * n + 2;
}

int slowP(const std::vector<int>& a) {
    int n = a.size();
    int N = 0;
    for (int i = 0; i < n; i++) {
        N += a[i];
    }
    N += -n + 1;
    int ANS = wielandtNumber(N);
    std::vector<bool> dp(ANS + a[0]);
    dp[0] = true;
    int cnt = 0;
    for (int p = 0; p < ANS + a[0]; p++) {
        for (int i = 0; i < n; i++) {
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

int slowP(const int a[], int n) {
    std::vector<int> v(n);
    for (int i = 0; i < n; i++) {
        v[i] = a[i];
    }
    return slowP(v);
}

int P(std::vector<int> a) {
    int n = a.size();
    int N = 0;
    for (int i = 0; i < n; i++) {
        N += a[i];
    }
    N -= n + 1;
    int INF = (a[0] - 1) * (a[n - 1] - 1) + a[0] * (2 * a[n - 1] - 2) + a[0];
    std::vector<int> M(a[0], INF);
    M[0] = 0;
    for (int i = 1; i < n; i++) {
        for (int k = 0; k < a[0]; k++) {
            int now = a[i] * k % a[0];
            M[now] = std::min(M[now], a[i] * k);
        }
    }
    int deg = 1;
    while (deg <= n) {
        for (int i = 0; i < a[0]; i++) {
            for (int j = 0; j < a[0]; j++) {
                int now = (i + j) % a[0];
                M[now] = std::min(M[now], M[i] + M[j]);
            }
        } 
        deg *= 2; // CHANGE
    }
    return *std::max_element(M.begin(), M.end()) - a[0] + 1;
}

int P(const int a[], int n) {
    std::vector<int> v(n);
    for (int i = 0; i < n; i++) {
        v[i] = a[i];
    }
    return P(v);
}

int TforChamomile(const int a[], int n) {
    return P(a, n) + 2 * a[n - 1] - 2;
}
