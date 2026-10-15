// POJ 1355 - Selecting the Optimal Teaching Fiends
// Model: gpt-6.1-sol
// Validate each complete ballot before accumulating rank counts and scores.
// Sort by score, vote count, then ID. IDs are those listed, not necessarily
// 1..n; repeated 00 vacancies are allowed. Process cases through -1.
#include <iostream>
#include <cstdio>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;
struct Teacher {
    int id, score, votes, count[10];
    string name, department;
    Teacher():id(0),score(0),votes(0) { for(int j=0;j<10;++j) count[j]=0; }
};
bool before(const Teacher &a,const Teacher &b) {
    if(a.score!=b.score) return a.score>b.score;
    if(a.votes!=b.votes) return a.votes>b.votes;
    return a.id<b.id;
}
int main() {
    int n,m;
    const int points[10]={15,12,9,7,6,5,4,3,2,1};
    while(cin>>n && n!=-1) {
        cin>>m;
        vector<Teacher> t(n);
        int index[100];
        for(int i=0;i<100;++i) index[i]=-1;
        for(int i=0;i<n;++i) {
            cin>>t[i].id>>t[i].name>>t[i].department;
            if(t[i].id>0 && t[i].id<100) index[t[i].id]=i;
        }
        for(int i=0;i<m;++i) {
            string s; cin>>s;
            bool valid=s.size()==20, seen[100]={false};
            int ids[10]={0};
            if(valid) for(int j=0;j<10;++j) {
                if(s[2*j]<'0'||s[2*j]>'9'||s[2*j+1]<'0'||s[2*j+1]>'9') { valid=false; break; }
                int id=ids[j]=(s[2*j]-'0')*10+s[2*j+1]-'0';
                if(id && (index[id]<0 || seen[id])) valid=false;
                seen[id]=true;
            }
            if(valid) for(int j=0;j<10;++j) if(ids[j]) {
                Teacher &a=t[index[ids[j]]];
                a.score+=points[j]; ++a.votes; ++a.count[j];
            }
        }
        sort(t.begin(),t.end(),before);
        for(int i=0;i<n && i<10;++i) {
            const Teacher &a=t[i];
            printf("%-2d  %02d  %-15s%-37s%6d%6d",i+1,a.id,a.name.c_str(),a.department.c_str(),a.score,a.votes);
            for(int j=0;j<10;++j) printf("%6d",a.count[j]);
            printf("\n");
        }
    }
    return 0;
}
