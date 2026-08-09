// POJ 2564 - Edit Step Ladders
// Model: gpt-5.6-terra
// Approach: Dynamic programming over lexicographic input order, with a hash
// table for predecessors formed by one deletion, smaller replacement, or
// lexicographically smaller insertion.
#include <iostream>
#include <cstring>
using namespace std;

const int MAX_WORDS = 25005;
const int MAX_LENGTH = 128;
const int TABLE_SIZE = 131071;
char words[MAX_WORDS][MAX_LENGTH];
int best[MAX_WORDS], table[TABLE_SIZE];

unsigned int hashWord(const char *s) {
    unsigned int h = 2166136261u;
    while (*s) {
        h ^= (unsigned char)*s++;
        h *= 16777619u;
    }
    return h;
}

int findWord(const char *s) {
    unsigned int p = hashWord(s) % TABLE_SIZE;
    while (table[p] != -1) {
        if (strcmp(words[table[p]], s) == 0) return table[p];
        if (++p == TABLE_SIZE) p = 0;
    }
    return -1;
}

void insertWord(int id) {
    unsigned int p = hashWord(words[id]) % TABLE_SIZE;
    while (table[p] != -1) {
        if (++p == TABLE_SIZE) p = 0;
    }
    table[p] = id;
}

int main() {
    char current[MAX_LENGTH], candidate[MAX_LENGTH];
    int count = 0, answer = 0, length, i, c, id;
    memset(table, -1, sizeof(table));
    while (cin >> current) {
        length = strlen(current);
        best[count] = 1;
        for (i = 0; i < length; ++i) {
            memcpy(candidate, current, i);
            strcpy(candidate + i, current + i + 1);
            id = findWord(candidate);
            if (id != -1 && best[id] + 1 > best[count]) best[count] = best[id] + 1;
        }
        strcpy(candidate, current);
        for (i = 0; i < length; ++i) {
            char original = candidate[i];
            for (c = 'a'; c < original; ++c) {
                candidate[i] = c;
                id = findWord(candidate);
                if (id != -1 && best[id] + 1 > best[count]) best[count] = best[id] + 1;
            }
            candidate[i] = original;
        }
        for (i = 0; i < length; ++i)
            for (c = 'a'; c < current[i]; ++c) {
                memcpy(candidate, current, i);
                candidate[i] = c;
                strcpy(candidate + i + 1, current + i);
                id = findWord(candidate);
                if (id != -1 && best[id] + 1 > best[count]) best[count] = best[id] + 1;
            }
        strcpy(words[count], current);
        insertWord(count);
        if (best[count] > answer) answer = best[count];
        ++count;
    }
    cout << answer << '\n';
    return 0;
}
