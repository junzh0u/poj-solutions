// POJ 1557 - Mapmaker
// Model: claude-sonnet-5
// Approach: for each declared array, compute the linear-addressing
// coefficients C[1..D] via the backward recurrence
// C[D] = element size, C[d] = C[d+1] * (U[d+1]-L[d+1]+1) for d<D,
// then C[0] = B - sum(C[d]*L[d]).  A reference's address is then
// C[0] + sum(C[d]*i[d]).  Arrays are looked up by name in a map;
// each reference's dimensionality is taken from the array's own D
// (references give exactly that many indices, per the statement).
// No ambiguity found in the statement; the two board threads restate
// the same recurrence used here.
#include <cstdio>
#include <string>
#include <map>
using namespace std;

struct ArrDef {
    int D;
    long C[12];
};

int main() {
    int n, r;
    if (scanf("%d %d", &n, &r) != 2) return 0;
    map<string, ArrDef> arrs;
    for (int i = 0; i < n; i++) {
        char name[64];
        long base, elemSize;
        int D;
        scanf("%s %ld %ld %d", name, &base, &elemSize, &D);
        ArrDef def;
        def.D = D;
        long L[12], U[12];
        for (int d = 1; d <= D; d++) {
            scanf("%ld %ld", &L[d], &U[d]);
        }
        def.C[D] = elemSize;
        for (int d = D - 1; d >= 1; d--) {
            def.C[d] = def.C[d + 1] * (U[d + 1] - L[d + 1] + 1);
        }
        long c0 = base;
        for (int d = 1; d <= D; d++) {
            c0 -= def.C[d] * L[d];
        }
        def.C[0] = c0;
        arrs[string(name)] = def;
    }
    for (int i = 0; i < r; i++) {
        char name[64];
        scanf("%s", name);
        map<string, ArrDef>::iterator it = arrs.find(string(name));
        ArrDef &def = it->second;
        long idx[12];
        for (int d = 1; d <= def.D; d++) {
            scanf("%ld", &idx[d]);
        }
        long addr = def.C[0];
        for (int d = 1; d <= def.D; d++) {
            addr += def.C[d] * idx[d];
        }
        printf("%s[", name);
        for (int d = 1; d <= def.D; d++) {
            printf("%ld%s", idx[d], d == def.D ? "" : ", ");
        }
        printf("] = %ld\n", addr);
    }
    return 0;
}
