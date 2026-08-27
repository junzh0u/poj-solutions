// POJ 1955 - Rubik's Cube
// Model: gpt-5.6-terra
// Apply each turn as the face-and-adjacent-ring permutation in the input net.
// Clockwise is defined while looking directly at the selected face.

#include <iostream>
#include <cstring>

using namespace std;

static const int clockwise_to[6][20] = {
    {12, 24, 36, 35, 34, 22, 10, 11, 52, 49, 46, 37, 25, 13, 7, 4, 1, 21, 33, 45},
    {15, 14, 27, 39, 38, 37, 25, 13, 46, 47, 48, 40, 28, 16, 9, 8, 7, 12, 24, 36},
    {18, 30, 42, 41, 40, 28, 16, 17, 48, 51, 54, 43, 31, 19, 3, 6, 9, 15, 27, 39},
    {19, 20, 21, 33, 45, 44, 43, 31, 54, 53, 52, 34, 22, 10, 1, 2, 3, 18, 30, 42},
    {1, 2, 3, 6, 9, 8, 7, 4, 13, 14, 15, 16, 17, 18, 19, 20, 21, 10, 11, 12},
    {46, 47, 48, 51, 54, 53, 52, 49, 45, 44, 43, 42, 41, 40, 39, 38, 37, 36, 35, 34}
};

static const int clockwise_from[6][20] = {
    {10, 11, 12, 24, 36, 35, 34, 22, 37, 25, 13, 7, 4, 1, 21, 33, 45, 52, 49, 46},
    {13, 25, 14, 15, 27, 39, 38, 37, 40, 28, 16, 9, 8, 7, 12, 24, 36, 46, 47, 48},
    {16, 17, 18, 30, 42, 41, 40, 28, 43, 31, 19, 3, 6, 9, 15, 27, 39, 48, 51, 54},
    {43, 31, 19, 20, 21, 33, 45, 44, 34, 22, 10, 1, 2, 3, 18, 30, 42, 54, 53, 52},
    {7, 4, 1, 2, 3, 6, 9, 8, 16, 17, 18, 19, 20, 21, 10, 11, 12, 13, 14, 15},
    {52, 49, 46, 47, 48, 51, 54, 53, 42, 41, 40, 39, 38, 37, 36, 35, 34, 45, 44, 43}
};

static void turn(char cube[], int face, int direction) {
    char old[55];
    int i;
    memcpy(old, cube, sizeof(old));
    if (direction == 1) {
        for (i = 0; i < 20; ++i)
            cube[clockwise_to[face][i]] = old[clockwise_from[face][i]];
    } else {
        for (i = 0; i < 20; ++i)
            cube[clockwise_from[face][i]] = old[clockwise_to[face][i]];
    }
}

static void print_cube(const char cube[]) {
    int i;
    for (i = 1; i <= 9; ++i) {
        if ((i - 1) % 3 == 0) cout << "     ";
        cout << ' ' << cube[i];
        if (i % 3 == 0) cout << '\n';
    }
    for (i = 10; i <= 45; ++i) {
        if (i != 10 && i != 22 && i != 34) cout << ' ';
        cout << cube[i];
        if (i == 21 || i == 33 || i == 45) cout << '\n';
    }
    for (i = 46; i <= 54; ++i) {
        if ((i - 1) % 3 == 0) cout << "     ";
        cout << ' ' << cube[i];
        if (i % 3 == 0) cout << '\n';
    }
}

int main() {
    int scenarios, scenario, i, turns, face, direction;
    char cube[55];
    cin >> scenarios;
    for (scenario = 1; scenario <= scenarios; ++scenario) {
        for (i = 1; i <= 54; ++i) cin >> cube[i];
        cin >> turns;
        while (turns--) {
            cin >> face >> direction;
            turn(cube, face, direction);
        }
        cout << "Scenario #" << scenario << ":\n";
        print_cube(cube);
        cout << '\n';
    }
    return 0;
}
