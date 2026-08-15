// POJ 2254 - Globetrotter
// Model: claude-sonnet-5
// Approach: read cities (name, lat, lon) terminated by a line "#", storing
// each city's unit-sphere Cartesian coordinates. Read queries (two names)
// terminated by "# #"; for each, look up both names by linear scan (<=100
// cities, so brute force lookup is fine) and report "Unknown" if either is
// missing, otherwise the great-circle distance R*acos(dot product) rounded
// to the nearest integer, printing a blank line after every query.
// Ambiguity: none of substance; the board's only real note is to avoid
// printf's %lf (undefined pre-C99) - we use iostream instead, sidestepping
// the whole format-specifier question.
#include <cstdio>
#include <cstring>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <string>
using namespace std;

const double PI = 3.141592653589793;
const double R = 6378.0;

struct City {
    string name;
    double x, y, z;
};

int main() {
    City cities[200];
    int n = 0;
    string tok;
    while (cin >> tok) {
        if (tok == "#") break;
        double lat, lon;
        cin >> lat >> lon;
        double latr = lat * PI / 180.0;
        double lonr = lon * PI / 180.0;
        cities[n].name = tok;
        cities[n].x = cos(latr) * cos(lonr);
        cities[n].y = cos(latr) * sin(lonr);
        cities[n].z = sin(latr);
        n++;
    }

    string a, b;
    while (cin >> a) {
        cin >> b;
        if (a == "#" && b == "#") break;
        cout << a << " - " << b << "\n";
        int ia = -1, ib = -1;
        for (int i = 0; i < n; i++) {
            if (cities[i].name == a) ia = i;
            if (cities[i].name == b) ib = i;
        }
        if (ia == -1 || ib == -1) {
            cout << "Unknown\n";
        } else {
            double dot = cities[ia].x * cities[ib].x +
                         cities[ia].y * cities[ib].y +
                         cities[ia].z * cities[ib].z;
            if (dot > 1.0) dot = 1.0;
            if (dot < -1.0) dot = -1.0;
            double ang = acos(dot);
            double dist = R * ang;
            long rounded = (long)floor(dist + 0.5);
            cout << rounded << " km\n";
        }
        cout << "\n";
    }
    return 0;
}
