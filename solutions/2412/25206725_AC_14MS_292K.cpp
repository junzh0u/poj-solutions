// POJ 2412 - Equidistance
// Model: claude-sonnet-5
//
// Approach: convert each location's (lat, lon) to a unit Cartesian vector on
// the sphere. The set of points equidistant from A and B on the sphere's
// surface is the great circle cut out by the plane through the sphere's
// center with normal n = normalize(A - B) (since |PA|=|PB| for points on
// the sphere of radius R iff P.A == P.B iff P.(A-B) == 0). For a query point
// M (unit vector u), the great-circle (angular) distance from u to that
// great circle is asin(|u . n|); multiplying by the sphere radius R gives
// the surface distance. Verified against North/South pole sample query
// (90 degrees * R = 10019 km).
//
// Ambiguity handled per the problem's own discuss board (POJ 2412, message
// 15289, quoting the original Ulm judges): Alice and Bob's home may be the
// same location, in which case every point is trivially equidistant, so the
// answer is 0; this is not derivable from the formula above since the
// normal vector A-B is undefined (zero), so it is special-cased directly.
// A location name absent from the location list makes the whole query's
// distance unknown ("?"), regardless of which of the three names is
// missing; the printed names are always exactly as given in the query line.
//
// Precision: use double throughout (float caused precision-related WAs
// reported on the board) and round to nearest integer with floor(x+0.5)
// (distance is always non-negative).
#include <cstdio>
#include <cstring>
#include <cmath>
#include <map>
#include <string>

using namespace std;

static const double R = 6378.0;
static const double PI = 3.14159265358979323846;

struct Vec3 {
    double x, y, z;
};

Vec3 toVec(double latDeg, double lonDeg) {
    double lat = latDeg * PI / 180.0;
    double lon = lonDeg * PI / 180.0;
    Vec3 v;
    v.x = cos(lat) * cos(lon);
    v.y = cos(lat) * sin(lon);
    v.z = sin(lat);
    return v;
}

double dot(const Vec3 &a, const Vec3 &b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

Vec3 sub(const Vec3 &a, const Vec3 &b) {
    Vec3 v;
    v.x = a.x - b.x;
    v.y = a.y - b.y;
    v.z = a.z - b.z;
    return v;
}

double norm(const Vec3 &a) {
    return sqrt(dot(a, a));
}

int main() {
    map<string, Vec3> loc;
    char buf[64];
    while (scanf("%63s", buf) == 1) {
        if (strcmp(buf, "#") == 0) break;
        double lat, lon;
        scanf("%lf %lf", &lat, &lon);
        loc[string(buf)] = toVec(lat, lon);
    }

    char aName[64], bName[64], mName[64];
    while (scanf("%63s", aName) == 1) {
        if (strcmp(aName, "#") == 0) break;
        scanf("%63s %63s", bName, mName);

        map<string, Vec3>::iterator itA = loc.find(aName);
        map<string, Vec3>::iterator itB = loc.find(bName);
        map<string, Vec3>::iterator itM = loc.find(mName);

        if (itA == loc.end() || itB == loc.end() || itM == loc.end()) {
            printf("%s is ? km off %s/%s equidistance.\n", mName, aName, bName);
            continue;
        }

        Vec3 A = itA->second, B = itB->second, M = itM->second;
        Vec3 n = sub(A, B);
        double nlen = norm(n);

        double distKm;
        if (nlen < 1e-12) {
            // A and B coincide: every point is trivially equidistant.
            distKm = 0.0;
        } else {
            double t = dot(M, n) / nlen; // = u_M . normalize(n), in [-1,1] up to fp error
            if (t > 1.0) t = 1.0;
            if (t < -1.0) t = -1.0;
            double angle = asin(fabs(t));
            distKm = R * angle;
        }

        long rounded = (long)floor(distKm + 0.5);
        printf("%s is %ld km off %s/%s equidistance.\n", mName, rounded, aName, bName);
    }

    return 0;
}
