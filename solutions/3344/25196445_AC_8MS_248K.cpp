// POJ 3344 - Chessboard Dance
// Model: gpt-5.6-terra
// Approach: Keep Betty outside the board array and execute each move one square
// at a time.  When the next square is occupied, shift the contiguous run of
// pieces forward by one square, discarding its last piece if it crosses an edge.

#include <iostream>
#include <sstream>
#include <string>

using namespace std;

const int BOARD_SIZE = 8;

char board[BOARD_SIZE][BOARD_SIZE];
int bettyRow;
int bettyColumn;
int direction;

const int rowChange[4] = {-1, 0, 1, 0};
const int columnChange[4] = {0, 1, 0, -1};
const char directionSymbol[4] = {'^', '>', 'v', '<'};

bool inside(int row, int column) {
    return 0 <= row && row < BOARD_SIZE &&
           0 <= column && column < BOARD_SIZE;
}

int symbolDirection(char symbol) {
    for (int index = 0; index < 4; ++index) {
        if (directionSymbol[index] == symbol) {
            return index;
        }
    }
    return -1;
}

void pushPieces(int firstRow, int firstColumn) {
    int row = firstRow;
    int column = firstColumn;
    while (inside(row, column) && board[row][column] != '.') {
        row += rowChange[direction];
        column += columnChange[direction];
    }

    if (!inside(row, column)) {
        row -= rowChange[direction];
        column -= columnChange[direction];
    }

    while (row != firstRow || column != firstColumn) {
        int previousRow = row - rowChange[direction];
        int previousColumn = column - columnChange[direction];
        board[row][column] = board[previousRow][previousColumn];
        row = previousRow;
        column = previousColumn;
    }
    board[firstRow][firstColumn] = '.';
}

void moveForward(int steps) {
    for (int step = 0; step < steps; ++step) {
        int nextRow = bettyRow + rowChange[direction];
        int nextColumn = bettyColumn + columnChange[direction];
        if (!inside(nextRow, nextColumn)) {
            break;
        }
        if (board[nextRow][nextColumn] != '.') {
            pushPieces(nextRow, nextColumn);
        }
        bettyRow = nextRow;
        bettyColumn = nextColumn;
    }
}

void removeCarriageReturn(string &line) {
    if (!line.empty() && line[line.size() - 1] == '\r') {
        line.erase(line.size() - 1);
    }
}

int main() {
    string line;
    while (getline(cin, line)) {
        removeCarriageReturn(line);
        if (line == "--") {
            break;
        }

        string rows[BOARD_SIZE];
        rows[0] = line;
        for (int row = 1; row < BOARD_SIZE; ++row) {
            getline(cin, rows[row]);
            removeCarriageReturn(rows[row]);
        }

        for (int row = 0; row < BOARD_SIZE; ++row) {
            for (int column = 0; column < BOARD_SIZE; ++column) {
                board[row][column] = rows[row][column];
                int initialDirection = symbolDirection(board[row][column]);
                if (initialDirection != -1) {
                    bettyRow = row;
                    bettyColumn = column;
                    direction = initialDirection;
                    board[row][column] = '.';
                }
            }
        }

        while (getline(cin, line)) {
            removeCarriageReturn(line);
            if (line == "#") {
                break;
            }
            if (line.compare(0, 5, "move ") == 0) {
                int steps;
                istringstream input(line.substr(5));
                input >> steps;
                moveForward(steps);
            } else if (line == "turn right") {
                direction = (direction + 1) % 4;
            } else if (line == "turn left") {
                direction = (direction + 3) % 4;
            } else if (line == "turn back") {
                direction = (direction + 2) % 4;
            }
        }

        board[bettyRow][bettyColumn] = directionSymbol[direction];
        for (int row = 0; row < BOARD_SIZE; ++row) {
            for (int column = 0; column < BOARD_SIZE; ++column) {
                cout << board[row][column];
            }
            cout << '\n';
        }
        cout << '\n';
    }
    return 0;
}
