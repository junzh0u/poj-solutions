// POJ 1642 - Stacking Cubes
// Model: gpt-5.6-terra
// Approach: Treat the stacking pattern as an order ideal of occupied cubes.
// A left rotation groups cubes by original height and row, so each new entry
// counts columns above that level.  A right rotation groups by original column
// and height, so each new entry counts rows above that level.

#include <iostream>
#include <vector>

typedef std::vector< std::vector<int> > Pattern;

Pattern rotate_left(const Pattern &source) {
    Pattern result;
    const int maximum_height = source[0][0];
    for (int level = 0; level < maximum_height; ++level) {
        std::vector<int> row;
        for (std::size_t i = 0; i < source.size(); ++i) {
            int columns = 0;
            while (columns < static_cast<int>(source[i].size())
                   && source[i][columns] > level) {
                ++columns;
            }
            if (columns == 0) {
                break;
            }
            row.push_back(columns);
        }
        result.push_back(row);
    }
    return result;
}

Pattern rotate_right(const Pattern &source) {
    Pattern result;
    const int maximum_width = static_cast<int>(source[0].size());
    for (int column = 0; column < maximum_width; ++column) {
        std::vector<int> row;
        for (int level = 0; level < source[0][column]; ++level) {
            int rows = 0;
            while (rows < static_cast<int>(source.size())
                   && column < static_cast<int>(source[rows].size())
                   && source[rows][column] > level) {
                ++rows;
            }
            row.push_back(rows);
        }
        result.push_back(row);
    }
    return result;
}

void print_pattern(const Pattern &pattern) {
    for (std::size_t i = 0; i < pattern.size(); ++i) {
        for (std::size_t j = 0; j < pattern[i].size(); ++j) {
            if (j != 0) {
                std::cout << ' ';
            }
            std::cout << pattern[i][j];
        }
        std::cout << '\n';
    }
}

int main() {
    std::ios::sync_with_stdio(false);

    bool first_instance = true;
    int row_count;
    while (std::cin >> row_count && row_count != 0) {
        Pattern source(row_count);
        for (int row = 0; row < row_count; ++row) {
            int height;
            while (std::cin >> height && height != 0) {
                source[row].push_back(height);
            }
        }

        if (!first_instance) {
            std::cout << "\n\n";
        }
        first_instance = false;
        print_pattern(rotate_left(source));
        std::cout << '\n';
        print_pattern(rotate_right(source));
    }
    return 0;
}
