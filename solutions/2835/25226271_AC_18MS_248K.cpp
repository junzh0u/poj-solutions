// POJ 2835 - Sunzi's Problem
// Model: gpt-6-astra
// Split the lcm into coprime maximal prime powers. Assign each CRT
// projector to an input modulus containing that prime power.
// Then sum(b_i)=1 (mod L) and a_i*b_i=0 (mod L), so
// sum((N mod a_i)*b_i)=N (mod L) for every N.
// Remainders come from the same N: non-coprime moduli are valid too.
// Replace zero coefficients by L to make every output positive.
#include <iostream>
using namespace std;
typedef long long Int;
Int gcd(Int a, Int b) { while (b) { Int r=a%b; a=b; b=r; } return a; }
int main() {
    int n;
    while (cin >> n && n) {
        int a[10]; Int b[10]={0}, L=1;
        for (int i=0;i<n;++i) { cin >> a[i]; L=L/gcd(L,a[i])*a[i]; }
        for (int p=2;p<=50;++p) {
            bool prime=true;
            for (int d=2;d*d<=p;++d) if (p%d==0) prime=false;
            if (!prime) continue;
            int q=1;
            while (L%(q*p)==0) q*=p;
            if (q==1) continue;
            Int part=L/q;
            int inv=1;
            while ((part%q)*inv%q!=1) ++inv;
            for (int i=0;i<n;++i) if (a[i]%q==0) {
                b[i]=(b[i]+part*inv)%L;
                break;
            }
        }
        for (int i=0;i<n;++i) {
            if (i) cout << ' ';
            cout << (b[i] ? b[i] : L);
        }
        cout << '\n';
    }
    return 0;
}
