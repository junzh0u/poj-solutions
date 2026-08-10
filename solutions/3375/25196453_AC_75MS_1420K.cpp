// POJ 3375 - Network Connection
// Model: gpt-5.6-terra
// Sort both sets and use the noncrossing matching DP. For each computer, only
// interfaces within N sorted positions of its coordinate can be optimal: any
// farther choice has an unused strictly closer interface available. Prefix
// minima then evaluate all relevant transitions in O(N^2) time.

#include <algorithm>
#include <cstdio>

using namespace std;

const int MAX_M = 100000;
const int INFINITY_COST = 2000000001;

int interface_position[MAX_M];
int computer_position[2000];
int first_cost[MAX_M];
int second_cost[MAX_M];

int distance_between(int first, int second) {
    return first >= second ? first - second : second - first;
}

int main() {
    int interface_count;
    int computer_count;
    if (scanf("%d%d", &interface_count, &computer_count) != 2) {
        return 0;
    }
    for (int i = 0; i < interface_count; ++i) {
        scanf("%d", &interface_position[i]);
    }
    for (int i = 0; i < computer_count; ++i) {
        scanf("%d", &computer_position[i]);
    }

    sort(interface_position, interface_position + interface_count);
    sort(computer_position, computer_position + computer_count);

    int *previous = first_cost;
    int *current = second_cost;
    int previous_left = 0;
    int previous_right = -1;

    for (int computer = 0; computer < computer_count; ++computer) {
        const int lower = static_cast<int>(
            lower_bound(interface_position,
                        interface_position + interface_count,
                        computer_position[computer]) - interface_position);

        int left = max(0, lower - computer_count);
        int right = min(interface_count - 1, lower + computer_count);
        left = max(left, computer);
        right = min(right, interface_count - computer_count + computer);

        if (computer == 0) {
            for (int interface_index = left;
                 interface_index <= right; ++interface_index) {
                previous[interface_index] =
                    distance_between(computer_position[computer],
                                     interface_position[interface_index]);
            }
        } else {
            int best_prefix = INFINITY_COST;
            int old_index = previous_left;
            for (int interface_index = left;
                 interface_index <= right; ++interface_index) {
                while (old_index <= previous_right &&
                       old_index < interface_index) {
                    best_prefix = min(best_prefix, previous[old_index]);
                    ++old_index;
                }
                if (best_prefix == INFINITY_COST) {
                    current[interface_index] = INFINITY_COST;
                } else {
                    current[interface_index] =
                        best_prefix +
                        distance_between(computer_position[computer],
                                         interface_position[interface_index]);
                }
            }
            int *temporary = previous;
            previous = current;
            current = temporary;
        }

        previous_left = left;
        previous_right = right;
    }

    int answer = INFINITY_COST;
    for (int interface_index = previous_left;
         interface_index <= previous_right; ++interface_index) {
        answer = min(answer, previous[interface_index]);
    }
    printf("%d\n", answer);
    return 0;
}
