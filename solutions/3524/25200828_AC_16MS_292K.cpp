// POJ 3524 - Bug Hunt
// Model: gpt-5.6-terra
// Interpret every assignment with sparse maps of initialized elements.  Recursive
// expression evaluation validates subscripts and rejects reads of undefined cells.
// Line numbers include declarations, as demonstrated by the fourth sample program.
#include <iostream>
#include <string>
#include <map>
#include <cctype>
using namespace std;

struct Interpreter {
    int length[128];
    map<int, int> value[128];
    string text;
    int pos;
    bool good;

    Interpreter() {
        int i;
        for (i = 0; i < 128; ++i) length[i] = -1;
    }

    int expression() {
        if (!good) return 0;
        if (isdigit(text[pos])) {
            int result = 0;
            while (pos < (int)text.size() && isdigit(text[pos])) {
                result = result * 10 + text[pos] - '0';
                ++pos;
            }
            return result;
        }
        unsigned char name = text[pos++];
        ++pos; // '['
        int index = expression();
        ++pos; // ']'
        if (!good) return 0;
        if (index < 0 || index >= length[name]) {
            good = false;
            return 0;
        }
        map<int, int>::iterator it = value[name].find(index);
        if (it == value[name].end()) {
            good = false;
            return 0;
        }
        return it->second;
    }

    bool execute(const string &line) {
        text = line;
        pos = 0;
        good = true;
        unsigned char name = text[pos++];
        ++pos; // '['
        int index = expression();
        ++pos; // ']'
        if (!good || index < 0 || index >= length[name]) {
            good = false;
            return false;
        }
        ++pos; // '='
        int rhs = expression();
        if (!good) return false;
        value[name][index] = rhs;
        return true;
    }

    void declareArray(const string &line) {
        unsigned char name = line[0];
        int n = 0;
        int i;
        for (i = 2; line[i] != ']'; ++i) n = n * 10 + line[i] - '0';
        length[name] = n;
        value[name].clear();
    }
};

int main() {
    string line;
    while (getline(cin, line)) {
        if (line == ".") break;
        Interpreter state;
        int lineNumber = 0;
        int answer = 0;
        while (line != ".") {
            ++lineNumber;
            if (line.find('=') == string::npos) {
                state.declareArray(line);
            } else if (answer == 0 && !state.execute(line)) {
                answer = lineNumber;
            }
            if (!getline(cin, line)) return 0;
        }
        cout << answer << '\n';
    }
    return 0;
}
