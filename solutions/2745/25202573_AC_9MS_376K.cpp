// POJ 2745 - Network Mess
// Model: claude-sonnet-5
// Approach: Reconstruct the unweighted tree (unit-length edges) from the
// leaf-to-leaf distance matrix by inserting leaves one at a time, rooted at
// leaf 1. For leaf k, and each already-placed leaf i, the depth of their
// lowest common ancestor (distance from root) is
//   L(i,k) = (dist(1,i) + dist(1,k) - dist(i,k)) / 2.
// The leaf i maximizing L(i,k) identifies where k branches off the existing
// tree (since all edges have integer length 1, this LCA depth always lands
// exactly on an already-built node on i's root path). A new unit-edge chain
// is then grown from that node down to a fresh leaf node for k. After all N
// leaves are inserted, every node that is not one of the N leaves is a
// switch; its degree is the number of tree edges incident to it. Output all
// switch degrees ascending.
// No ambiguity in the statement; verified against the samples and against
// randomly generated ground-truth trees (build a random tree with degree-1
// leaves/degree>=2 switches, compute its leaf distance matrix, reconstruct,
// and compare the multiset of switch degrees to the true one).
#include <cstdio>
#include <vector>
#include <algorithm>
using namespace std;

int a[55][55];

int main() {
    int n;
    while (scanf("%d", &n) == 1 && n != 0) {
        for (int i = 1; i <= n; i++)
            for (int j = 1; j <= n; j++)
                scanf("%d", &a[i][j]);

        vector<int> degree;
        vector<bool> isLeafNode;
        vector<vector<int> > path(n + 1);

        // root = leaf 1
        degree.push_back(0);
        isLeafNode.push_back(true);
        path[1].push_back(0);

        for (int k = 2; k <= n; k++) {
            int Dk = a[1][k];
            int bestI = 1, bestM = 0; // L(1,k) is always 0
            for (int i = 1; i < k; i++) {
                int Di = a[1][i];
                int L = (Di + Dk - a[i][k]) / 2;
                if (L > bestM) {
                    bestM = L;
                    bestI = i;
                }
            }
            int m = bestM;
            int P = path[bestI][m];
            int prev = P;
            vector<int> newPath(path[bestI].begin(), path[bestI].begin() + m + 1);
            for (int d = m + 1; d <= Dk - 1; d++) {
                int id = (int) degree.size();
                degree.push_back(0);
                isLeafNode.push_back(false);
                degree[prev]++;
                degree[id]++;
                newPath.push_back(id);
                prev = id;
            }
            int leafId = (int) degree.size();
            degree.push_back(0);
            isLeafNode.push_back(true);
            degree[prev]++;
            degree[leafId]++;
            newPath.push_back(leafId);
            path[k] = newPath;
        }

        vector<int> switchDeg;
        for (size_t id = 0; id < degree.size(); id++) {
            if (!isLeafNode[id]) switchDeg.push_back(degree[id]);
        }
        sort(switchDeg.begin(), switchDeg.end());
        for (size_t i = 0; i < switchDeg.size(); i++) {
            if (i) printf(" ");
            printf("%d", switchDeg[i]);
        }
        printf("\n");
    }
    return 0;
}
