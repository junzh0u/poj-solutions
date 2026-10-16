// POJ 1540 - The Finite State Text-Processing
// Model: gpt-6.1-sol
// Simulate named states, preferring explicit input sets to the catch-all.
// Decode escapes once; keep output-current-character distinct from literals.
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <limits>
using namespace std;
struct Transition { string input, next; vector<int> output; bool fallback; };
vector<int> decode(const string &s) {
    vector<int> v;
    for (size_t i=0;i<s.size();++i) {
        if(s[i]!='\\') v.push_back((unsigned char)s[i]);
        else {
            char c=s[++i];
            if(c=='b') v.push_back(' ');
            else if(c=='n') v.push_back('\n');
            else if(c=='c') v.push_back(256);
            else if(c!='0') v.push_back((unsigned char)c);
        }
    }
    return v;
}
int main() {
    int n, number=0;
    while(cin>>n && n) {
        map<string, vector<Transition> > states;
        for(int i=0;i<n;++i) {
            string name; int count; cin>>name>>count;
            for(int j=0;j<count;++j) {
                string in,out; Transition t; cin>>in>>t.next>>out;
                vector<int> chars=decode(in);
                t.fallback=false;
                for(size_t k=0;k<chars.size();++k)
                    if(chars[k]==256) t.fallback=true;
                    else t.input+=(char)chars[k];
                t.output=decode(out); states[name].push_back(t);
            }
        }
        cin.ignore(numeric_limits<streamsize>::max(),'\n');
        cout<<"Finite State Machine "<<++number<<":\n";
        string current="START"; char c=0;
        while(current!="END" && cin.get(c)) {
            if(c=='\r') continue;
            vector<Transition> &ts=states[current];
            int chosen=-1, fallback=-1;
            for(size_t i=0;i<ts.size();++i) {
                if(ts[i].fallback) fallback=(int)i;
                if(ts[i].input.find(c)!=string::npos) { chosen=(int)i; break; }
            }
            if(chosen<0) chosen=fallback;
            if(chosen<0) continue;
            Transition &t=ts[chosen];
            for(size_t i=0;i<t.output.size();++i)
                cout.put(t.output[i]==256?c:(char)t.output[i]);
            current=t.next;
        }
        if(current=="END" && c!='\n') cin.ignore(numeric_limits<streamsize>::max(),'\n');
    }
    return 0;
}
