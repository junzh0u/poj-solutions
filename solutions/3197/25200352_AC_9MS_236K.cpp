// POJ 3197 - Continuous Fractions
// Model: gpt-5.6-terra
// Approach: perform Euclid's algorithm with a base-10000 non-negative
// integer, then render the resulting continued fraction from the inside out.
// The final Euclidean quotient is split into quotient-1, 1 as required by
// the statement's unique representation.

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Big {
    static const int BASE = 10000;
    vector<int> d;

    void trim() {
        while (d.size() > 1 && d.back() == 0) d.pop_back();
    }

public:
    Big(int x = 0) {
        if (x == 0) d.push_back(0);
        else while (x) {
            d.push_back(x % BASE);
            x /= BASE;
        }
    }

    static Big fromString(const string &s) {
        Big x;
        x.d.clear();
        for (int end = (int)s.size(); end > 0; end -= 4) {
            int begin = max(0, end - 4), part = 0;
            for (int i = begin; i < end; ++i) part = part * 10 + s[i] - '0';
            x.d.push_back(part);
        }
        x.trim();
        return x;
    }

    bool isZero() const { return d.size() == 1 && d[0] == 0; }

    string str() const {
        string s;
        for (int i = (int)d.size() - 1; i >= 0; --i) {
            string part;
            int x = d[i];
            if (i == (int)d.size() - 1) {
                do { part += char('0' + x % 10); x /= 10; } while (x);
            } else {
                for (int j = 0; j < 4; ++j) { part += char('0' + x % 10); x /= 10; }
            }
            reverse(part.begin(), part.end());
            s += part;
        }
        return s;
    }

    int compare(const Big &o) const {
        if (d.size() != o.d.size()) return d.size() < o.d.size() ? -1 : 1;
        for (int i = (int)d.size() - 1; i >= 0; --i)
            if (d[i] != o.d[i]) return d[i] < o.d[i] ? -1 : 1;
        return 0;
    }

    void subtract(const Big &o) {
        int borrow = 0;
        for (size_t i = 0; i < d.size(); ++i) {
            int v = d[i] - (i < o.d.size() ? o.d[i] : 0) - borrow;
            if (v < 0) { v += BASE; borrow = 1; }
            else borrow = 0;
            d[i] = v;
        }
        trim();
    }

    Big multiply(int m) const {
        Big r;
        r.d.assign(d.size() + 1, 0);
        int carry = 0;
        for (size_t i = 0; i < d.size(); ++i) {
            int v = d[i] * m + carry;
            r.d[i] = v % BASE;
            carry = v / BASE;
        }
        r.d[d.size()] = carry;
        r.trim();
        return r;
    }

    void shiftAdd(int x) {
        if (isZero()) d[0] = x;
        else d.insert(d.begin(), x);
        trim();
    }

    static void divmod(const Big &a, const Big &b, Big &q, Big &r) {
        q.d.assign(a.d.size(), 0);
        r = Big(0);
        for (int i = (int)a.d.size() - 1; i >= 0; --i) {
            r.shiftAdd(a.d[i]);
            int lo = 0, hi = BASE - 1;
            while (lo <= hi) {
                int mid = (lo + hi) / 2;
                if (b.multiply(mid).compare(r) <= 0) lo = mid + 1;
                else hi = mid - 1;
            }
            q.d[i] = hi;
            if (hi) r.subtract(b.multiply(hi));
        }
        q.trim();
    }

    void decrement() { subtract(Big(1)); }
};

struct Picture {
    int width;
    vector<string> row;
};

static Picture draw(const vector<Big> &a) {
    Picture p;
    p.width = (int)a.back().str().size();
    p.row.push_back(a.back().str());
    for (int i = (int)a.size() - 2; i >= 0; --i) {
        string value = a[i].str();
        int prefix = (int)value.size() + 3;
        Picture next;
        next.width = prefix + p.width;
        int left = (p.width - 1) / 2;
        next.row.push_back(string(prefix + left, '.') + "1" + string(p.width - left - 1, '.'));
        next.row.push_back(value + ".+." + string(p.width, '-'));
        for (size_t j = 0; j < p.row.size(); ++j)
            next.row.push_back(string(prefix, '.') + p.row[j]);
        p = next;
    }
    return p;
}

int main() {
    string ps, qs;
    int tc = 1;
    while (cin >> ps >> qs && !(ps == "0" && qs == "0")) {
        Big p = Big::fromString(ps), q = Big::fromString(qs);
        vector<Big> a;
        while (!q.isZero()) {
            Big quotient, remainder;
            Big::divmod(p, q, quotient, remainder);
            a.push_back(quotient);
            p = q;
            q = remainder;
        }
        a.back().decrement();
        a.push_back(Big(1));
        Picture out = draw(a);
        cout << "Case " << tc++ << ":\n" << ps << " / " << qs << "\n";
        for (size_t i = 0; i < out.row.size(); ++i) cout << out.row[i] << '\n';
    }
    return 0;
}
