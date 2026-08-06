// POJ 2771 - Guardian of Decency
// Model: gpt-5.6-terra
// Build an edge for each possible opposite-sex couple, then use Konig's theorem:
// the largest eligible set is N minus a maximum bipartite matching of those edges.
// A potential couple has height difference at most 40, equal music, and different sports.
#include <cstdlib>
#include <iostream>
#include <queue>
#include <string>
#include <vector>

using namespace std;

struct Pupil {
    int height;
    char sex;
    string music;
    string sport;
};

class HopcroftKarp {
public:
    HopcroftKarp(int leftCount, int rightCount)
        : leftCount(leftCount), graph(leftCount),
          matchLeft(leftCount, -1), matchRight(rightCount, -1), distance(leftCount), shortestFreeDistance(-1) {
    }

    void addEdge(int left, int right) {
        graph[left].push_back(right);
    }

    int maximumMatching() {
        int matching = 0;
        while (buildLayers()) {
            int left;
            for (left = 0; left < leftCount; ++left) {
                if (matchLeft[left] == -1 && augment(left)) ++matching;
            }
        }
        return matching;
    }

private:
    int leftCount;
    vector< vector<int> > graph;
    vector<int> matchLeft;
    vector<int> matchRight;
    vector<int> distance;
    int shortestFreeDistance;

    bool buildLayers() {
        queue<int> pending;
        int left;
        for (left = 0; left < leftCount; ++left) {
            if (matchLeft[left] == -1) {
                distance[left] = 0;
                pending.push(left);
            } else {
                distance[left] = -1;
            }
        }

        shortestFreeDistance = -1;
        while (!pending.empty()) {
            int current = pending.front();
            pending.pop();
            if (shortestFreeDistance != -1 && distance[current] >= shortestFreeDistance) continue;
            int i;
            for (i = 0; i < (int)graph[current].size(); ++i) {
                int nextLeft = matchRight[graph[current][i]];
                if (nextLeft == -1) {
                    if (shortestFreeDistance == -1) shortestFreeDistance = distance[current] + 1;
                } else if (distance[nextLeft] == -1) {
                    distance[nextLeft] = distance[current] + 1;
                    pending.push(nextLeft);
                }
            }
        }
        return shortestFreeDistance != -1;
    }

    bool augment(int left) {
        int i;
        for (i = 0; i < (int)graph[left].size(); ++i) {
            int right = graph[left][i];
            int nextLeft = matchRight[right];
            if ((nextLeft == -1 && distance[left] + 1 == shortestFreeDistance) ||
                (nextLeft != -1 && distance[nextLeft] == distance[left] + 1 && augment(nextLeft))) {
                matchLeft[left] = right;
                matchRight[right] = left;
                return true;
            }
        }
        distance[left] = -1;
        return false;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);

    int cases;
    cin >> cases;
    while (cases--) {
        int n;
        cin >> n;
        vector<Pupil> pupils(n);
        vector<int> males;
        vector<int> females;
        int i;
        for (i = 0; i < n; ++i) {
            cin >> pupils[i].height >> pupils[i].sex >> pupils[i].music >> pupils[i].sport;
            if (pupils[i].sex == 'M') {
                males.push_back(i);
            } else {
                females.push_back(i);
            }
        }

        HopcroftKarp matching((int)males.size(), (int)females.size());
        int male;
        for (male = 0; male < (int)males.size(); ++male) {
            int female;
            for (female = 0; female < (int)females.size(); ++female) {
                const Pupil& a = pupils[males[male]];
                const Pupil& b = pupils[females[female]];
                if (abs(a.height - b.height) <= 40 && a.music == b.music && a.sport != b.sport) {
                    matching.addEdge(male, female);
                }
            }
        }
        cout << n - matching.maximumMatching() << '\n';
    }
    return 0;
}
