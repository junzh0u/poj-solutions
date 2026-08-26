// POJ 3462 - Lucky Light
// Model: gpt-5.6-terra
// Project each segment endpoint onto the x-axis along a ray from the light.
// The shadow is the interval between the two projected endpoints; after
// sorting, the number of lighted areas is one plus the number of merged shadows.
#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

struct Fraction {
    int numerator;
    int denominator;
};

struct Interval {
    Fraction left;
    Fraction right;
};

bool lessFraction(const Fraction &a, const Fraction &b) {
    return a.numerator * b.denominator < b.numerator * a.denominator;
}

bool lessInterval(const Interval &a, const Interval &b) {
    return lessFraction(a.left, b.left);
}

Fraction projection(int x, int y, int xl, int yl) {
    Fraction result;
    result.numerator = x * yl - xl * y;
    result.denominator = yl - y;
    return result;
}

int main() {
    int tests;
    cin >> tests;
    while (tests--) {
        int n, xl, yl;
        cin >> n >> xl >> yl;
        vector<Interval> shadows;
        for (int i = 0; i < n; ++i) {
            int x1, y1, x2, y2;
            cin >> x1 >> y1 >> x2 >> y2;
            Interval shadow;
            shadow.left = projection(x1, y1, xl, yl);
            shadow.right = projection(x2, y2, xl, yl);
            if (lessFraction(shadow.right, shadow.left))
                swap(shadow.left, shadow.right);
            shadows.push_back(shadow);
        }
        if (shadows.empty()) {
            cout << 1 << '\n';
            continue;
        }
        sort(shadows.begin(), shadows.end(), lessInterval);
        int darkAreas = 1;
        Fraction rightmost = shadows[0].right;
        for (int i = 1; i < (int)shadows.size(); ++i) {
            if (lessFraction(rightmost, shadows[i].left)) {
                ++darkAreas;
                rightmost = shadows[i].right;
            } else if (lessFraction(rightmost, shadows[i].right)) {
                rightmost = shadows[i].right;
            }
        }
        cout << darkAreas + 1 << '\n';
    }
    return 0;
}
