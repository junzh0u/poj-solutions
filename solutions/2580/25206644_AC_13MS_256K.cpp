// POJ 2580 - The Door/Key Problem
// Model: claude-sonnet-5
//
// Fixed-point reachability over an undirected multigraph. Each door (edge)
// between two rooms requires a set of keys (letters A-Z) to pass in either
// direction; each room contains a fixed set of keys that are picked up the
// moment the room is first visited. Starting from room M with no keys,
// repeatedly expand the visited-room set through any door whose required
// key set is already a subset of the collected keys, adding the newly
// visited room's keys each time, until a fixpoint. Room 0 reachable -> YES.
//
// Input framing: blank lines are legal data (a room with no doors, or no
// keys) and are NOT separators between data sets, so the room list and key
// list must be read as exactly N raw lines each via getline, never with
// whitespace-skipping token extraction (which would silently merge an empty
// room's line with the next one). M == 0 is a valid, trivial case (answer
// is immediately YES since room 0 is the start).
//
// Ambiguity check: the sample's third data set contains a chain of keys
// each unlocking the next room up to room 9, which holds key X -- but the
// door back to room 0 needs key I, which is never obtainable. This
// confirms the reachability must be a genuine keys-subset-of-collected
// check per door, not e.g. "any one key opens any door" or "one key per
// room automatically opens the corresponding numbered door".

#include <cstdio>
#include <cctype>
#include <string>
#include <sstream>
#include <vector>
#include <iostream>

using namespace std;

static string rstrip(const string &s) {
    size_t end = s.size();
    while (end > 0 && (s[end - 1] == '\r' || s[end - 1] == '\n' ||
                        s[end - 1] == ' ' || s[end - 1] == '\t')) {
        end--;
    }
    return s.substr(0, end);
}

int main() {
    vector<string> lines;
    string line;
    while (getline(cin, line)) {
        lines.push_back(line);
    }

    size_t idx = 0;
    string out;

    while (idx < lines.size()) {
        string first = rstrip(lines[idx]);
        if (first.empty()) { idx++; continue; }
        if (first == "ENDOFINPUT") break;

        istringstream iss(first);
        string tag;
        int M, N;
        iss >> tag >> M >> N;
        idx++;

        vector<string> roomLines(N), keyLines(N);
        for (int i = 0; i < N; i++) {
            roomLines[i] = (idx < lines.size()) ? lines[idx++] : string();
        }
        for (int i = 0; i < N; i++) {
            keyLines[i] = (idx < lines.size()) ? lines[idx++] : string();
        }
        // Skip the END line.
        if (idx < lines.size()) idx++;

        // Parse edges: (a, b, requiredKeyMask)
        vector<int> ea, eb;
        vector<unsigned int> emask;
        for (int a = 0; a < N; a++) {
            istringstream ls(roomLines[a]);
            string tok;
            while (ls >> tok) {
                size_t p = 0;
                int b = 0;
                bool any = false;
                while (p < tok.size() && isdigit((unsigned char)tok[p])) {
                    b = b * 10 + (tok[p] - '0');
                    p++;
                    any = true;
                }
                if (!any) continue; // malformed guard, shouldn't happen
                unsigned int mask = 0;
                for (; p < tok.size(); p++) {
                    if (tok[p] >= 'A' && tok[p] <= 'Z') {
                        mask |= (1u << (tok[p] - 'A'));
                    }
                }
                ea.push_back(a);
                eb.push_back(b);
                emask.push_back(mask);
            }
        }

        // Parse keys per room.
        vector<unsigned int> roomKeys(N, 0);
        for (int r = 0; r < N; r++) {
            istringstream ls(keyLines[r]);
            string tok;
            while (ls >> tok) {
                for (size_t p = 0; p < tok.size(); p++) {
                    if (tok[p] >= 'A' && tok[p] <= 'Z') {
                        roomKeys[r] |= (1u << (tok[p] - 'A'));
                    }
                }
            }
        }

        vector<bool> visited(N, false);
        unsigned int keys = 0;
        if (M >= 0 && M < N) {
            visited[M] = true;
            keys |= roomKeys[M];
        }

        bool changed = true;
        while (changed) {
            changed = false;
            for (size_t e = 0; e < ea.size(); e++) {
                if ((emask[e] & ~keys) != 0) continue; // missing a required key
                int a = ea[e], b = eb[e];
                if (a < 0 || a >= N || b < 0 || b >= N) continue;
                if (visited[a] && !visited[b]) {
                    visited[b] = true;
                    keys |= roomKeys[b];
                    changed = true;
                }
                if (visited[b] && !visited[a]) {
                    visited[a] = true;
                    keys |= roomKeys[a];
                    changed = true;
                }
            }
        }

        out += (N > 0 && visited[0]) ? "YES\n" : "NO\n";
    }

    fputs(out.c_str(), stdout);
    return 0;
}
