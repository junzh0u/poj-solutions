// POJ 2162 - Document Indexing
// Model: claude-sonnet-5
// Approach: Simulate pagination of the document with the four widow/orphan
// correction rules applied to a flattened sequence of "items" (content lines
// and paragraph-separating blank lines). Each page tentatively takes n items;
// if the item that would be the n-th (last) one on the page is a content
// line, check which of the four rules applies (based on its position within
// its paragraph and the paragraph's length) and shrink the page accordingly,
// carrying the displaced line(s) to the next page. Once every content line
// is assigned a page number, extract words (maximal runs of English letters,
// case-insensitive) from each line and record the set of pages each word
// occurs on, then print them in lexicographic order with page numbers
// collapsed into ranges of length >= 3.
//
// Statement ambiguity: none found in the discuss board beyond confirming
// that any maximal run of letters counts as a word even when embedded among
// digits/punctuation (e.g. "12ABC34" -> "ABC") -- consistent with the sample
// ("world's" -> WORLD, S; "seventy-five" -> SEVENTY, FIVE).
#include <cstdio>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <iostream>
using namespace std;

struct Item {
    bool isSep;
    int paraId;
    int lineIdx;
};

static void extractWords(const string &line, int page, map<string, set<int> > &index) {
    string cur;
    for (size_t i = 0; i <= line.size(); ++i) {
        char c = (i < line.size()) ? line[i] : '\0';
        bool isLetter = (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
        if (isLetter) {
            if (c >= 'a' && c <= 'z') c = c - 'a' + 'A';
            cur.push_back(c);
        } else {
            if (!cur.empty()) {
                index[cur].insert(page);
                cur.clear();
            }
        }
    }
}

int main() {
    ios::sync_with_stdio(false);
    int n;
    if (!(cin >> n)) return 0;
    string dummy;
    getline(cin, dummy);

    vector<string> lines;
    string line;
    while (getline(cin, line)) {
        if (!line.empty() && line[line.size() - 1] == '\r') line.erase(line.size() - 1);
        lines.push_back(line);
    }

    vector<vector<string> > paragraphs;
    vector<string> cur;
    for (size_t i = 0; i < lines.size(); ++i) {
        if (lines[i].empty()) {
            if (!cur.empty()) {
                paragraphs.push_back(cur);
                cur.clear();
            }
        } else {
            cur.push_back(lines[i]);
        }
    }
    if (!cur.empty()) paragraphs.push_back(cur);

    vector<Item> items;
    for (size_t p = 0; p < paragraphs.size(); ++p) {
        for (size_t l = 0; l < paragraphs[p].size(); ++l) {
            Item it;
            it.isSep = false;
            it.paraId = (int)p;
            it.lineIdx = (int)l;
            items.push_back(it);
        }
        if (p + 1 < paragraphs.size()) {
            Item sep;
            sep.isSep = true;
            sep.paraId = -1;
            sep.lineIdx = -1;
            items.push_back(sep);
        }
    }

    map<string, set<int> > index;
    int N = (int)items.size();

    // process a contiguous range [l, r] (inclusive, may be empty if l>r) as page 'page'
    // implemented inline below.

    int pos = 0;
    int page = 1;
    while (pos < N) {
        int remaining = N - pos;
        if (remaining <= n) {
            for (int i = pos; i < N; ++i) {
                if (!items[i].isSep) {
                    extractWords(paragraphs[items[i].paraId][items[i].lineIdx], page, index);
                }
            }
            pos = N;
            break;
        }
        int pageEnd = pos + n - 1; // 0-indexed, guaranteed < N-1
        const Item &it = items[pageEnd];
        if (it.isSep) {
            for (int i = pos; i <= pageEnd; ++i) {
                if (!items[i].isSep) {
                    extractWords(paragraphs[items[i].paraId][items[i].lineIdx], page, index);
                }
            }
            pos = pageEnd + 1;
        } else {
            int paraLen = (int)paragraphs[it.paraId].size();
            int li = it.lineIdx;
            if (li == paraLen - 1) {
                // rule 1: last line of paragraph -> place through pageEnd, drop following sep
                for (int i = pos; i <= pageEnd; ++i) {
                    if (!items[i].isSep) {
                        extractWords(paragraphs[items[i].paraId][items[i].lineIdx], page, index);
                    }
                }
                int nextPos = pageEnd + 1;
                if (nextPos < N && items[nextPos].isSep) nextPos++;
                pos = nextPos;
            } else if (li == 0 && paraLen > 1) {
                // rule 2: orphan
                for (int i = pos; i <= pageEnd - 1; ++i) {
                    if (!items[i].isSep) {
                        extractWords(paragraphs[items[i].paraId][items[i].lineIdx], page, index);
                    }
                }
                pos = pageEnd;
            } else if (li == paraLen - 2 && paraLen > 3) {
                // rule 3: widow
                for (int i = pos; i <= pageEnd - 1; ++i) {
                    if (!items[i].isSep) {
                        extractWords(paragraphs[items[i].paraId][items[i].lineIdx], page, index);
                    }
                }
                pos = pageEnd;
            } else if (li == paraLen - 2 && (paraLen == 2 || paraLen == 3)) {
                // rule 4: move whole paragraph
                int firstLinePos = pageEnd - li;
                for (int i = pos; i <= firstLinePos - 1; ++i) {
                    if (!items[i].isSep) {
                        extractWords(paragraphs[items[i].paraId][items[i].lineIdx], page, index);
                    }
                }
                pos = firstLinePos;
            } else {
                for (int i = pos; i <= pageEnd; ++i) {
                    if (!items[i].isSep) {
                        extractWords(paragraphs[items[i].paraId][items[i].lineIdx], page, index);
                    }
                }
                pos = pageEnd + 1;
            }
        }
        page++;
    }

    for (map<string, set<int> >::iterator it = index.begin(); it != index.end(); ++it) {
        printf("%s ", it->first.c_str());
        const set<int> &pages = it->second;
        bool first = true;
        set<int>::const_iterator si = pages.begin();
        while (si != pages.end()) {
            int start = *si;
            int end = start;
            set<int>::const_iterator nxt = si;
            ++nxt;
            while (nxt != pages.end() && *nxt == end + 1) {
                end = *nxt;
                ++nxt;
            }
            int runLen = end - start + 1;
            if (!first) printf(",");
            first = false;
            if (runLen >= 3) {
                printf("%d-%d", start, end);
            } else if (runLen == 2) {
                printf("%d,%d", start, end);
            } else {
                printf("%d", start);
            }
            si = nxt;
        }
        printf("\n");
    }

    return 0;
}
