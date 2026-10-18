// POJ 3369 - Grocery store
// Model: gpt-6-astra
// Enumerate ordered integer-cent triples and solve exactly for the fourth
// price: d = 1000000*(a+b+c)/(a*b*c-1000000). The 2000-cent
// bound applies to the total, and solutions may appear in any order.
#include <cstdio>
int main() {
    const int scale = 1000000;
    for (int a = 1; a <= 500; ++a)
        for (int b = a; a + 3*b <= 2000; ++b)
            for (int c = b; a + b + 2*c <= 2000; ++c) {
                int den = a*b*c - scale;
                if (den <= 0) continue;
                int num = scale*(a+b+c);
                if (num % den) continue;
                int d = num/den;
                if (d < c || a+b+c+d > 2000) continue;
                std::printf("%d.%02d %d.%02d %d.%02d %d.%02d\n",
                    a/100,a%100,b/100,b%100,c/100,c%100,d/100,d%100);
            }
    return 0;
}
