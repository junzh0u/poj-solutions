// POJ 1139 - Cat and Mouse
// Model: gpt-5.6-terra
// Reachability identifies rooms both animals can visit.  For the second
// answer, delete every cat-reachable room and look for a cycle through the
// mouse home that visits a different room.
#include <iostream>
#include <cstring>

using namespace std;

int main() {
    int cases;
    cin >> cases;
    while (cases--) {
        int n, catHome, mouseHome;
        bool catDoor[101][101], mouseDoor[101][101];
        bool catReach[101], mouseReach[101], safeReach[101];
        memset(catDoor, 0, sizeof(catDoor));
        memset(mouseDoor, 0, sizeof(mouseDoor));
        cin >> n >> catHome >> mouseHome;

        int from, to;
        while (cin >> from >> to && from != -1)
            catDoor[from][to] = true;
        while (cin >> from >> to && from != -1)
            mouseDoor[from][to] = true;

        memset(catReach, 0, sizeof(catReach));
        catReach[catHome] = true;
        for (int repeat = 1; repeat <= n; ++repeat)
            for (int u = 1; u <= n; ++u)
                if (catReach[u])
                    for (int v = 1; v <= n; ++v)
                        if (catDoor[u][v]) catReach[v] = true;

        memset(mouseReach, 0, sizeof(mouseReach));
        mouseReach[mouseHome] = true;
        for (int repeat = 1; repeat <= n; ++repeat)
            for (int u = 1; u <= n; ++u)
                if (mouseReach[u])
                    for (int v = 1; v <= n; ++v)
                        if (mouseDoor[u][v]) mouseReach[v] = true;
        bool meet = false;
        for (int v = 1; v <= n; ++v)
            if (catReach[v] && mouseReach[v]) meet = true;

        memset(safeReach, 0, sizeof(safeReach));
        if (!catReach[mouseHome]) safeReach[mouseHome] = true;
        for (int repeat = 1; repeat <= n; ++repeat)
            for (int u = 1; u <= n; ++u)
                if (safeReach[u])
                    for (int v = 1; v <= n; ++v)
                        if (!catReach[v] && mouseDoor[u][v]) safeReach[v] = true;
        // Reverse reachability to the mouse home in the safe subgraph.
        bool back[101];
        memset(back, 0, sizeof(back));
        if (!catReach[mouseHome]) back[mouseHome] = true;
        for (int repeat = 1; repeat <= n; ++repeat)
            for (int v = 1; v <= n; ++v)
                if (back[v])
                    for (int u = 1; u <= n; ++u)
                        if (!catReach[u] && mouseDoor[u][v]) back[u] = true;
        bool roundTrip = false;
        for (int v = 1; v <= n; ++v)
            if (v != mouseHome && safeReach[v] && back[v]) roundTrip = true;

        cout << (meet ? 'Y' : 'N') << ' ' << (roundTrip ? 'Y' : 'N') << '\n';
    }
    return 0;
}
