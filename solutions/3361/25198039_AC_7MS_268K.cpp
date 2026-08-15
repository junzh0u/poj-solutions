// POJ 3361 - Gaussian Prime Factors
// Model: claude-sonnet-5
//
// For each rational prime p dividing n we need the Gaussian prime(s)
// that appear in the factorization of n, using the canonical
// representative a+bj with a>0 and (b==0 or |b|>=a):
//   p == 2        : 2 = (1+j)(1-j)                 -> a=1,b=1
//   p % 4 == 1    : p = a^2+b^2 with 0<a<=b (Fermat)-> a,b
//   p % 4 == 3    : p is itself a Gaussian prime     -> a=p,b=0
// Multiplicity of p in n does not matter: the sample (700 = 2^2*5^2*7)
// shows each distinct Gaussian prime listed once regardless of exponent.
// Output order: ascending a, then ascending |b|, then +b before -b
// (confirmed by the discuss board, which also notes the statement's
// own Hint wording is slightly garbled but means exactly this).
// n < 2e9 per the discuss board (no explicit bound in the statement),
// so trial division up to sqrt(n) (~44721) using a precomputed prime
// sieve is used for speed, and a linear two-pointer scan finds a,b
// for split primes in O(sqrt(p)).
#include <cstdio>
#include <cmath>
#include <vector>
#include <algorithm>
using namespace std;

const int LIM = 44730;
vector<int> primes;
bool sieve_notprime[LIM + 1];

void build_sieve() {
    for (int i = 2; i <= LIM; ++i) {
        if (!sieve_notprime[i]) {
            primes.push_back(i);
            for (long long j = (long long)i * i; j <= LIM; j += i)
                sieve_notprime[j] = true;
        }
    }
}

// find a<=b, a>0,b>0, a*a+b*b==p  (p % 4 == 1 or p==2), via two-pointer
void find_ab(long long p, long long &A, long long &B) {
    long long a = 1;
    long long b = (long long)sqrtl((long double)p);
    while (b * b > p) b--;
    while ((b + 1) * (b + 1) <= p) b++;
    while (a <= b) {
        long long s = a * a + b * b;
        if (s == p) { A = a; B = b; return; }
        else if (s < p) a++;
        else b--;
    }
    // should not happen for valid p
    A = 0; B = 0;
}

struct Item {
    long long a, b; // b may be 0 (real), or +-value
    Item(long long a_, long long b_) : a(a_), b(b_) {}
};

bool cmpItem(const Item &x, const Item &y) {
    if (x.a != y.a) return x.a < y.a;
    long long xb = x.b < 0 ? -x.b : x.b;
    long long yb = y.b < 0 ? -y.b : y.b;
    if (xb != yb) return xb < yb;
    return x.b > y.b; // positive before negative
}

int main() {
    build_sieve();
    long long n;
    int tc = 0;
    while (scanf("%lld", &n) == 1) {
        ++tc;
        vector<long long> factors;
        long long m = n;
        for (size_t i = 0; i < primes.size() && (long long)primes[i] * primes[i] <= m; ++i) {
            int p = primes[i];
            if (m % p == 0) {
                factors.push_back(p);
                while (m % p == 0) m /= p;
            }
        }
        if (m > 1) factors.push_back(m);

        vector<Item> items;
        for (size_t i = 0; i < factors.size(); ++i) {
            long long p = factors[i];
            if (p == 2) {
                items.push_back(Item(1, 1));
                items.push_back(Item(1, -1));
            } else if (p % 4 == 1) {
                long long A, B;
                find_ab(p, A, B);
                items.push_back(Item(A, B));
                items.push_back(Item(A, -B));
            } else {
                items.push_back(Item(p, 0));
            }
        }
        sort(items.begin(), items.end(), cmpItem);

        printf("Case #%d:", tc);
        for (size_t i = 0; i < items.size(); ++i) {
            if (i) printf(",");
            long long a = items[i].a, b = items[i].b;
            if (b == 0) {
                printf(" %lld", a);
            } else if (b == 1) {
                printf(" %lld+j", a);
            } else if (b == -1) {
                printf(" %lld-j", a);
            } else if (b > 0) {
                printf(" %lld+%lldj", a, b);
            } else {
                printf(" %lld-%lldj", a, -b);
            }
        }
        printf("\n");
    }
    return 0;
}
