// POJ 1231 - The Alphabet Game
// Model: gpt-5.6-terra
// Approach: reduce each letter to its bounding box. A usable grid line crosses
// no box, so draw every usable vertical and horizontal line; the division works
// exactly when no two boxes occupy one resulting cell.
#include <cstdio>

struct Box {
    int min_x, max_x, min_y, max_y;
};

int main() {
    int tests;
    if (std::scanf("%d", &tests) != 1) return 0;
    while (tests--) {
        int k, p;
        std::scanf("%d%d", &k, &p);
        Box boxes[26];
        for (int i = 0; i < k; ++i) {
            boxes[i].min_x = boxes[i].min_y = 1000001;
            boxes[i].max_x = boxes[i].max_y = 0;
            for (int j = 0; j < p; ++j) {
                int x, y;
                std::scanf("%d%d", &x, &y);
                if (x < boxes[i].min_x) boxes[i].min_x = x;
                if (x > boxes[i].max_x) boxes[i].max_x = x;
                if (y < boxes[i].min_y) boxes[i].min_y = y;
                if (y > boxes[i].max_y) boxes[i].max_y = y;
            }
        }

        int cuts_x[26], cuts_y[26], count_x = 0, count_y = 0;
        for (int i = 0; i < k; ++i) {
            bool safe_x = true, safe_y = true;
            for (int j = 0; j < k; ++j) {
                if (boxes[j].min_x <= boxes[i].max_x && boxes[i].max_x < boxes[j].max_x) safe_x = false;
                if (boxes[j].min_y <= boxes[i].max_y && boxes[i].max_y < boxes[j].max_y) safe_y = false;
            }
            if (safe_x) cuts_x[count_x++] = boxes[i].max_x;
            if (safe_y) cuts_y[count_y++] = boxes[i].max_y;
        }

        int cell_x[26], cell_y[26];
        for (int i = 0; i < k; ++i) {
            cell_x[i] = cell_y[i] = 0;
            for (int j = 0; j < count_x; ++j) if (cuts_x[j] < boxes[i].min_x) ++cell_x[i];
            for (int j = 0; j < count_y; ++j) if (cuts_y[j] < boxes[i].min_y) ++cell_y[i];
        }

        bool possible = true;
        for (int i = 0; i < k; ++i)
            for (int j = 0; j < i; ++j)
                if (cell_x[i] == cell_x[j] && cell_y[i] == cell_y[j]) possible = false;
        std::puts(possible ? "YES" : "NO");
    }
    return 0;
}
