// POJ 2896 - Changing Phone Numbers
// Model: gpt-6.1-sol
// Sweep queries by starting year to identify their area using current codes.
// Then replay only that area's rules in (year1, year2]. Initial codes precede
// all rules; changes at year1 already affect parsing but are not replayed.
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
using namespace std;
struct Rule { int year, type, area, pos; string code; };
bool ruleLess(const Rule& a,const Rule& b){return a.year<b.year;}
struct Query { int first,last,id; string number; };
bool queryLess(const Query& a,const Query& b){return a.first<b.first;}
int main(){
    ios::sync_with_stdio(false); cin.tie(0);
    int a; if(!(cin>>a)) return 0;
    map<string,int> names, codes;
    vector<string> current(a);
    for(int i=0;i<a;++i){string name;cin>>current[i]>>name;names[name]=i;codes[current[i]]=i;}
    int r;cin>>r;vector<Rule> rules(r);
    for(int i=0;i<r;++i){string name;cin>>rules[i].year>>rules[i].type>>name;rules[i].area=names[name];rules[i].pos=0;
        if(rules[i].type==3)cin>>rules[i].code;else cin>>rules[i].pos;
    }
    sort(rules.begin(),rules.end(),ruleLess);
    vector< vector<Rule> > local(a);
    for(int i=0;i<r;++i)local[rules[i].area].push_back(rules[i]);
    vector<Query> queries;Query q;
    while(cin>>q.first>>q.last>>q.number){if(q.first==0&&q.last==0&&q.number=="0")break;q.id=queries.size();queries.push_back(q);}
    vector<string> answers(queries.size());sort(queries.begin(),queries.end(),queryLess);
    int done=0;
    for(size_t k=0;k<queries.size();++k){
        const Query& x=queries[k];
        while(done<r&&rules[done].year<=x.first){const Rule& v=rules[done++];if(v.type==3){codes.erase(current[v.area]);current[v.area]=v.code;codes[v.code]=v.area;}}
        int area=-1;size_t len=0;string prefix;
        for(size_t j=0;j<x.number.size();++j){prefix+=x.number[j];map<string,int>::iterator it=codes.find(prefix);if(it!=codes.end()){area=it->second;len=j+1;break;}}
        if(area<0)return 0;
        string code=current[area],number=x.number.substr(len);
        const vector<Rule>& v=local[area];
        size_t lo=0,hi=v.size();while(lo<hi){size_t mid=(lo+hi)/2;if(v[mid].year<=x.first)lo=mid+1;else hi=mid;}
        for(size_t j=lo;j<v.size()&&v[j].year<=x.last;++j){
            if(v[j].type==3)code=v[j].code;
            else if(v[j].type==1)number.insert(v[j].pos-1,1,number[v[j].pos-1]);
            else swap(number[v[j].pos-1],number[v[j].pos]);
        }
        answers[x.id]=code+number;
    }
    for(size_t i=0;i<answers.size();++i)cout<<answers[i]<<'\n';
    return 0;
}
