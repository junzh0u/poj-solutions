// POJ 3588 - The Downloading Task
// Model: claude-sonnet-5
//
// Simulate a download manager. Tasks move between states
// downloading/waiting/paused/finished under six instruction types.
// The "highest priority waiting task" is always the alphabetically
// smallest name (ascending mode) or largest name (descending mode);
// the mode only flips which end of the same alphabetical order is
// read, so waiting tasks are kept in one std::set<string> ordered by
// name, and promotion reads begin() or rbegin() depending on the
// current mode. At most one task is promoted per Pause/Finish since
// each such event frees exactly one download slot.
//
// Ambiguity/board notes:
//  - The statement's own assumption ("Pause target is waiting or
//    downloading", "Continue target is paused", "Finish target is
//    downloading") is violated by the actual judge data per the
//    board (message 346874): defensively ignore an instruction whose
//    target is not in the required state instead of trusting the
//    assumption.
//  - The sample's raw output has a blank line only *between* test
//    cases (none after the last one), but the statement says "print
//    a blank line after each test case" and a widely-discussed
//    reference implementation prints it unconditionally after every
//    case including the last. Following the literal statement here.
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
#include <set>
#include <map>
#include <algorithm>
using namespace std;

enum State { DOWNLOADING, WAITING, PAUSED, FINISHED };

// Comparator context for sorting task indices by name; C++98 has no
// lambdas, so plain globals back the comparator functions below.
static const vector<string>* g_names = 0;

bool cmpAsc(int a, int b) { return (*g_names)[a] < (*g_names)[b]; }
bool cmpDesc(int a, int b) { return (*g_names)[a] > (*g_names)[b]; }

const char* stateName(int s) {
    switch (s) {
        case DOWNLOADING: return "downloading";
        case WAITING: return "waiting";
        case PAUSED: return "paused";
        default: return "finished";
    }
}

int main() {
    int T;
    if (scanf("%d", &T) != 1) return 0;
    char cmdBuf[32], argBuf[32];
    while (T--) {
        int n, m;
        scanf("%d %d", &n, &m);

        map<string, int> idOf;
        vector<string> names;
        vector<int> state;
        set<string> waiting; // always lexicographically ordered by name
        int downloadCount = 0;
        int mode = 0; // 0 = asc, 1 = desc

        names.reserve(m);
        state.reserve(m);

        for (int i = 0; i < m; ++i) {
            scanf("%s %s", cmdBuf, argBuf);
            string cmd(cmdBuf), arg(argBuf);

            if (cmd == "Sort") {
                mode = (arg == "desc") ? 1 : 0;
            } else if (cmd == "New") {
                if (idOf.find(arg) == idOf.end()) {
                    int tid = (int)names.size();
                    names.push_back(arg);
                    idOf[arg] = tid;
                    if (downloadCount < n) {
                        state.push_back(DOWNLOADING);
                        downloadCount++;
                    } else {
                        state.push_back(WAITING);
                        waiting.insert(arg);
                    }
                }
                // else: duplicate "New" for an existing name — ignore defensively.
            } else if (cmd == "Pause") {
                map<string, int>::iterator it = idOf.find(arg);
                if (it != idOf.end()) {
                    int tid = it->second;
                    if (state[tid] == DOWNLOADING) {
                        state[tid] = PAUSED;
                        downloadCount--;
                        if (downloadCount < n && !waiting.empty()) {
                            string promo = (mode == 0) ? *waiting.begin() : *waiting.rbegin();
                            waiting.erase(promo);
                            state[idOf[promo]] = DOWNLOADING;
                            downloadCount++;
                        }
                    } else if (state[tid] == WAITING) {
                        state[tid] = PAUSED;
                        waiting.erase(arg);
                    }
                    // else: already paused or finished — ignore defensively.
                }
            } else if (cmd == "Continue") {
                map<string, int>::iterator it = idOf.find(arg);
                if (it != idOf.end()) {
                    int tid = it->second;
                    if (state[tid] == PAUSED) {
                        if (downloadCount < n) {
                            state[tid] = DOWNLOADING;
                            downloadCount++;
                        } else {
                            state[tid] = WAITING;
                            waiting.insert(arg);
                        }
                    }
                    // else: not paused — ignore defensively.
                }
            } else if (cmd == "Finish") {
                map<string, int>::iterator it = idOf.find(arg);
                if (it != idOf.end()) {
                    int tid = it->second;
                    if (state[tid] == DOWNLOADING) {
                        state[tid] = FINISHED;
                        downloadCount--;
                        if (downloadCount < n && !waiting.empty()) {
                            string promo = (mode == 0) ? *waiting.begin() : *waiting.rbegin();
                            waiting.erase(promo);
                            state[idOf[promo]] = DOWNLOADING;
                            downloadCount++;
                        }
                    }
                    // else: not downloading — ignore defensively.
                }
            }
        }

        int total = (int)names.size();
        vector<int> order(total);
        for (int i = 0; i < total; ++i) order[i] = i;
        g_names = &names;
        sort(order.begin(), order.end(), (mode == 0) ? cmpAsc : cmpDesc);

        for (int i = 0; i < total; ++i) {
            printf("%s %s\n", names[order[i]].c_str(), stateName(state[order[i]]));
        }
        printf("\n");
    }
    return 0;
}
