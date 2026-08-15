// POJ 2571 - Global Roaming
// Model: claude-sonnet-5
// Approach: Convert each location and the satellite to 3D Cartesian
// coordinates on/above a sphere of radius R = 6378 km (satellite at
// radius R+h along its own latitude/longitude direction). A location P
// (|P|=R) has the satellite S in view iff S lies on the outward side of
// the tangent plane at P, i.e. (S-P).P >= 0, which simplifies to
// S.P >= R*R. No ambiguity in the statement; discuss board (msg 33301)
// confirms this exact dot-product formula (vector center->P as the
// tangent-plane normal, not center->S). Blank line printed after every
// test case, including the last, per the statement.
#include <cstdio>
#include <cstring>
#include <cmath>

static const double R = 6378.0;
static const double PI = 3.14159265358979323846;

struct Vec {
    double x, y, z;
};

Vec toVec(double latDeg, double lonDeg, double radius) {
    double lat = latDeg * PI / 180.0;
    double lon = lonDeg * PI / 180.0;
    Vec v;
    v.x = radius * cos(lat) * cos(lon);
    v.y = radius * cos(lat) * sin(lon);
    v.z = radius * sin(lat);
    return v;
}

int main() {
    int n;
    int tc = 0;
    while (scanf("%d", &n) == 1 && n != 0) {
        double slat, slon, sh;
        scanf("%lf %lf %lf", &slat, &slon, &sh);
        Vec S = toVec(slat, slon, R + sh);
        double R2 = R * R;

        ++tc;
        printf("Test case %d:\n", tc);
        for (int i = 0; i < n; ++i) {
            char label[70];
            double lat, lon;
            scanf("%s %lf %lf", label, &lat, &lon);
            Vec P = toVec(lat, lon, R);
            double dot = P.x * S.x + P.y * S.y + P.z * S.z;
            if (dot >= R2 - 1e-6) {
                printf("%s\n", label);
            }
        }
        printf("\n");
    }
    return 0;
}
