// POJ 2337 - Catenyms
// Model: gpt-5.6-terra
// Treat each word as an edge from its first to its last letter.  A valid
// compound catenym is an Euler trail.  After checking the directed degree
// conditions and weak connectivity, Hierholzer's algorithm consumes outgoing
// words in lexical order; reversing its postorder gives the least trail.
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

int parent_[26];
int indeg[26], outdeg[26], nextEdge[26];
vector<string> graph[26];
vector<string> trail;

int findSet(int x) {
    if (parent_[x] != x) parent_[x] = findSet(parent_[x]);
    return parent_[x];
}

void joinSet(int a, int b) {
    a = findSet(a);
    b = findSet(b);
    if (a != b) parent_[a] = b;
}

void euler(int u) {
    while (nextEdge[u] < (int)graph[u].size()) {
        string word = graph[u][nextEdge[u]++];
        euler(word[word.size() - 1] - 'a');
        trail.push_back(word);
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);

    int tests;
    cin >> tests;
    while (tests--) {
        int n;
        cin >> n;
        string smallest;
        for (int i = 0; i < 26; ++i) {
            parent_[i] = i;
            indeg[i] = outdeg[i] = nextEdge[i] = 0;
            graph[i].clear();
        }

        for (int i = 0; i < n; ++i) {
            string word;
            cin >> word;
            int from = word[0] - 'a';
            int to = word[word.size() - 1] - 'a';
            graph[from].push_back(word);
            ++outdeg[from];
            ++indeg[to];
            joinSet(from, to);
            if (i == 0 || word < smallest) smallest = word;
        }

        bool possible = true;
        int start = -1, finish = -1;
        for (int i = 0; i < 26; ++i) {
            if (outdeg[i] == indeg[i] + 1) {
                if (start != -1) possible = false;
                start = i;
            } else if (indeg[i] == outdeg[i] + 1) {
                if (finish != -1) possible = false;
                finish = i;
            } else if (indeg[i] != outdeg[i]) {
                possible = false;
            }
        }
        if ((start == -1) != (finish == -1)) possible = false;
        if (start == -1) start = smallest[0] - 'a';

        int component = -1;
        for (int i = 0; i < 26; ++i) {
            if (indeg[i] + outdeg[i] == 0) continue;
            if (component == -1) component = findSet(i);
            else if (findSet(i) != component) possible = false;
        }

        for (int i = 0; i < 26; ++i)
            sort(graph[i].begin(), graph[i].end());
        trail.clear();
        if (possible) {
            euler(start);
            if ((int)trail.size() != n) possible = false;
        }

        if (!possible) {
            cout << "***\n";
        } else {
            reverse(trail.begin(), trail.end());
            for (int i = 0; i < n; ++i) {
                if (i) cout << '.';
                cout << trail[i];
            }
            cout << '\n';
        }
    }
    return 0;
}
