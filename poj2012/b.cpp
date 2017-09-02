#include <cstdio>
#include <iostream>
#include <cmath>

#define N 1000

using namespace std;

typedef long long LL;

bool is_prime[N];
int prime[N];
int m;
int depth;

int getPrime(int n) {
    for(int i = 2; i <= n; ++i) {
        is_prime[i] = true;
    }
    for(int i = 2; i * i <= n; ++i) {
        for(int j = i; i * j <= n; ++j) {
            is_prime[i * j] = false;
        }
    }
    m = 0;
    for(int i = 2; i <= n; ++i) {
        if(is_prime[i]) {
            prime[++m] = i;
        }
    }
    return m;
}

bool check(LL s) {
    LL b = (LL)sqrt(4*s-3);
    if(b * b == 4*s-3) {
        cout << depth << ": s=" << s;
        for(int i = 1; i <= m; ++i) {
            LL t = 1;
            while(s % prime[i] == 0) {
                t *= prime[i];
                s /= prime[i];
            }
            if(t != 1) cout << " " << t;
        }
        cout << endl;
        return true;
    }
    return false;
}

bool DFS(LL s, int k, int cnt) {
    if(cnt == depth) {
        if(check(s)) {
            return true;
        }
        return false;
    } 
    for(int i = k + 1; i <= m; ++i) {
        if(DFS(s * prime[i], i, cnt + 1)) {
            return true;
        }
    }
    return false;
}
        
int main() {
    getPrime(200);
    prime[1] = 4;
    cout << m << endl;
    for(int i = 1; i <= m; ++i) {
        cout << prime[i] << endl;
    }
    for(depth = 1; depth <= 10; ++depth) {
        DFS(1, 0, 0);
    }
    system("pause");
    return 0;
}
