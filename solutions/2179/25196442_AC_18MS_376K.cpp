// POJ 2179 - Inlay Cutters
// Model: gpt-5.6-terra
// Approach: Scale coordinates by two, mark every cut intersection, and inspect
// each possible right-angle vertex.  A triangular piece exists exactly when
// the nearest vertices on two perpendicular rays are joined by one unbroken
// segment of a cut.  The plate border is treated as four additional cuts.

#include <iostream>

using namespace std;

const int MAX_LINES = 304;
const int MAX_COORD = 100;

enum Direction {
    HORIZONTAL = 0,
    VERTICAL = 1,
    DIAGONAL_UP = 2,
    DIAGONAL_DOWN = 3
};

struct Line {
    int x1;
    int y1;
    int x2;
    int y2;
    int direction;
};

int width;
int height;
int lineCount;
Line lines[MAX_LINES];
bool vertex[MAX_COORD + 1][MAX_COORD + 1];

void swapInt(int &a, int &b) {
    int temporary = a;
    a = b;
    b = temporary;
}

int setDirectionAndOrder(Line &line) {
    int direction;
    if (line.y1 == line.y2) {
        direction = HORIZONTAL;
        if (line.x1 > line.x2) {
            swapInt(line.x1, line.x2);
            swapInt(line.y1, line.y2);
        }
    } else if (line.x1 == line.x2) {
        direction = VERTICAL;
        if (line.y1 > line.y2) {
            swapInt(line.x1, line.x2);
            swapInt(line.y1, line.y2);
        }
    } else if (line.x2 - line.x1 == line.y2 - line.y1) {
        direction = DIAGONAL_UP;
        if (line.x1 > line.x2) {
            swapInt(line.x1, line.x2);
            swapInt(line.y1, line.y2);
        }
    } else {
        direction = DIAGONAL_DOWN;
        if (line.x1 > line.x2) {
            swapInt(line.x1, line.x2);
            swapInt(line.y1, line.y2);
        }
    }
    line.direction = direction;
    return direction;
}

bool onLine(const Line &line, int x, int y) {
    if (line.direction == HORIZONTAL) {
        return y == line.y1;
    }
    if (line.direction == VERTICAL) {
        return x == line.x1;
    }
    if (line.direction == DIAGONAL_UP) {
        return x - line.x1 == y - line.y1;
    }
    return x - line.x1 == line.y1 - y;
}

void markVertex(int x, int y) {
    if (0 <= x && x <= width && 0 <= y && y <= height) {
        vertex[x][y] = true;
    }
}

int nearestVertex(int x, int y, int dx, int dy) {
    int distance = 1;
    x += dx;
    y += dy;
    while (0 <= x && x <= width && 0 <= y && y <= height) {
        if (vertex[x][y]) {
            return distance;
        }
        ++distance;
        x += dx;
        y += dy;
    }
    return -1;
}

int directionOfSegment(int &x1, int &y1, int &x2, int &y2) {
    Line segment;
    segment.x1 = x1;
    segment.y1 = y1;
    segment.x2 = x2;
    segment.y2 = y2;
    int direction = setDirectionAndOrder(segment);
    x1 = segment.x1;
    y1 = segment.y1;
    x2 = segment.x2;
    y2 = segment.y2;
    return direction;
}

bool isUnbrokenSegment(int x1, int y1, int x2, int y2) {
    int direction = directionOfSegment(x1, y1, x2, y2);
    static const int dx[4] = {1, 0, 1, 1};
    static const int dy[4] = {0, 1, 1, -1};

    for (int i = 0; i < lineCount; ++i) {
        if (lines[i].direction == direction &&
            onLine(lines[i], x1, y1) && onLine(lines[i], x2, y2)) {
            int distance = nearestVertex(x1, y1, dx[direction], dy[direction]);
            return distance > 0 &&
                   x1 + distance * dx[direction] == x2 &&
                   y1 + distance * dy[direction] == y2;
        }
    }
    return false;
}

void markAllIntersections() {
    for (int i = 0; i < lineCount; ++i) {
        for (int j = 0; j < lineCount; ++j) {
            if (i == j) {
                continue;
            }
            const Line &a = lines[i];
            const Line &b = lines[j];
            if (a.direction == VERTICAL && b.direction == HORIZONTAL) {
                markVertex(a.x1, b.y1);
            } else if (a.direction == VERTICAL && b.direction == DIAGONAL_UP) {
                markVertex(a.x1, b.y1 + a.x1 - b.x1);
            } else if (a.direction == VERTICAL && b.direction == DIAGONAL_DOWN) {
                markVertex(a.x1, b.y1 - a.x1 + b.x1);
            } else if (a.direction == HORIZONTAL && b.direction == DIAGONAL_UP) {
                markVertex(b.x1 + a.y1 - b.y1, a.y1);
            } else if (a.direction == HORIZONTAL && b.direction == DIAGONAL_DOWN) {
                markVertex(b.x1 - a.y1 + b.y1, a.y1);
            } else if (a.direction == DIAGONAL_UP && b.direction == DIAGONAL_DOWN) {
                int distance = (b.x1 + b.y1 - a.x1 - a.y1) / 2;
                markVertex(a.x1 + distance, a.y1 + distance);
            }
        }
    }
}

