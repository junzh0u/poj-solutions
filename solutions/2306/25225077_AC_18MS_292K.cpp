// POJ 2306 - Genealogical Research
// Model: gpt-6.1-sol
// Store birth records by name and traverse relatives in alphabetical order.
// Parent roles do not imply gender; unknown parents print only their names.
// Repeated ancestry paths are printed independently, not globally deduplicated.
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <algorithm>
using namespace std;
struct Person { string birth, death, mother, father; };
map<string, Person> people;
string trim(const string &s) {
    size_t a=s.find_first_not_of(" \t\r\n");
    if(a==string::npos) return "";
    return s.substr(a,s.find_last_not_of(" \t\r\n")-a+1);
}
void printPerson(const string &name,int depth) {
    cout << string(depth,' ') << name;
    map<string,Person>::const_iterator it=people.find(name);
    if(it!=people.end()) {
        cout << " " << it->second.birth << " -";
        if(!it->second.death.empty()) cout << " " << it->second.death;
    }
    cout << '\n';
}
void tree(const string &name,int depth,bool ancestors) {
    vector<string> relatives;
    if(ancestors) {
        map<string,Person>::const_iterator it=people.find(name);
        if(it==people.end()) return;
        relatives.push_back(it->second.mother);
        relatives.push_back(it->second.father);
        sort(relatives.begin(),relatives.end());
    } else {
        for(map<string,Person>::const_iterator it=people.begin();it!=people.end();++it)
            if(it->second.mother==name || it->second.father==name)
                relatives.push_back(it->first);
    }
    for(size_t i=0;i<relatives.size();++i) {
        printPerson(relatives[i],depth);
        tree(relatives[i],depth+2,ancestors);
    }
}
int main() {
    string line; bool first=true;
    while(getline(cin,line)) {
        line=trim(line);
        size_t space=line.find_first_of(" \t");
        string cmd=line.substr(0,space);
        if(cmd=="QUIT") break;
        if(space==string::npos) continue;
        string rest=trim(line.substr(space+1));
        vector<string> fields; size_t start=0,colon;
        while((colon=rest.find(':',start))!=string::npos) {
            fields.push_back(trim(rest.substr(start,colon-start))); start=colon+1;
        }
        fields.push_back(trim(rest.substr(start)));
        if(cmd=="BIRTH") {
            Person &p=people[fields[0]];
            p.birth=fields[1]; p.mother=fields[2]; p.father=fields[3];
        } else if(cmd=="DEATH") people[fields[0]].death=fields[1];
        else if(cmd=="ANCESTORS" || cmd=="DESCENDANTS") {
            if(!first) cout << '\n'; first=false;
            cout << cmd << " of " << rest << '\n';
            tree(rest,2,cmd=="ANCESTORS");
        }
    }
    return 0;
}
