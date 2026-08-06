// POJ 3249 - Test for Job
// Model: gpt-5.6-terra
// Process the DAG in topological order. dp[v] is the best profit of a route
// from any indegree-zero city to v; maximize it over every outdegree-zero city.
// Values may be negative and an isolated city is both a source and a target.
#include <cstdio>
#include <vector>

using namespace std;

class FastInput {
public:
    FastInput() : position(0), length(0) {}

    bool read_int(int &value) {
        int c = next_char();
        while (c != EOF && c <= ' ') c = next_char();
        if (c == EOF) return false;
        bool negative = false;
        if (c == '-') {
            negative = true;
            c = next_char();
        }
        value = 0;
        while (c >= '0' && c <= '9') {
            value = value * 10 + c - '0';
            c = next_char();
        }
        if (negative) value = -value;
        return true;
    }

private:
    char buffer[1 << 16];
    int position;
    int length;

    int next_char() {
        if (position == length) {
            length = (int)fread(buffer, 1, sizeof(buffer), stdin);
            position = 0;
            if (length == 0) return EOF;
        }
        return buffer[position++];
    }
};

int main() {
    FastInput input;
    int cities;
    while (input.read_int(cities)) {
        int roads;
        input.read_int(roads);
        vector<int> value(cities);
        vector<int> indegree(cities, 0);
        vector<int> head(cities, -1);
        vector<int> to(roads);
        vector<int> next(roads);
        int i;
        for (i = 0; i < cities; ++i) input.read_int(value[i]);
        for (i = 0; i < roads; ++i) {
            int from, destination;
            input.read_int(from);
            input.read_int(destination);
            --from;
            --destination;
            to[i] = destination;
            next[i] = head[from];
            head[from] = i;
            ++indegree[destination];
        }

        // A route has at most 100000 cities and each value is within 20000,
        // so its total stays inside signed int (at most 2,000,000,000).
        const int NEG_INF = -2147483647;
        vector<int> best(cities, NEG_INF);
        vector<int> queue(cities);
        int front = 0;
        int back = 0;
        for (i = 0; i < cities; ++i) {
            if (indegree[i] == 0) {
                best[i] = value[i];
                queue[back++] = i;
            }
        }

        int answer = NEG_INF;
        while (front < back) {
            int city = queue[front++];
            if (head[city] == -1 && best[city] > answer) answer = best[city];
            int edge;
            for (edge = head[city]; edge != -1; edge = next[edge]) {
                int destination = to[edge];
                int candidate = best[city] + value[destination];
                if (candidate > best[destination]) best[destination] = candidate;
                --indegree[destination];
                if (indegree[destination] == 0) queue[back++] = destination;
            }
        }
        printf("%d\n", answer);
    }
    return 0;
}