int countTriangles() {
    int answer = 0;
    for (int i = 0; i < lineCount; ++i) {
        if (lines[i].direction != VERTICAL) {
            continue;
        }
        for (int j = 0; j < lineCount; ++j) {
            if (lines[j].direction != HORIZONTAL) {
                continue;
            }
            int x = lines[i].x1;
            int y = lines[j].y1;
            int left = nearestVertex(x, y, -1, 0);
            int right = nearestVertex(x, y, 1, 0);
            int up = nearestVertex(x, y, 0, 1);
            int down = nearestVertex(x, y, 0, -1);
            if (left > 0 && down > 0 &&
                isUnbrokenSegment(x - left, y, x, y - down)) {
                ++answer;
            }
            if (left > 0 && up > 0 &&
                isUnbrokenSegment(x - left, y, x, y + up)) {
                ++answer;
            }
            if (right > 0 && down > 0 &&
                isUnbrokenSegment(x + right, y, x, y - down)) {
                ++answer;
            }
            if (right > 0 && up > 0 &&
                isUnbrokenSegment(x + right, y, x, y + up)) {
                ++answer;
            }
        }
    }

    for (int i = 0; i < lineCount; ++i) {
        if (lines[i].direction != DIAGONAL_UP) {
            continue;
        }
        for (int j = 0; j < lineCount; ++j) {
            if (lines[j].direction != DIAGONAL_DOWN) {
                continue;
            }
            int distance = (lines[j].x1 + lines[j].y1 -
                            lines[i].x1 - lines[i].y1) / 2;
            int x = lines[i].x1 + distance;
            int y = lines[i].y1 + distance;
            if (x < 0 || x > width || y < 0 || y > height) {
                continue;
            }
            int downLeft = nearestVertex(x, y, -1, -1);
            int upRight = nearestVertex(x, y, 1, 1);
            int upLeft = nearestVertex(x, y, -1, 1);
            int downRight = nearestVertex(x, y, 1, -1);
            if (downLeft > 0 && downRight > 0 &&
                isUnbrokenSegment(x - downLeft, y - downLeft,
                                  x + downRight, y - downRight)) {
                ++answer;
            }
            if (downLeft > 0 && upLeft > 0 &&
                isUnbrokenSegment(x - downLeft, y - downLeft,
                                  x - upLeft, y + upLeft)) {
                ++answer;
            }
            if (upRight > 0 && downRight > 0 &&
                isUnbrokenSegment(x + upRight, y + upRight,
                                  x + downRight, y - downRight)) {
                ++answer;
            }
            if (upRight > 0 && upLeft > 0 &&
                isUnbrokenSegment(x + upRight, y + upRight,
                                  x - upLeft, y + upLeft)) {
                ++answer;
            }
        }
    }
    return answer;
}

int main() {
    int cuts;
    cin >> width >> height >> cuts;
    width *= 2;
    height *= 2;

    lineCount = cuts;
    for (int i = 0; i < cuts; ++i) {
        cin >> lines[i].x1 >> lines[i].y1 >> lines[i].x2 >> lines[i].y2;
        lines[i].x1 *= 2;
        lines[i].y1 *= 2;
        lines[i].x2 *= 2;
        lines[i].y2 *= 2;
    }

    lines[lineCount].x1 = 0;
    lines[lineCount].y1 = 0;
    lines[lineCount].x2 = width;
    lines[lineCount++].y2 = 0;
    lines[lineCount].x1 = width;
    lines[lineCount].y1 = 0;
    lines[lineCount].x2 = width;
    lines[lineCount++].y2 = height;
    lines[lineCount].x1 = width;
    lines[lineCount].y1 = height;
    lines[lineCount].x2 = 0;
    lines[lineCount++].y2 = height;
    lines[lineCount].x1 = 0;
    lines[lineCount].y1 = height;
    lines[lineCount].x2 = 0;
    lines[lineCount++].y2 = 0;

    for (int i = 0; i < lineCount; ++i) {
        setDirectionAndOrder(lines[i]);
    }
    markAllIntersections();
    cout << countTriangles() << '\n';
    return 0;
}
