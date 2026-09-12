// POJ 2026 - As the Crow Flies
// Model: claude-sonnet-5
//
// Approach: An itinerary must follow the flight network (the given legs),
// taking the network's shortest path from origin to destination; the length
// of each leg used is the great-circle ("as the crow flies") distance
// between the two cities it directly connects. So build a graph whose edge
// weights are great-circle distances (sphere radius 4000) between the
// endpoints of each given flight leg, run all-pairs shortest paths
// (Floyd-Warshall, n<100), and report the pair whose shortest-path distance
// is maximum, names sorted alphabetically. (Confirmed against the sample:
// the diameter pair Equatorfour/Equatorone, reached only via a 3-leg path
// through the middle cities, has a longer shortest-path distance than the
// antipodal Northpole/Southpole pair, which is directly connected by a
// shorter chain of legs -- so the answer is NOT simply the two points that
// are geometrically farthest apart on the sphere, and the network structure
// is essential, not a red herring.)
//
// Ambiguity: none in the statement itself; the discuss board's only
// relevant notes are that latitude 0 has no N/S distinction and a caution
// about accidentally mutating input data mid-computation -- both handled
// naturally here since parsed doubles feed straight into a separate
// distance matrix that is never overwritten in place.
#include <cstdio>
#include <cstring>
#include <cmath>
#include <string>
#include <map>
#include <algorithm>
using namespace std;

static const double R = 4000.0;
static const double PI = 3.14159265358979323846;
static const int MAXC = 105;

double toRad(double d) { return d * PI / 180.0; }

double greatCircle(double lat1, double lon1, double lat2, double lon2) {
    double p1 = toRad(lat1), l1 = toRad(lon1);
    double p2 = toRad(lat2), l2 = toRad(lon2);
    double cosang = sin(p1) * sin(p2) + cos(p1) * cos(p2) * cos(l1 - l2);
    if (cosang > 1.0) cosang = 1.0;
    if (cosang < -1.0) cosang = -1.0;
    return R * acos(cosang);
}

char names[MAXC][40];
double lat[MAXC], lon[MAXC];
double dist[MAXC][MAXC];

int main() {
    int n;
    if (scanf("%d", &n) != 1) return 0;
    while (n--) {
        int X, Y;
        scanf("%d %d", &X, &Y);
        map<string, int> idx;
        for (int i = 0; i < X; i++) {
            char name[40], ns, ew;
            int la, lo;
            scanf("%s %d %c %d %c", name, &la, &ns, &lo, &ew);
            strcpy(names[i], name);
            double latVal = (double)la;
            if (ns == 'S' || ns == 's') latVal = -latVal;
            double lonVal = (double)lo;
            if (ew == 'W' || ew == 'w') lonVal = -lonVal;
            lat[i] = latVal;
            lon[i] = lonVal;
            idx[string(name)] = i;
        }
        for (int i = 0; i < X; i++)
            for (int j = 0; j < X; j++)
                dist[i][j] = (i == j) ? 0.0 : 1e18;
        for (int e = 0; e < Y; e++) {
            char a[40], b[40];
            scanf("%s %s", a, b);
            int ia = idx[string(a)];
            int ib = idx[string(b)];
            double d = greatCircle(lat[ia], lon[ia], lat[ib], lon[ib]);
            if (d < dist[ia][ib]) {
                dist[ia][ib] = d;
                dist[ib][ia] = d;
            }
        }
        for (int k = 0; k < X; k++)
            for (int i = 0; i < X; i++) {
                if (dist[i][k] >= 1e17) continue;
                for (int j = 0; j < X; j++) {
                    double nd = dist[i][k] + dist[k][j];
                    if (nd < dist[i][j]) dist[i][j] = nd;
                }
            }
        double best = -1.0;
        int bi = -1, bj = -1;
        for (int i = 0; i < X; i++)
            for (int j = i + 1; j < X; j++) {
                if (dist[i][j] > best) {
                    best = dist[i][j];
                    bi = i;
                    bj = j;
                }
            }
        string s1 = names[bi];
        string s2 = names[bj];
        if (s2 < s1) swap(s1, s2);
        printf("%s %s\n", s1.c_str(), s2.c_str());
    }
    return 0;
}
