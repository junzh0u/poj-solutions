// POJ 3699 - miniSQL
// Model: gpt-5.6-terra
// Parse every query, retain the matching input-order rows, and size each
// projected column from its header and those rows before rendering it.
// Cell content is centered in a field with one mandatory space on each side.
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <cstdlib>
using namespace std;

struct Column {
    string name;
    bool isString;
};

static string trim(const string &s) {
    size_t a = 0, b = s.size();
    while (a < b && (s[a] == ' ' || s[a] == '\t' || s[a] == '\r')) ++a;
    while (b > a && (s[b - 1] == ' ' || s[b - 1] == '\t' || s[b - 1] == '\r')) --b;
    return s.substr(a, b - a);
}

static void printBorder(const vector<int> &width) {
    int i, j;
    cout << '+';
    for (i = 0; i < (int)width.size(); ++i) {
        for (j = 0; j < width[i] + 2; ++j) cout << '-';
        if (i + 1 < (int)width.size()) cout << '-';
    }
    cout << "+\n";
}

static void printRule(const vector<int> &width) {
    int i, j;
    for (i = 0; i < (int)width.size(); ++i) {
        cout << '|';
        for (j = 0; j < width[i] + 2; ++j) cout << '-';
    }
    cout << "|\n";
}

static void printCell(const string &s, int width) {
    int extra = width - (int)s.size();
    int left = extra / 2 + 1;
    int right = extra - extra / 2 + 1;
    cout << '|';
    while (left--) cout << ' ';
    cout << s;
    while (right--) cout << ' ';
}

int main() {
    int m, n, k, i, r, q;
    if (!(cin >> m >> n >> k)) return 0;
    vector<Column> col(m);
    map<string, int> id;
    for (i = 0; i < m; ++i) {
        string type;
        cin >> col[i].name >> type;
        col[i].isString = (type == "STR");
        id[col[i].name] = i;
    }
    vector< vector<string> > table(n, vector<string>(m));
    for (r = 0; r < n; ++r)
        for (i = 0; i < m; ++i) cin >> table[r][i];
    string line;
    getline(cin, line);
    for (q = 0; q < k; ++q) {
        getline(cin, line);
        size_t where = line.find(" where ");
        string list = line.substr(7, where - 7);
        string cond = trim(line.substr(where + 7));
        vector<int> out;
        size_t pos = 0;
        while (1) {
            size_t comma = list.find(',', pos);
            out.push_back(id[trim(list.substr(pos, comma == string::npos ? string::npos : comma - pos))]);
            if (comma == string::npos) break;
            pos = comma + 1;
        }
        size_t opPos = cond.find_first_of("<>=");
        string condName = trim(cond.substr(0, opPos));
        char op = cond[opPos];
        string value = trim(cond.substr(opPos + 1));
        int ci = id[condName];
        if (col[ci].isString) value = value.substr(1, value.size() - 2);
        vector<int> rows;
        for (r = 0; r < n; ++r) {
            bool ok;
            if (col[ci].isString) ok = (table[r][ci] == value);
            else {
                long long a = strtoll(table[r][ci].c_str(), 0, 10);
                long long b = strtoll(value.c_str(), 0, 10);
                ok = (op == '=' ? a == b : (op == '<' ? a < b : a > b));
            }
            if (ok) rows.push_back(r);
        }
        vector<int> width(out.size());
        for (i = 0; i < (int)out.size(); ++i) {
            width[i] = col[out[i]].name.size();
            for (r = 0; r < (int)rows.size(); ++r)
                if ((int)table[rows[r]][out[i]].size() > width[i]) width[i] = table[rows[r]][out[i]].size();
        }
        printBorder(width);
        for (i = 0; i < (int)out.size(); ++i) printCell(col[out[i]].name, width[i]);
        cout << "|\n";
        printRule(width);
        for (r = 0; r < (int)rows.size(); ++r) {
            for (i = 0; i < (int)out.size(); ++i) printCell(table[rows[r]][out[i]], width[i]);
            cout << "|\n";
        }
        printBorder(width);
        cout << '\n';
    }
    return 0;
}
