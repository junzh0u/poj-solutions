// POJ 1728 - A flea on a chessboard
// Model: gpt-5.6-terra
// Approach: A landing's boundary and color status depends only on both
// coordinates modulo 2S.  The joint state therefore repeats within 2S jumps;
// test that complete period and report failure if no white interior is reached.
// Use 64-bit coordinates because x, y, dx, and dy have no stated upper bound.

#include <iostream>

__extension__ typedef long long Integer;

bool is_white_interior(Integer x, Integer y, Integer side) {
    if (x % side == 0 || y % side == 0) {
        return false;
    }
    return (x / side + y / side) % 2 == 1;
}

int main() {
    std::ios::sync_with_stdio(false);

    Integer side;
    Integer x;
    Integer y;
    Integer dx;
    Integer dy;
    while (std::cin >> side >> x >> y >> dx >> dy) {
        if (side == 0 && x == 0 && y == 0 && dx == 0 && dy == 0) {
            break;
        }

        const Integer period = 2 * side;
        Integer state_x = x % period;
        Integer state_y = y % period;
        const Integer step_x = dx % period;
        const Integer step_y = dy % period;
        bool escaped = false;

        for (Integer jumps = 0; jumps < period; ++jumps) {
            if (is_white_interior(state_x, state_y, side)) {
                std::cout << "After " << jumps
                          << " jumps the flea lands at ("
                          << x + jumps * dx << ", "
                          << y + jumps * dy << ").\n";
                escaped = true;
                break;
            }
            state_x = (state_x + step_x) % period;
            state_y = (state_y + step_y) % period;
        }

        if (!escaped) {
            std::cout << "The flea cannot escape from black squares.\n";
        }
    }
    return 0;
}
