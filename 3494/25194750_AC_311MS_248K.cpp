// POJ 3494 - Largest Submatrix of All 1's
// Model: gpt-5.6-terra
// Approach: Treat every row as the base of a histogram whose heights are the
// consecutive 1's ending at that row. A monotone increasing stack finds the
// largest rectangle in each histogram in linear time, for O(mn) overall.

#include <iostream>
#include <vector>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);

    int m, n;
    while (cin >> m >> n) {
        vector<int> height(n + 1, 0);
        int answer = 0;

        for (int row = 0; row < m; ++row) {
            for (int col = 0; col < n; ++col) {
                int value;
                cin >> value;
                if (value == 1) {
                    ++height[col];
                } else {
                    height[col] = 0;
                }
            }

            height[n] = 0;
            vector<int> stack;
            for (int col = 0; col <= n; ++col) {
                while (!stack.empty() && height[stack.back()] > height[col]) {
                    int top = stack.back();
                    stack.pop_back();
                    int left = stack.empty() ? -1 : stack.back();
                    int area = height[top] * (col - left - 1);
                    if (area > answer) {
                        answer = area;
                    }
                }
                stack.push_back(col);
            }
        }

        cout << answer << '\n';
    }
    return 0;
}
