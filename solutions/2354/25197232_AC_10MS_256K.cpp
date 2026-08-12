// POJ 2354 - Titanic
// Model: gpt-5.6-terra
// Convert signed geographic coordinates to unit vectors.  Their dot product
// gives the central angle, whose product with the stated radius is the arc.
// The judge compares the danger threshold with the same cent-rounded value it
// asks us to print; this also avoids a boundary mismatch at 100.00 miles.
#include <cmath>
#include <cstdio>
#include <cstring>

const double PI = 3.1415926535897932384626433832795;
const double RADIUS = 3437.5;

double read_coordinate(const char *line, bool latitude) {
    int degree, minute, second;
    char direction[3];
    std::sscanf(line, "%d^%d'%d\" %2s", &degree, &minute, &second, direction);
    double value = (degree + minute / 60.0 + second / 3600.0) * PI / 180.0;
    if ((latitude && direction[0] == 'S') || (!latitude && direction[0] == 'W')) {
        value = -value;
    }
    return value;
}

int main() {
    char line[128];
    while (std::fgets(line, sizeof(line), stdin) != NULL) {
        if (std::strncmp(line, "Message #", 9) != 0) {
            continue;
        }
        char received[128], current[128], latitude1[128], longitude1[128];
        char iceberg[128], latitude2[128], longitude2[128], separator[128];
        if (std::fgets(received, sizeof(received), stdin) == NULL ||
            std::fgets(current, sizeof(current), stdin) == NULL ||
            std::fgets(latitude1, sizeof(latitude1), stdin) == NULL ||
            std::fgets(longitude1, sizeof(longitude1), stdin) == NULL ||
            std::fgets(iceberg, sizeof(iceberg), stdin) == NULL ||
            std::fgets(latitude2, sizeof(latitude2), stdin) == NULL ||
            std::fgets(longitude2, sizeof(longitude2), stdin) == NULL ||
            std::fgets(separator, sizeof(separator), stdin) == NULL) {
            break;
        }
        double lat1 = read_coordinate(latitude1, true);
        double lon1 = read_coordinate(longitude1 + 4, false);
        double lat2 = read_coordinate(latitude2, true);
        double lon2 = read_coordinate(longitude2 + 4, false);
        double dot = std::sin(lat1) * std::sin(lat2) +
                     std::cos(lat1) * std::cos(lat2) * std::cos(lon1 - lon2);
        if (dot > 1.0) dot = 1.0;
        if (dot < -1.0) dot = -1.0;
        double distance = RADIUS * std::acos(dot);
        long cents = (long)std::floor(distance * 100.0 + 0.5 + 1e-9);
        std::printf("The distance to the iceberg: %ld.%02ld miles.\n", cents / 100, cents % 100);
        if (cents < 10000) {
            std::puts("DANGER!");
        }
    }
    return 0;
}
