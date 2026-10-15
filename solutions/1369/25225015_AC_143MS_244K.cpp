// POJ 1369 - Append
// Model: gpt-6.1-sol
// A split survives iff every later copy starts at or after its boundary.
// Keep surviving recent boundaries in order; old boundaries cannot be
// invalidated because all backward distances are less than 1000.
// Read whole lines: a literal may itself be a space. EOF ends the last block.
#include <iostream>
#include <string>
#include <sstream>
#include <deque>
using namespace std;
int main() {
    string line;
    deque<long long> cuts;
    long long length = 0, permanent = 0;
    bool active = false;
    while (true) {
        bool have = static_cast<bool>(getline(cin, line));
        if (have && !line.empty() && line[line.size()-1] == '\r')
            line.erase(line.size()-1);
        if (!have || line.empty()) {
            if (active) cout << permanent + cuts.size() - 1 << '\n';
            cuts.clear(); length = permanent = 0; active = false;
            if (!have) break;
            continue;
        }
        istringstream in(line);
        int p, r = 1;
        if (!(in >> p)) continue;
        active = true;
        if (p) {
            in >> r;
            while (!cuts.empty() && cuts.back() > length - p)
                cuts.pop_back();
        }
        length += r;
        cuts.push_back(length);
        while (!cuts.empty() && length - cuts.front() >= 999) {
            ++permanent;
            cuts.pop_front();
        }
    }
    return 0;
}
