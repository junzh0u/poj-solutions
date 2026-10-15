// POJ 3875 - Lights
// Model: gpt-6.1-sol
// Treat a game as an m-element subset of the vector space of switch sets.
// Character orthogonality extracts its XOR: the trivial character contributes
// C(2^n,m), and each nontrivial character contributes [z^m](1-z^2)^(2^(n-1)).
// Their sum at the target is 2^n-1 for v=0 and -1 otherwise.
// Falling products compute binomials modulo the prime since m < the modulus.
#include <iostream>
using namespace std;
const int P = 10567201;
long long inv[1001], pow2[1001];
long long power(long long a, int b) {
    long long r = 1;
    while (b) { if (b & 1) r = r*a%P; a = a*a%P; b >>= 1; }
    return r;
}
long long choose(long long a, int b) {
    long long r = 1;
    for (int i=1; i<=b; ++i) r = r*((a-i+1+P)%P)%P*inv[i]%P;
    return r;
}
int main() {
    inv[1]=1;
    for (int i=2; i<=1000; ++i) inv[i]=P-(P/i)*inv[P%i]%P;
    pow2[0]=1;
    for (int i=1; i<=1000; ++i) pow2[i]=pow2[i-1]*2%P;
    int n,m,v;
    while (cin >> n >> m >> v && n) {
        long long a=choose(pow2[n],m), b=0;
        if (!(m&1)) {
            b=choose(pow2[n-1],m/2);
            if ((m/2)&1) b=(P-b)%P;
        }
        long long factor=v==0 ? pow2[n]-1 : P-1;
        cout << (a+b*factor%P)%P*power(pow2[n],P-2)%P << '\n';
    }
    return 0;
}
