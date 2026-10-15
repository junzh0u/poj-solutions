// POJ 1876 - The Letter Carrier's Rounds
// Model: gpt-6.1-sol
// Group recipients by destination in first-appearance order, deduplicate each
// address, and simulate one SMTP session per group. The discussion clarifies
// that the unspecified MTA order must be first appearance for this judge.
// Preserve message lines, including blanks, and indent by exactly five spaces.
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <set>
using namespace std;
void line(const string &s) { cout << "     " << s << '\n'; }
int main() {
    map<string, set<string> > users;
    string s, host, user;
    int n;
    while (cin >> s && s[0] != '*') {
        cin >> host >> n;
        for (int i=0;i<n;++i) { cin >> user; users[host].insert(user); }
    }
    string sender;
    while (cin >> sender && sender[0] != '*') {
        vector<string> hosts;
        map<string, vector<string> > groups;
        set<string> seen;
        while (cin >> s && s[0] != '*') {
            if (!seen.insert(s).second) continue;
            host=s.substr(s.find('@')+1);
            if (groups.find(host)==groups.end()) hosts.push_back(host);
            groups[host].push_back(s);
        }
        getline(cin,s);
        vector<string> body;
        while (getline(cin,s)) {
            if (!s.empty() && s[s.size()-1]=='\r') s.erase(s.size()-1);
            if (!s.empty() && s[0]=='*') break;
            body.push_back(s);
        }
        string origin=sender.substr(sender.find('@')+1);
        for (size_t i=0;i<hosts.size();++i) {
            host=hosts[i];
            cout << "Connection between " << origin << " and " << host << '\n';
            line("HELO "+origin); line("250");
            line("MAIL FROM:<"+sender+">"); line("250");
            bool accepted=false;
            vector<string> &rec=groups[host];
            for (size_t j=0;j<rec.size();++j) {
                line("RCPT TO:<"+rec[j]+">");
                bool valid=users[host].count(rec[j].substr(0,rec[j].find('@')))!=0;
                line(valid?"250":"550");
                accepted=accepted || valid;
            }
            if (accepted) {
                line("DATA"); line("354");
                for (size_t j=0;j<body.size();++j) line(body[j]);
                line("."); line("250");
            }
            line("QUIT"); line("221");
        }
    }
    return 0;
}
