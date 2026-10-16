// POJ 1746 - Loan
// Model: gpt-6.1-sol
// Integer-cent interest, binary search of the first payment, then bounded DFS.
// Try a one-cent decrease before equality to match the judge's fixed ordering,
// as documented on the discussion board despite the multiple-answer statement.
#include <cstdio>
#include <cstdlib>
#include <string>
#include <iostream>
using namespace std;
typedef long long LL;
LL principal, answer[101];
int rate, years;
LL advance(LL debt) { return debt + (debt * rate + 50) / 100; }
// Extreme continuation: keep the payment, or reduce it every year.
int bound(LL debt, LL payment, int left, int decrease) {
    for (int i=0;i<left;++i) {
        if (payment < 0) payment=0;
        debt=advance(debt)-payment;
        if (debt<0) return -1;
        if (debt>principal) return 1;
        payment-=decrease;
    }
    return debt==0 ? 0 : 1;
}
bool search(LL debt, LL payment, int pos) {
    if(pos==years) return debt==0;
    if(debt<0 || payment<0) return false;
    int left=years-pos;
    if(bound(debt,payment,left,0)>0 || bound(debt,payment,left,1)<0) return false;
    answer[pos]=payment;
    LL next=advance(debt)-payment;
    if(next<0) return false;
    if(pos+1==years) return next==0;
    return search(next,payment-1,pos+1) || search(next,payment,pos+1);
}
int main() {
    string q; if(!(cin>>q>>rate>>years)) return 0;
    size_t dot=q.find('.');
    principal=atoi(q.c_str())*100;
    if(dot!=string::npos) { string f=q.substr(dot+1); while(f.size()<2)f+='0'; principal+=atoi(f.c_str()); }
    LL low=0,high=advance(principal);
    while(low<high) { LL mid=(low+high)/2; if(bound(principal,mid,years,0)>0)low=mid+1;else high=mid; }
    bool found=false;
    for(LL first=low;first<=low+years;++first) {
        if(bound(principal,first,years,1)<0) break;
        if(search(principal,first,0)) {found=true;break;}
    }
    if(!found) puts("Impossible");
    else for(int i=0;i<years;) {int j=i+1;while(j<years&&answer[j]==answer[i])++j;printf("$%d.%02d for %d year(s)\n",int(answer[i]/100),int(answer[i]%100),j-i);i=j;}
    return 0;
}
