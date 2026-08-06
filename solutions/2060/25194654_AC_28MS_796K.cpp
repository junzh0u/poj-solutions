// POJ 2060 - Taxi Cab Scheme
// Model: gpt-5.6-terra
// Build a DAG edge i -> j when one cab can finish ride i, travel to j's
// pickup, and arrive at least one minute before j departs.  The minimum path
// cover of this DAG is M minus a maximum bipartite matching.
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>

struct Ride {
    int start;
    int x1;
    int y1;
    int x2;
    int y2;
};

static int absInt(int x) {
    return x < 0 ? -x : x;
}

static bool canFollow(const Ride &first, const Ride &second) {
    int finish = first.start + absInt(first.x1 - first.x2) + absInt(first.y1 - first.y2);
    int travel = absInt(first.x2 - second.x1) + absInt(first.y2 - second.y1);
    return finish + travel < second.start;
}

static bool augment(int u, const std::vector<int> graph[500], int mateLeft[500], int mateRight[500], int distance[500]) {
    std::vector<int>::const_iterator it;
    for (it = graph[u].begin(); it != graph[u].end(); ++it) {
        int v = *it;
        if (mateRight[v] == -1 || (distance[mateRight[v]] == distance[u] + 1 && augment(mateRight[v], graph, mateLeft, mateRight, distance))) {
            mateLeft[u] = v;
            mateRight[v] = u;
            return true;
        }
    }
    distance[u] = -1;
    return false;
}

static int maximumMatching(int m, const std::vector<int> graph[500]) {
    int mateLeft[500];
    int mateRight[500];
    int distance[500];
    int queue[500];
    int matching = 0;
    int i;
    std::memset(mateLeft, -1, sizeof(mateLeft));
    std::memset(mateRight, -1, sizeof(mateRight));
    while (true) {
        int head = 0;
        int tail = 0;
        bool foundFreeRight = false;
        for (i = 0; i < m; ++i) {
            if (mateLeft[i] == -1) {
                distance[i] = 0;
                queue[tail++] = i;
            } else {
                distance[i] = -1;
            }
        }
        while (head < tail) {
            int u = queue[head++];
            std::vector<int>::const_iterator it;
            for (it = graph[u].begin(); it != graph[u].end(); ++it) {
                int paired = mateRight[*it];
                if (paired == -1) {
                    foundFreeRight = true;
                } else if (distance[paired] == -1) {
                    distance[paired] = distance[u] + 1;
                    queue[tail++] = paired;
                }
            }
        }
        if (!foundFreeRight) {
            break;
        }
        for (i = 0; i < m; ++i) {
            if (mateLeft[i] == -1 && augment(i, graph, mateLeft, mateRight, distance)) {
                ++matching;
            }
        }
    }
    return matching;
}

int main() {
    int cases;
    if (std::scanf("%d", &cases) != 1) {
        return 0;
    }
    while (cases--) {
        int m;
        Ride rides[500];
        std::vector<int> graph[500];
        int i, j, hour, minute, matching;
        std::scanf("%d", &m);
        for (i = 0; i < m; ++i) {
            std::scanf("%d:%d %d %d %d %d", &hour, &minute, &rides[i].x1, &rides[i].y1, &rides[i].x2, &rides[i].y2);
            rides[i].start = hour * 60 + minute;
        }
        for (i = 0; i < m; ++i) {
            for (j = i + 1; j < m; ++j) {
                if (canFollow(rides[i], rides[j])) {
                    graph[i].push_back(j);
                }
            }
        }
        matching = maximumMatching(m, graph);
        std::printf("%d\n", m - matching);
    }
    return 0;
}
