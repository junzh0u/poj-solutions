// POJ 3550 - AT - sequence
// Model: claude-sonnet-5
// Approach: parse an AT-sequence into its commands. A command's mandatory
// header part is either one or two uppercase letters, or one of &,#,@
// followed by one uppercase letter; an optional numeric header suffix
// (digits) may follow, and an optional "=value" (digits) may follow that.
// The statement's disambiguation rule ("a command notation consisting only
// of one or two Latin letters cannot precede another command whose notation
// starts with a letter") guarantees that in a *correct* AT-sequence any
// maximal run of consecutive uppercase letters not introduced by &/#/@ is at
// most 2 characters long and belongs to a single command's mandatory part:
// splitting such a run into two commands would always create a bare
// letters-only command immediately followed by a letter-starting command,
// which is forbidden. So a left-to-right greedy scan (take up to 2 letters,
// then digits, then an optional "=" + digits) reproduces the unique valid
// parse; no backtracking or validation of the input is needed since the
// input is guaranteed to already be a correct AT-sequence.
#include <cstdio>
#include <cctype>
#include <string>
#include <iostream>

int main() {
    std::string line;
    if (!std::getline(std::cin, line)) return 0;
    while (!line.empty() && (line[line.size()-1] == '\r' || line[line.size()-1] == '\n'))
        line.erase(line.size()-1);

    size_t pos = 0;
    size_t n = line.size();
    if (n >= 2 && line[0] == 'A' && line[1] == 'T') pos = 2;

    while (pos < n) {
        std::string cmd;
        char c = line[pos];
        if (c == '&' || c == '#' || c == '@') {
            cmd += line[pos];
            cmd += line[pos + 1];
            pos += 2;
        } else {
            cmd += line[pos];
            pos++;
            if (pos < n && line[pos] >= 'A' && line[pos] <= 'Z') {
                cmd += line[pos];
                pos++;
            }
        }
        while (pos < n && isdigit((unsigned char)line[pos])) {
            cmd += line[pos];
            pos++;
        }
        if (pos < n && line[pos] == '=') {
            cmd += line[pos];
            pos++;
            while (pos < n && isdigit((unsigned char)line[pos])) {
                cmd += line[pos];
                pos++;
            }
        }
        std::cout << cmd << "\n";
    }
    return 0;
}
