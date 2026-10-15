// POJ 3972 - BF
// Model: gpt-6.1-sol
// Match bracket pairs once, then interpret the eight BF commands.
// The single spaces surrounding the input stream are framing, not input.
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
using namespace std;
int main() {
    int cases;
    if (scanf("%d", &cases) != 1) return 0;
    for (int tc = 1; tc <= cases; ++tc) {
        string code, input;
        int c;
        while ((c = getchar()) != EOF && c != '$')
            if (strchr("><+-.,[]", c)) code += char(c);
        c = getchar(); // One separating space.
        while ((c = getchar()) != EOF && c != '$') input += char(c);
        if (!input.empty() && input[input.size()-1] == ' ') input.erase(input.size()-1);
        vector<int> match(code.size()), stack;
        for (int i = 0; i < (int)code.size(); ++i) {
            if (code[i] == '[') stack.push_back(i);
            if (code[i] == ']') {
                int j = stack.back(); stack.pop_back();
                match[i] = j; match[j] = i;
            }
        }
        unsigned char tape[30000];
        memset(tape, 0, sizeof(tape));
        int p = 0;
        unsigned int next = 0;
        printf("Case %d: ", tc);
        for (int i = 0; i < (int)code.size(); ++i) {
            switch (code[i]) {
                case '>': p = (p + 1) % 30000; break;
                case '<': p = (p + 29999) % 30000; break;
                case '+': ++tape[p]; break;
                case '-': --tape[p]; break;
                case '.': putchar(tape[p]); break;
                case ',': tape[p] = next < input.size() ? (unsigned char)input[next++] : 0; break;
                case '[': if (!tape[p]) i = match[i]; break;
                case ']': if (tape[p]) i = match[i]; break;
            }
        }
        putchar('\n');
    }
    return 0;
}
