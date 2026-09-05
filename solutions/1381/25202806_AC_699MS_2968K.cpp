// POJ 1381 - Secret Code
// Model: gpt-5.6-terra
// Enumerate admissible least-significant digits (strictly below |B|) in order,
// divide the current Gaussian integer by the base, and backtrack.  The board
// confirms that increasing a0 order is the required tie-break.
#include <iostream>
#include <set>
#include <vector>
using namespace std;

struct Gaussian {
    long r, i;
    Gaussian(long rr = 0, long ii = 0) : r(rr), i(ii) {}
    bool operator<(const Gaussian& other) const {
        return r != other.r ? r < other.r : i < other.i;
    }
};

struct Move {
    long digit;
    Gaussian next;
    Move(long d, Gaussian n) : digit(d), next(n) {}
};

long gcd(long a, long b) {
    while (b != 0) {
        long t = a % b;
        a = b;
        b = t;
    }
    return a < 0 ? -a : a;
}

long inverse(long a, long m) {
    long b = m, u = 1, v = 0;
    while (b != 0) {
        long t = a / b;
        long next = a - t * b;
        a = b;
        b = next;
        next = u - t * v;
        u = v;
        v = next;
    }
    u %= m;
    if (u < 0) u += m;
    return u;
}

void moves(long xr, long xi, long br, long bi, vector<Move>& result) {
    long norm = br * br + bi * bi;
    long limit = 0;
    while ((limit + 1) * (limit + 1) < norm) ++limit;
    ++limit;
    long digit;
    if (bi == 0) {
        long mod = br < 0 ? -br : br;
        digit = xr % mod;
        if (digit < 0) digit += mod;
        long rr = xr - digit;
        long nr = rr * br;
        long ni = xi * br;
        if (nr % norm == 0 && ni % norm == 0)
            result.push_back(Move(digit, Gaussian(nr / norm, ni / norm)));
        return;
    }
    long g = gcd(bi, norm);
    long rhs = xr * bi - xi * br;
    if (rhs % g != 0) return;
    long period = norm / g;
    long residue = ((rhs / g) % period) * inverse((bi / g + period) % period, period) % period;
    if (residue < 0) residue += period;
    for (digit = residue; digit < limit; digit += period) {
        long rr = xr - digit;
        long nr = rr * br + xi * bi;
        long ni = xi * br - rr * bi;
        if (nr % norm == 0 && ni % norm == 0)
            result.push_back(Move(digit, Gaussian(nr / norm, ni / norm)));
    }
}

bool search(const Gaussian& value, long br, long bi, int depth,
            set<Gaussian>& active, vector<long>& digits) {
    if (value.r == 0 && value.i == 0) return true;
    if (depth == 100) return false;
    vector<Move> options;
    moves(value.r, value.i, br, bi, options);
    for (int i = 0; i < (int)options.size(); ++i) {
        if (active.find(options[i].next) != active.end()) continue;
        active.insert(options[i].next);
        digits.push_back(options[i].digit);
        if (search(options[i].next, br, bi, depth + 1, active, digits)) return true;
        digits.pop_back();
        active.erase(options[i].next);
    }
    return false;
}

int main() {
    int tests;
    if (!(cin >> tests)) return 0;
    while (tests--) {
        long xr, xi, br, bi;
        cin >> xr >> xi >> br >> bi;
        vector<long> digits;
        Gaussian start(xr, xi);
        set<Gaussian> active;
        active.insert(start);
        bool ok = search(start, br, bi, 0, active, digits);
        if (!ok) {
            cout << "The code cannot be decrypted.\n";
        } else if (digits.empty()) {
            cout << "0\n";
        } else {
            for (int i = (int)digits.size() - 1; i >= 0; --i) {
                if (i != (int)digits.size() - 1) cout << ',';
                cout << digits[i];
            }
            cout << '\n';
        }
    }
    return 0;
}
