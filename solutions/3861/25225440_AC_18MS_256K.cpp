// POJ 3861 - Royal Gems
// Model: gpt-6.1-sol
// There are only 21 rectangles up to transposition. Precompute their exact
// optima, then answer each query by table lookup.
// Offline binary model: x[v,c] means cell v has color c (R,E,S,D = 0..3).
// Sum_c x[v,c] = 1. For each c<d, impose
// x[v,c] <= sum(x[u,d] : u is an orthogonal neighbor of v).
// Maximize sum_v x[v,0]. This is precisely every rule in the statement;
// no neighbor outside the board exists. OR-Tools CP-SAT proved OPTIMAL
// for every size, with objective equal to its upper bound. An independent
// enumeration of all colorings also checked 2x2, 2x3, 2x4, 3x3 and 3x4.
// The POJ discussion suggests offline state-compressed DP, supporting
// precomputation rather than a guessed construction or numerical formula.
// Optimal witness boards below were independently checked against all rules.
// 2,2 = 0: SD / DS
// 2,3 = 1: DRS / SED
// 2,4 = 1: SEDD / DRSE
// 2,5 = 2: DRSED / SEDRS
// 2,6 = 2: DRSEDD / SEDRSE
// 2,7 = 3: SEDRSED / DRSEDRS
// 3,3 = 2: DSD / RER / SDS
// 3,4 = 2: DSRD / DRED / DDSE
// 3,5 = 4: SRDRS / DESED / SRDRS
// 3,6 = 4: SDDSDS / RESRER / DRSDSD
// 3,7 = 6: SRDRSRD / DESEDES / SRDRSRD
// 4,4 = 3: DRED / ESSR / RDDE / SDDS
// 4,5 = 5: DSERD / REDSE / SRRED / DDSRD
// 4,6 = 6: SDSEDD / RERRSS / DSDDER / DERSRD
// 4,7 = 7: SRDSERD / DERSDSS / DSREDER / DDDRSRD
// 5,5 = 6: DSRDD / DRESS / SRDER / RESRD / DDDDS
// 5,6 = 8: DSRDDD / DRESER / SRDRRS / DESEDD / SRDRSS
// 5,7 = 10: SDSDERS / RERRSDD / DSDERRS / RERSDER / SDDERSD
// 6,6 = 10: DSRDDS / DRESER / SRDRRD / REDESE / DSSRDR / DREDDS
// 6,7 = 12: DSRDDRS / DRESRED / SRDERSR / RESRDDE / DDSREES / SREDSRD
// 7,7 = 15: DSERDRS / REDSRED / SRDDRSR / DRESEDE / RSRDRRS / EDRESED / SDSRDRS
#include <cstdio>
int main() {
    const int answer[6][6] = {
        {0, 1, 1, 2, 2, 3},
        {1, 2, 2, 4, 4, 6},
        {1, 2, 3, 5, 6, 7},
        {2, 4, 5, 6, 8, 10},
        {2, 4, 6, 8, 10, 12},
        {3, 6, 7, 10, 12, 15},
    };
    int n, m;
    while (std::scanf("%d%d", &n, &m) == 2 && (n || m))
        std::printf("%d\n", answer[n-2][m-2]);
    return 0;
}
