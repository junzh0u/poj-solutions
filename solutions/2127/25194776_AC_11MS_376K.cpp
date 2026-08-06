// POJ 2127 - Greatest Common Increasing Subsequence
// Model: gpt-5.6-terra
// Dynamic programming scans the first sequence and keeps the best LCIS ending
// at every position of the second sequence.  Persistent predecessor nodes keep
// reconstruction valid when a later update replaces an earlier DP state.
// Equal values are not allowed to extend a subsequence: only strictly smaller
// second-sequence values contribute to the running best for each first value.
#include <cstdio>
#include <vector>

struct Node {
    int value;
    int parent;
    Node(int value_, int parent_) : value(value_), parent(parent_) {}
};

int main() {
    int n;
    if (std::scanf("%d", &n) != 1) return 0;

    std::vector<int> a(n);
    int i;
    for (i = 0; i < n; ++i) std::scanf("%d", &a[i]);

    int m;
    std::scanf("%d", &m);
    std::vector<int> b(m);
    int j;
    for (j = 0; j < m; ++j) std::scanf("%d", &b[j]);

    std::vector<int> length(m, 0);
    std::vector<int> node_at(m, -1);
    std::vector<Node> nodes;

    for (i = 0; i < n; ++i) {
        int best_length = 0;
        int best_node = -1;
        for (j = 0; j < m; ++j) {
            if (a[i] == b[j] && best_length + 1 > length[j]) {
                length[j] = best_length + 1;
                nodes.push_back(Node(b[j], best_node));
                node_at[j] = (int)nodes.size() - 1;
            }
            if (a[i] > b[j] && length[j] > best_length) {
                best_length = length[j];
                best_node = node_at[j];
            }
        }
    }

    int answer_length = 0;
    int answer_node = -1;
    for (j = 0; j < m; ++j) {
        if (length[j] >= answer_length) {
            answer_length = length[j];
            answer_node = node_at[j];
        }
    }

    std::vector<int> answer;
    while (answer_node != -1) {
        answer.push_back(nodes[answer_node].value);
        answer_node = nodes[answer_node].parent;
    }

    std::printf("%d\n", answer_length);
    for (i = (int)answer.size() - 1; i >= 0; --i) {
        if (i != (int)answer.size() - 1) std::printf(" ");
        std::printf("%d", answer[i]);
    }
    std::printf("\n");
    return 0;
}
