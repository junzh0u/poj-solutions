// POJ 2082 - Terrible Sets
// Model: gpt-5.6-terra
// Treat every input pair as one consecutive histogram bar of integer width.
// A monotone increasing stack records each height's leftmost possible x; when
// the height drops, that bar's maximal right endpoint is known.

#include <cstdio>
#include <vector>

struct Bar {
    long long height;
    long long left;

    Bar(long long height_value, long long left_value)
        : height(height_value), left(left_value) {}
};

int main() {
    int n;
    while (std::scanf("%d", &n) == 1 && n != -1) {
        std::vector<Bar> stack;
        long long x = 0;
        long long answer = 0;

        for (int i = 0; i < n; ++i) {
            long long width, height;
            std::scanf("%lld%lld", &width, &height);
            long long left = x;

            while (!stack.empty() && stack.back().height > height) {
                long long area = stack.back().height * (x - stack.back().left);
                if (area > answer) {
                    answer = area;
                }
                left = stack.back().left;
                stack.pop_back();
            }
            if (stack.empty() || stack.back().height < height) {
                stack.push_back(Bar(height, left));
            }
            x += width;
        }

        while (!stack.empty()) {
            long long area = stack.back().height * (x - stack.back().left);
            if (area > answer) {
                answer = area;
            }
            stack.pop_back();
        }
        std::printf("%lld\n", answer);
    }
    return 0;
}
