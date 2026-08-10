// POJ 2047 - Concert Hall Scheduling
// Model: gpt-5.6-terra
// Approach: Send two units of minimum-cost flow through a time-expanded graph.
// Idle edges carry both rooms to the next day; an application is a unit-capacity
// jump from its first day to the day after its last, with cost minus its price.

#include <iostream>
#include <queue>
#include <vector>

struct Edge {
    int to;
    int reverse;
    int capacity;
    int cost;

    Edge(int destination, int reverse_index, int edge_capacity, int edge_cost)
        : to(destination), reverse(reverse_index), capacity(edge_capacity),
          cost(edge_cost) {}
};

typedef std::vector< std::vector<Edge> > Graph;

void add_edge(Graph &graph, int from, int to, int capacity, int cost) {
    const int forward_reverse = static_cast<int>(graph[to].size());
    const int backward_reverse = static_cast<int>(graph[from].size());
    graph[from].push_back(Edge(to, forward_reverse, capacity, cost));
    graph[to].push_back(Edge(from, backward_reverse, 0, -cost));
}

int maximum_income(Graph &graph, int source, int sink) {
    const int infinity = 2000000000;
    const int node_count = static_cast<int>(graph.size());
    int total_cost = 0;

    for (int flow = 0; flow < 2; ++flow) {
        std::vector<int> distance(node_count, infinity);
        std::vector<int> previous_node(node_count, -1);
        std::vector<int> previous_edge(node_count, -1);
        std::vector<bool> in_queue(node_count, false);
        std::queue<int> pending;

        distance[source] = 0;
        pending.push(source);
        in_queue[source] = true;

        while (!pending.empty()) {
            const int here = pending.front();
            pending.pop();
            in_queue[here] = false;

            for (std::size_t i = 0; i < graph[here].size(); ++i) {
                const Edge &edge = graph[here][i];
                if (edge.capacity > 0
                    && distance[edge.to] > distance[here] + edge.cost) {
                    distance[edge.to] = distance[here] + edge.cost;
                    previous_node[edge.to] = here;
                    previous_edge[edge.to] = static_cast<int>(i);
                    if (!in_queue[edge.to]) {
                        pending.push(edge.to);
                        in_queue[edge.to] = true;
                    }
                }
            }
        }

        total_cost += distance[sink];
        for (int node = sink; node != source; node = previous_node[node]) {
            Edge &edge = graph[previous_node[node]][previous_edge[node]];
            --edge.capacity;
            ++graph[node][edge.reverse].capacity;
        }
    }

    return -total_cost;
}

int main() {
    std::ios::sync_with_stdio(false);

    int application_count;
    while (std::cin >> application_count && application_count != 0) {
        const int first_day = 1;
        const int after_last_day = 366;
        Graph graph(after_last_day + 1);

        for (int day = first_day; day < after_last_day; ++day) {
            add_edge(graph, day, day + 1, 2, 0);
        }

        for (int i = 0; i < application_count; ++i) {
            int first;
            int last;
            int price;
            std::cin >> first >> last >> price;
            add_edge(graph, first, last + 1, 1, -price);
        }

        std::cout << maximum_income(graph, first_day, after_last_day) << '\n';
    }
    return 0;
}
