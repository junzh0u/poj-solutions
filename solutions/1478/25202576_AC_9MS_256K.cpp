// POJ 1478 - Island of Logic
// Model: claude-sonnet-5
// Approach: brute-force enumeration. At most 5 distinct inhabitants (A-E)
// appear in a conversation (as speaker or referenced by name); enumerate
// every assignment of {divine, human, evil} to each relevant inhabitant and
// every choice of day/night (3^5 * 2 = 486 combinations), check every
// statement for consistency (a statement's asserted boolean must equal
// whether its speaker is currently truthful: divine always truthful, evil
// never, human truthful iff day), and collect which facts (an inhabitant's
// type, or day/night) are constant across all valid combinations.
// No valid combination -> "This is impossible."; valid combinations but no
// constant fact -> "No facts are deducible."; otherwise print the constant
// facts (inhabitants alphabetically, then day/night).
// Statement "X is lying" / "I am lying" is a statement about the CURRENT
// truthfulness of X (lying = evil, or human at night), not a type; verified
// by hand against all four sample conversations, including the self-refuting
// "I am lying." (always impossible, matching sample #2's liar paradox) and
// the two-statement deduction in sample #4.
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
#include <sstream>
using namespace std;

enum { DIVINE = 0, HUMAN = 1, EVIL = 2 };
enum { KIND_TYPE = 0, KIND_LYING = 1, KIND_DAY = 2 };

struct Statement {
    int speaker;   // 0..4 (A..E)
    int kind;      // KIND_TYPE, KIND_LYING, KIND_DAY
    int subject;   // for TYPE/LYING
    int typeVal;   // for TYPE: DIVINE/HUMAN/EVIL
    bool negate;   // for TYPE/LYING
    bool dayIsDay; // for DAY: true = "It is day.", false = "It is night."
};

static int typeWordToVal(const string &w) {
    if (w == "divine") return DIVINE;
    if (w == "human") return HUMAN;
    if (w == "evil") return EVIL;
    return -1; // "lying"
}

int main() {
    int n;
    int caseNo = 0;
    // consume trailing newline handling via cin/getline mixed with scanf carefully:
    // we'll read n via scanf, then read n lines via getline after eating the rest of line.
    while (scanf("%d", &n) == 1) {
        if (n == 0) break;
        caseNo++;
        char buf[256];
        // eat rest of current line
        fgets(buf, sizeof(buf), stdin);

        vector<Statement> stmts(n);
        bool relevant[5] = { false, false, false, false, false };

        for (int i = 0; i < n; i++) {
            if (!fgets(buf, sizeof(buf), stdin)) {
                buf[0] = '\0';
            }
            string line(buf);
            // strip trailing CR/LF
            while (!line.empty() && (line[line.size()-1] == '\n' || line[line.size()-1] == '\r'))
                line.erase(line.size()-1);

            int speaker = line[0] - 'A';
            relevant[speaker] = true;
            // content starts after "X: "
            size_t pos = line.find(':');
            string rest = line.substr(pos + 1);
            // strip leading spaces
            size_t start = rest.find_first_not_of(' ');
            rest = rest.substr(start);
            // strip trailing period
            if (!rest.empty() && rest[rest.size()-1] == '.')
                rest.erase(rest.size()-1);

            // split into words
            vector<string> words;
            {
                istringstream iss(rest);
                string w;
                while (iss >> w) words.push_back(w);
            }

            Statement st;
            st.speaker = speaker;

            if (words[0] == "I") {
                // "I am [not] TYPEWORD"
                int idx = 2; // words[1] == "am"
                bool neg = false;
                if (words[idx] == "not") { neg = true; idx++; }
                string tw = words[idx];
                st.subject = speaker;
                st.negate = neg;
                if (tw == "lying") {
                    st.kind = KIND_LYING;
                } else {
                    st.kind = KIND_TYPE;
                    st.typeVal = typeWordToVal(tw);
                }
            } else if (words[0] == "It") {
                // "It is (day|night)"
                st.kind = KIND_DAY;
                st.dayIsDay = (words[2] == "day");
            } else {
                // "<letter> is [not] TYPEWORD"
                int subj = words[0][0] - 'A';
                relevant[subj] = true;
                int idx = 2; // words[1] == "is"
                bool neg = false;
                if (words[idx] == "not") { neg = true; idx++; }
                string tw = words[idx];
                st.subject = subj;
                st.negate = neg;
                if (tw == "lying") {
                    st.kind = KIND_LYING;
                } else {
                    st.kind = KIND_TYPE;
                    st.typeVal = typeWordToVal(tw);
                }
            }

            stmts[i] = st;
        }

        // Enumerate all type assignments (3^5) x day/night (2)
        int possibleTypes[5] = {0, 0, 0, 0, 0}; // bitmask of possible types per person
        bool sawDay = false, sawNight = false;
        int validCount = 0;

        int types[5];
        for (int code = 0; code < 243; code++) {
            int c = code;
            for (int p = 0; p < 5; p++) { types[p] = c % 3; c /= 3; }
            for (int dayCode = 0; dayCode < 2; dayCode++) {
                bool isDay = (dayCode == 1);
                bool ok = true;
                for (size_t s = 0; s < stmts.size(); s++) {
                    const Statement &st = stmts[s];
                    bool assertionValue;
                    if (st.kind == KIND_TYPE) {
                        assertionValue = (types[st.subject] == st.typeVal);
                        if (st.negate) assertionValue = !assertionValue;
                    } else if (st.kind == KIND_LYING) {
                        bool lying = (types[st.subject] == EVIL) ||
                                     (types[st.subject] == HUMAN && !isDay);
                        assertionValue = lying;
                        if (st.negate) assertionValue = !assertionValue;
                    } else { // KIND_DAY
                        assertionValue = (isDay == st.dayIsDay);
                    }

                    int spk = st.speaker;
                    bool speakerTruthful;
                    if (types[spk] == DIVINE) speakerTruthful = true;
                    else if (types[spk] == EVIL) speakerTruthful = false;
                    else speakerTruthful = isDay;

                    if (assertionValue != speakerTruthful) { ok = false; break; }
                }
                if (ok) {
                    validCount++;
                    for (int p = 0; p < 5; p++) {
                        if (relevant[p]) possibleTypes[p] |= (1 << types[p]);
                    }
                    if (isDay) sawDay = true; else sawNight = true;
                }
            }
        }

        printf("Conversation #%d\n", caseNo);
        if (validCount == 0) {
            printf("This is impossible.\n\n");
            continue;
        }

        bool anyFact = false;
        string lines;
        for (int p = 0; p < 5; p++) {
            if (!relevant[p]) continue;
            int mask = possibleTypes[p];
            // count bits
            int cnt = 0, only = -1;
            for (int t = 0; t < 3; t++) if (mask & (1 << t)) { cnt++; only = t; }
            if (cnt == 1) {
                anyFact = true;
                char c = 'A' + p;
                const char *tn = (only == DIVINE) ? "divine" : (only == HUMAN) ? "human" : "evil";
                printf("%c is %s.\n", c, tn);
            }
        }
        if (sawDay != sawNight) {
            anyFact = true;
            printf("It is %s.\n", sawDay ? "day" : "night");
        }

        if (!anyFact) {
            printf("No facts are deducible.\n\n");
        } else {
            printf("\n");
        }
    }
    return 0;
}
