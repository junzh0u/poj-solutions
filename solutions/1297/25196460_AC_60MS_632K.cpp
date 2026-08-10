// POJ 1297 - Supermarket
// Model: gpt-5.6-terra
// Approach: Stream the aisle while maintaining the minimum cost of every
// requested-list prefix.  For each product, update only equal requested items
// in descending list order, so one shelf occurrence cannot be bought twice.

#include <algorithm>
#include <iomanip>
#include <iostream>

const int MAX_PRODUCT_ID = 100000;
const int MAX_LIST_SIZE = 100;

int main() {
    std::ios::sync_with_stdio(false);

    int list_size;
    int product_count;
    while (std::cin >> list_size >> product_count
           && (list_size != 0 || product_count != 0)) {
        int first_position[MAX_PRODUCT_ID + 1];
        int previous_equal[MAX_LIST_SIZE];
        std::fill(first_position, first_position + MAX_PRODUCT_ID + 1, -1);

        for (int position = 0; position < list_size; ++position) {
            int product;
            std::cin >> product;
            previous_equal[position] = first_position[product];
            first_position[product] = position;
        }

        const double infinity = 1e100;
        double minimum_cost[MAX_LIST_SIZE + 1];
        std::fill(minimum_cost, minimum_cost + list_size + 1, infinity);
        minimum_cost[0] = 0.0;

        for (int i = 0; i < product_count; ++i) {
            int product;
            double price;
            std::cin >> product >> price;

            for (int position = first_position[product]; position != -1;
                 position = previous_equal[position]) {
                if (minimum_cost[position] != infinity) {
                    const double candidate = minimum_cost[position] + price;
                    if (candidate < minimum_cost[position + 1]) {
                        minimum_cost[position + 1] = candidate;
                    }
                }
            }
        }

        if (minimum_cost[list_size] == infinity) {
            std::cout << "Impossible\n";
        } else {
            std::cout << std::fixed << std::setprecision(2)
                      << minimum_cost[list_size] << '\n';
        }
    }
    return 0;
}
