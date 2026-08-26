// POJ 2467 - Snow Clearing
// Model: gpt-5.6-terra
// Every street has one lane in each direction.  Directing each physical
// street both ways gives a balanced connected graph, hence it has an Euler
// tour from the hangar.  The plow clears each lane once, so only twice the
// total street length at the plowing speed is needed.
// The public board confirms that the hangar coordinates are read but do not
// contribute to the time; minutes must be rounded and printed with two digits.

#include <cmath>
#include <cstdio>

int main() {
    double hangarX, hangarY;
    if (std::scanf("%lf%lf", &hangarX, &hangarY) != 2) {
        return 0;
    }

    double totalMetres = 0.0;
    double x1, y1, x2, y2;
    while (std::scanf("%lf%lf%lf%lf", &x1, &y1, &x2, &y2) == 4) {
        const double dx = x1 - x2;
        const double dy = y1 - y2;
        totalMetres += std::sqrt(dx * dx + dy * dy);
    }

    const int minutes = static_cast<int>(totalMetres * 2.0 * 60.0 / 20000.0 + 0.5);
    std::printf("%d:%02d\n", minutes / 60, minutes % 60);
    return 0;
}
