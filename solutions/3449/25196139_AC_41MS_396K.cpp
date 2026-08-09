// POJ 3449 - Geometric Shapes
// Model: gpt-5.6-terra
// Scale coordinates by two so square vertices and all segment tests stay exact.
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>

struct Point {
    long long x, y;
    Point() : x(0), y(0) {}
    Point(long long xx, long long yy) : x(xx), y(yy) {}
};

struct Shape {
    char name;
    std::vector<Point> p;
};

static long long cross(const Point &a, const Point &b, const Point &c) {
    return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
}

static bool between(long long a, long long b, long long x) {
    return (a <= x && x <= b) || (b <= x && x <= a);
}

static bool on_segment(const Point &a, const Point &b, const Point &p) {
    return cross(a, b, p) == 0 && between(a.x, b.x, p.x) && between(a.y, b.y, p.y);
}

static int sign(long long x) {
    return x < 0 ? -1 : (x > 0 ? 1 : 0);
}

static bool intersect(const Point &a, const Point &b, const Point &c, const Point &d) {
    long long c1 = cross(a, b, c), c2 = cross(a, b, d);
    long long c3 = cross(c, d, a), c4 = cross(c, d, b);
    if (c1 == 0 && on_segment(a, b, c)) return true;
    if (c2 == 0 && on_segment(a, b, d)) return true;
    if (c3 == 0 && on_segment(c, d, a)) return true;
    if (c4 == 0 && on_segment(c, d, b)) return true;
    return sign(c1) != sign(c2) && sign(c3) != sign(c4);
}

static bool shapes_intersect(const Shape &a, const Shape &b) {
    int an = (int)a.p.size(), bn = (int)b.p.size();
    int ae = an == 2 ? 1 : an;
    int be = bn == 2 ? 1 : bn;
    int i, j;
    for (i = 0; i < ae; ++i)
        for (j = 0; j < be; ++j)
            if (intersect(a.p[i], a.p[(i + 1) % an], b.p[j], b.p[(j + 1) % bn])) return true;
    return false;
}

static std::vector<long long> numbers(const std::string &s) {
    std::vector<long long> result;
    int i = 0, n = (int)s.size();
    while (i < n) {
        if (s[i] == '-' || (s[i] >= '0' && s[i] <= '9')) {
            int neg = s[i] == '-';
            if (neg && (i + 1 == n || s[i + 1] < '0' || s[i + 1] > '9')) { ++i; continue; }
            if (neg) ++i;
            long long value = 0;
            while (i < n && s[i] >= '0' && s[i] <= '9') value = value * 10 + s[i++] - '0';
            result.push_back(neg ? -value : value);
        } else ++i;
    }
    return result;
}

static Point point_at(const std::vector<long long> &v, int index) {
    return Point(2 * v[index], 2 * v[index + 1]);
}

static Shape parse(const std::string &line) {
    std::stringstream input(line);
    Shape shape;
    std::string kind;
    input >> shape.name >> kind;
    std::vector<long long> v = numbers(line);
    if (kind == "line") {
        shape.p.push_back(point_at(v, 0));
        shape.p.push_back(point_at(v, 2));
    } else if (kind == "triangle") {
        int i;
        for (i = 0; i < 6; i += 2) shape.p.push_back(point_at(v, i));
    } else if (kind == "polygon") {
        int count = (int)v[0];
        int i;
        for (i = 0; i < count; ++i) shape.p.push_back(point_at(v, 1 + 2 * i));
    } else if (kind == "rectangle") {
        Point a = point_at(v, 0), b = point_at(v, 2), c = point_at(v, 4);
        shape.p.push_back(a); shape.p.push_back(b); shape.p.push_back(c);
        shape.p.push_back(Point(a.x + c.x - b.x, a.y + c.y - b.y));
    } else {
        Point a = point_at(v, 0), c = point_at(v, 2);
        Point b(a.x / 2 + c.x / 2 + a.y / 2 - c.y / 2,
                a.y / 2 + c.y / 2 - a.x / 2 + c.x / 2);
        Point d(a.x + c.x - b.x, a.y + c.y - b.y);
        shape.p.push_back(a); shape.p.push_back(b); shape.p.push_back(c); shape.p.push_back(d);
    }
    return shape;
}

static bool name_before(const Shape &a, const Shape &b) {
    return a.name < b.name;
}

int main() {
    std::string line;
    while (std::getline(std::cin, line)) {
        if (line == ".") break;
        std::vector<Shape> shapes;
        while (line != "-") {
            shapes.push_back(parse(line));
            std::getline(std::cin, line);
        }
        std::sort(shapes.begin(), shapes.end(), name_before);
        int i, j;
        for (i = 0; i < (int)shapes.size(); ++i) {
            std::vector<char> hit;
            for (j = 0; j < (int)shapes.size(); ++j)
                if (i != j && shapes_intersect(shapes[i], shapes[j])) hit.push_back(shapes[j].name);
            std::cout << shapes[i].name << ' ';
            if (hit.empty()) std::cout << "has no intersections\n";
            else {
                std::cout << "intersects with ";
                std::cout << hit[0];
                for (j = 1; j < (int)hit.size(); ++j) {
                    if (j + 1 == (int)hit.size())
                        std::cout << (hit.size() == 2 ? " and " : ", and ");
                    else
                        std::cout << ", ";
                    std::cout << hit[j];
                }
                std::cout << '\n';
            }
        }
        std::cout << '\n';
    }
    return 0;
}
