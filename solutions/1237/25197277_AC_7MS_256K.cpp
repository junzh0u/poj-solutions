// POJ 1237 - The Postal Worker Rings Once
// Model: gpt-5.6-terra
// Each route has zero or two odd-degree intersections.  Its shortest closed
// postman tour uses every edge once, plus the shortest path between the two
// odd vertices when they exist.  Floyd-Warshall on the 26 possible letters
// supplies that path; routes are read through EOF, one per "deadend" marker.

#include <iostream>
#include <string>

using namespace std;

const int INF = 1000000000;

int main() {
    string street;
    while (cin >> street) {
        int dist[26][26];
        int degree[26];
        int i, j, k;
        for (i = 0; i < 26; ++i) {
            degree[i] = 0;
            for (j = 0; j < 26; ++j)
                dist[i][j] = (i == j ? 0 : INF);
        }

        int total = 0;
        while (street != "deadend") {
            int a = street[0] - 'a';
            int b = street[street.size() - 1] - 'a';
            int length = (int)street.size();
            dist[a][b] = dist[b][a] = length;
            ++degree[a];
            ++degree[b];
            total += length;
            cin >> street;
        }

        for (k = 0; k < 26; ++k)
            for (i = 0; i < 26; ++i)
                for (j = 0; j < 26; ++j)
                    if (dist[i][k] + dist[k][j] < dist[i][j])
                        dist[i][j] = dist[i][k] + dist[k][j];

        int odd[2];
        int count = 0;
        for (i = 0; i < 26; ++i)
            if (degree[i] % 2 != 0)
                odd[count++] = i;
        if (count == 2)
            total += dist[odd[0]][odd[1]];
        cout << total << '\n';
    }
    return 0;
}
