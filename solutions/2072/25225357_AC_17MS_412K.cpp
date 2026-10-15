// POJ 2072 - GHOST
// Model: gpt-6.1-sol
// Sort the vocabulary and stop each playable branch at its first word of
// at least four letters. Classify each next letter by the players who can
// complete those terminal words; prefer an always-safe letter to a mixed one.
// Words shorter than four letters are ignored, as clarified by the board.
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>
using namespace std;
int main() {
    string line;
    while (getline(cin, line)) {
        if (!line.empty() && line[line.size()-1]=='\r') line.erase(line.size()-1);
        int n=0; istringstream in(line); in>>n;
        if (n<2) break;
        vector<string> words;
        while (getline(cin,line)) {
            if (!line.empty() && line[line.size()-1]=='\r') line.erase(line.size()-1);
            if (line.empty()) break;
            if (line.size()>=4) words.push_back(line);
        }
        string p; getline(cin,p);
        if (!p.empty() && p[p.size()-1]=='\r') p.erase(p.size()-1);
        sort(words.begin(),words.end());
        vector<string> terminal;
        for (size_t i=0;i<words.size();++i)
            if (terminal.empty() || words[i].compare(0,terminal.back().size(),terminal.back())!=0)
                terminal.push_back(words[i]);
        bool win[26]={false}, lose[26]={false}, valid=false, complete=false;
        for (size_t i=0;i<terminal.size();++i) {
            const string &w=terminal[i];
            if (p.compare(0,w.size(),w)==0) complete=true;
            if (w.size()<=p.size() || w.compare(0,p.size(),p)!=0) continue;
            valid=true;
            int c=w[p.size()]-'a';
            if ((w.size()-p.size()-1)%n==0) lose[c]=true;
            else win[c]=true;
        }
        cout<<p<<' ';
        if (complete || !valid) {cout<<"Challenge\n"; continue;}
        int choice=-1;
        for (int c=0;c<26;++c) if(win[c]&&!lose[c]) {choice=c;break;}
        if(choice<0) for(int c=0;c<26;++c) if(win[c]) {choice=c;break;}
        if(choice<0) cout<<"Bluff\n";
        else cout<<char('a'+choice)<<'\n';
    }
    return 0;
}
