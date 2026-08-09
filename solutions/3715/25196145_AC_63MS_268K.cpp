// POJ 3715 - Blue and Red
// Model: gpt-5.6-terra
// Approach: cross-group friendships form a bipartite graph, so Konig's theorem
// equates minimum vertex-cover size with maximum matching. Test soldiers in
// order; keep a soldier in the cover exactly when deleting it lowers the
// residual maximum matching, maintaining that matching decrementally.
#include <iostream>
#include <vector>

using namespace std;

int n;
int group[200];
bool edge[200][200];
bool alive[200];
int match_left[200], match_right[200];
bool seen_left[200], seen_right[200];

bool augment_left(int left) {
    if (seen_left[left]) return false;
    seen_left[left] = true;
    for (int right = 0; right < n; ++right) {
        if (!alive[right] || !edge[left][right] || seen_right[right]) continue;
        seen_right[right] = true;
        if (match_right[right] == -1 || augment_left(match_right[right])) {
            match_left[left] = right;
            match_right[right] = left;
            return true;
        }
    }
    return false;
}

bool augment_right(int right) {
    if (seen_right[right]) return false;
    seen_right[right] = true;
    for (int left = 0; left < n; ++left) {
        if (!alive[left] || !edge[left][right] || seen_left[left]) continue;
        seen_left[left] = true;
        if (match_left[left] == -1 || augment_right(match_left[left])) {
            match_right[right] = left;
            match_left[left] = right;
            return true;
        }
    }
    return false;
}

bool delete_lowers_matching(int v) {
    if (group[v] == 0) {
        if (match_left[v] == -1) return false;
        int right = match_left[v];
        match_left[v] = match_right[right] = -1;
        alive[v] = false;
        for (int i = 0; i < n; ++i) seen_left[i] = seen_right[i] = false;
        if (!augment_right(right)) return true;
        alive[v] = true;
        return false;
    }
    if (match_right[v] == -1) return false;
    int left = match_right[v];
    match_left[left] = match_right[v] = -1;
    alive[v] = false;
    for (int i = 0; i < n; ++i) seen_left[i] = seen_right[i] = false;
    if (!augment_left(left)) return true;
    alive[v] = true;
    return false;
}

int main() {
    int tests;
    cin >> tests;
    while (tests--) {
        int m;
        cin >> n >> m;
        for (int i = 0; i < n; ++i) {
            cin >> group[i];
            alive[i] = true;
            match_left[i] = match_right[i] = -1;
            for (int j = 0; j < n; ++j) edge[i][j] = false;
        }
        for (int i = 0; i < m; ++i) {
            int x, y;
            cin >> x >> y;
            if (group[x] != group[y]) {
                if (group[x] == 0) edge[x][y] = true;
                else edge[y][x] = true;
            }
        }

        for (int left = 0; left < n; ++left) if (group[left] == 0) {
            for (int i = 0; i < n; ++i) seen_left[i] = seen_right[i] = false;
            augment_left(left);
        }

        vector<int> answer;
        for (int v = 0; v < n; ++v)
            if (delete_lowers_matching(v)) {
                answer.push_back(v);
            }

        cout << answer.size();
        for (int i = 0; i < (int)answer.size(); ++i) cout << ' ' << answer[i];
        cout << '\n';
    }
    return 0;
}
