// POJ 2711 - Leapin' Lizards
// Model: gpt-5.6-terra
// Split every pillar into capacity-limited in/out nodes and maximize the flow
// of lizards from their starting pillars to pillars that can reach safety.
// The graph uses the values as read, including POJ data with d=4 and capacities
// greater than the stated upper bound of 3.
#include <algorithm>
#include <iostream>
#include <queue>
#include <string>
#include <vector>

using namespace std;

struct Edge {
    int to;
    int capacity;
    int reverse;

    Edge(int destination, int edgeCapacity, int reverseIndex)
        : to(destination), capacity(edgeCapacity), reverse(reverseIndex) {}
};

class Dinic {
public:
    explicit Dinic(int nodeCount)
        : graph_(nodeCount), level_(nodeCount), nextEdge_(nodeCount) {}

    void addEdge(int from, int to, int capacity) {
        Edge forward(to, capacity, static_cast<int>(graph_[to].size()));
        Edge backward(from, 0, static_cast<int>(graph_[from].size()));
        graph_[from].push_back(forward);
        graph_[to].push_back(backward);
    }

    int maximumFlow(int source, int sink) {
        int flow = 0;
        while (buildLevels(source, sink)) {
            fill(nextEdge_.begin(), nextEdge_.end(), 0);
            int pushed;
            while ((pushed = sendFlow(source, sink, 1000000000)) > 0) {
                flow += pushed;
            }
        }
        return flow;
    }

private:
    vector<vector<Edge> > graph_;
    vector<int> level_;
    vector<int> nextEdge_;

    bool buildLevels(int source, int sink) {
        fill(level_.begin(), level_.end(), -1);
        queue<int> pending;
        level_[source] = 0;
        pending.push(source);
        while (!pending.empty()) {
            const int node = pending.front();
            pending.pop();
            for (size_t i = 0; i < graph_[node].size(); ++i) {
                const Edge& edge = graph_[node][i];
                if (edge.capacity > 0 && level_[edge.to] == -1) {
                    level_[edge.to] = level_[node] + 1;
                    pending.push(edge.to);
                }
            }
        }
        return level_[sink] != -1;
    }

    int sendFlow(int node, int sink, int available) {
        if (node == sink) {
            return available;
        }
        for (int& index = nextEdge_[node];
             index < static_cast<int>(graph_[node].size()); ++index) {
            Edge& edge = graph_[node][index];
            if (edge.capacity == 0 || level_[edge.to] != level_[node] + 1) {
                continue;
            }
            const int pushed = sendFlow(edge.to, sink,
                                        min(available, edge.capacity));
            if (pushed > 0) {
                edge.capacity -= pushed;
                graph_[edge.to][edge.reverse].capacity += pushed;
                return pushed;
            }
        }
        return 0;
    }
};

int main() {
    int testCases;
    cin >> testCases;
    for (int caseNumber = 1; caseNumber <= testCases; ++caseNumber) {
        int rows;
        int leap;
        cin >> rows >> leap;
        vector<string> pillars(rows);
        for (int row = 0; row < rows; ++row) {
            cin >> pillars[row];
        }
        const int columns = static_cast<int>(pillars[0].size());
        vector<string> lizards(rows);
        for (int row = 0; row < rows; ++row) {
            cin >> lizards[row];
        }

        const int cellCount = rows * columns;
        const int source = 2 * cellCount;
        const int sink = source + 1;
        Dinic network(sink + 1);
        int lizardCount = 0;
        const int infinite = 1000000000;

        for (int row = 0; row < rows; ++row) {
            for (int column = 0; column < columns; ++column) {
                if (pillars[row][column] == '0') {
                    continue;
                }
                const int cell = row * columns + column;
                const int inNode = 2 * cell;
                const int outNode = inNode + 1;
                network.addEdge(inNode, outNode,
                                pillars[row][column] - '0');
                if (lizards[row][column] == 'L') {
                    network.addEdge(source, inNode, 1);
                    ++lizardCount;
                }
                if (row < leap || row >= rows - leap || column < leap ||
                    column >= columns - leap) {
                    network.addEdge(outNode, sink, infinite);
                }

                for (int otherRow = 0; otherRow < rows; ++otherRow) {
                    for (int otherColumn = 0; otherColumn < columns;
                         ++otherColumn) {
                        if (pillars[otherRow][otherColumn] == '0' ||
                            (row == otherRow && column == otherColumn)) {
                            continue;
                        }
                        const int deltaRow = row - otherRow;
                        const int deltaColumn = column - otherColumn;
                        if (deltaRow * deltaRow + deltaColumn * deltaColumn <=
                            leap * leap) {
                            const int otherCell =
                                otherRow * columns + otherColumn;
                            network.addEdge(outNode, 2 * otherCell, infinite);
                        }
                    }
                }
            }
        }

        const int casualties =
            lizardCount - network.maximumFlow(source, sink);
        cout << "Case #" << caseNumber << ": ";
        if (casualties == 0) {
            cout << "no lizard was left behind.\n";
        } else if (casualties == 1) {
            cout << "1 lizard was left behind.\n";
        } else {
            cout << casualties << " lizards were left behind.\n";
        }
    }
    return 0;
}
