// POJ 2604 - Preparing an article
// Model: gpt-6.1-sol
// Scan TeX commands and pair ordinary quotes within each paragraph.
// Keep the pending opening quote's output position so an unmatched one
// can be deleted at a blank line, an exact \par command, or end of input.
// Alphabetic command names are maximal; \" is copied without conversion.
#include <iostream>
#include <string>
#include <cctype>
using namespace std;
void finish(string &out, string::size_type &pending) {
    if (pending != string::npos) out.erase(pending, 2);
    pending = string::npos;
}
int main() {
    string line, out;
    string::size_type pending = string::npos;
    bool done = false;
    while (!done && getline(cin, line)) {
        if (!line.empty() && line[line.size()-1] == '\r') line.erase(line.size()-1);
        if (line.find_first_not_of(" \t") == string::npos) finish(out, pending);
        for (string::size_type i = 0; i < line.size();) {
            if (line[i] == '\\') {
                string::size_type j = i + 1;
                if (j < line.size() && isalpha((unsigned char)line[j])) {
                    while (j < line.size() && isalpha((unsigned char)line[j])) ++j;
                } else if (j < line.size()) ++j;
                string command = line.substr(i, j-i);
                if (command == "\\par" || command == "\\endinput") finish(out, pending);
                out += command;
                i = j;
                if (command == "\\endinput") {
                    out += line.substr(i);
                    done = true;
                    break;
                }
            } else if (line[i] == '"') {
                if (pending == string::npos) {
                    pending = out.size();
                    out += "``";
                } else {
                    out += "''";
                    pending = string::npos;
                }
                ++i;
            } else out += line[i++];
        }
        out += '\n';
    }
    finish(out, pending);
    cout << out;
    return 0;
}
